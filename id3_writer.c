/**
 * @file id3_writer.c
 * @brief Implementation of functions for writing and editing ID3 tags in MP3 files.
 */

#include <stdio.h>
#include <string.h>
#include "id3_writer.h"
#include "id3_reader.h"
#include "id3_utils.h"

/**
 * @brief Writes the ID3 tags to an MP3 file.
 * 
 * @param filename The name of the MP3 file.
 * @param data Pointer to the TagData structure containing the ID3 tags.
 * @return 0 on success, non-zero on failure.
 */
//-t  TIT2
//-A  TPE1
//-a  TALB
//-y  TYER
//-G  TCON
//-c  COMM



/** TODO: Add documentation as sample given above */
int edit_tag(const char *filename, const char *tag, const char *value) {
    if(verify_arguments(filename,tag,value))
    {
        FILE *fptr = fopen(filename, "r");
        if (fptr == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", filename);

            return 0;
        }
        if(write_id3(fptr,tag,value))
        {
            //print statements here.
            printf("EDITED DATA->%s\n",value);
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return 1;
    // Implementation for writing ID3 tags to file
}

int verify_arguments(const char *filename, const char *tag, const char *value)
{
    char *ch=NULL;
    if(ch=strstr(filename,".mp3"))
    {
        if(strcmp(ch,".mp3")==0)
        {
            if(strcmp(tag,"-t")==0 || strcmp(tag,"-A")==0 || strcmp(tag,"-a")==0 || strcmp(tag,"-y")==0 || strcmp(tag,"-G")==0 || strcmp(tag,"-c")==0)
            {
                return 1;
            }
            else
            {
                printf("Not a valid tag\n");
                return 0;
            }
        }
        else
        {
            printf("Not a valid mp3 file\n");
            return 0;
        }
    }
    else
    {
        printf("Not a valid mp3 file\n");
        return 0;
    }
    return 1;
}


int write_id3(FILE *fptr,const char *tag, const char *value)
{
    //take temporary file 
    FILE *t_fptr=fopen("temp.mp3","w");
    if(copy_header(fptr,t_fptr))
    {
        if(search_and_edit(fptr,t_fptr,tag,value))
        {
            if(copy_remaining_data(fptr,t_fptr))
            {
                //
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

}

int copy_header(FILE *fptr,FILE *t_fptr)
{
    rewind(fptr);
    rewind(t_fptr);
    char header[10];
    int temp=0;
    temp=fread(header,1,10,fptr);
    if(temp!=10)
        return 0;
    temp=0;
    temp=fwrite(header,1,10,t_fptr);
    if(temp!=10)
        return 0;
    return 1;
}


int search_and_edit(FILE *fptr,FILE *t_fptr,const char *tag, const char *value)
{
    fseek(fptr,10l,SEEK_SET);
    fseek(t_fptr,10l,SEEK_SET);

    char temp_tag[5];
    if(strcmp(tag,"-t")==0)
        strcpy(temp_tag,"TIT2");
    else if(strcmp(tag,"-A")==0)
        strcpy(temp_tag,"TPE1");
    else if(strcmp(tag,"-a")==0)
        strcpy(temp_tag,"TALB");
    else if(strcmp(tag,"-y")==0)
        strcpy(temp_tag,"TYER");
    else if(strcmp(tag,"-G")==0)
        strcpy(temp_tag,"TCON");
    else if(strcmp(tag,"-c")==0)
        strcpy(temp_tag,"COMM");
    char tags[5];
    for(int i=0;i<6;i++)
    {
        fread(tags,1,4,fptr);
        fwrite(tags,1,4,t_fptr);
        tags[4]='\0';    
        if(strcmp(tags,temp_tag)==0)
        {
            copy_content(fptr,t_fptr,1,value);
        }
        else
        {
            copy_content(fptr,t_fptr,0,value);
        }
    }
    return 1;
}

void copy_content(FILE *fptr,FILE *t_fptr,int flag,const char *value)
{
    if(flag)
    {
        int old_size,new_size;

        new_size=strlen(value);
        new_size++;
        char temp_value[new_size];
        strncpy(temp_value,value,new_size-1);

        fread(&old_size,4,1,fptr);
        old_size=swap_endian(old_size);
 
        new_size=swap_endian(new_size);
        fwrite(&new_size,4,1,t_fptr);
        new_size=swap_endian(new_size);

        char buff[3];
        fread(buff,1,3,fptr);
        fwrite(buff,1,3,t_fptr);

        fwrite(temp_value,1,new_size-1,t_fptr);

        char temp_old[old_size];
        fread(temp_old,1,old_size-1,fptr);
    }
    else
    {
        int size;
        fread(&size,4,1,fptr);
        fwrite(&size,4,1,t_fptr);
        size=swap_endian(size);
        char buff[3];
        fread(buff,1,3,fptr);
        fwrite(buff,1,3,t_fptr);
        char content[size-1];
        fread(content,1,(size-1),fptr);
        fwrite(content,1,(size-1),t_fptr);
    }
}

int copy_remaining_data(FILE *fptr,FILE *t_fptr)
{
    long start = ftell (fptr);
    fseek(fptr,0l,SEEK_END);
    long end=ftell(fptr);
    fseek(fptr,start,SEEK_SET);

    int count;
    char data[1024];
    while ((ftell (fptr)) != end)
    {
        count = fread (data, 1, 1024, fptr);
        if (count < 0)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return 0;
        }
        else
        {
            fwrite (data, 1, count,t_fptr);
        }
    }
    return 1;
}

