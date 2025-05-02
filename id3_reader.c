/**
 * @file id3_reader.c
 * @brief Implementation of functions for reading ID3 tags from MP3 files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id3_reader.h"
#include "error_handling.h"

TagData* read_id3_tags(const char *filename) {
    // Implementation for reading ID3 tags
    char *ch=NULL;
    if(ch=strstr(filename,".mp3"))
    {
        if(strcmp(ch,".mp3")==0)
        {
            FILE *fptr=NULL;
            if(open_file(filename,&fptr))
            {
                if(check_id3_tag_presence(fptr))
                {
                    TagData *metadata=create_tag_data();
                    if(collect_metadata(&metadata,fptr))
                    {
                        return metadata;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else
                {
                    display_error("...NO ID3 Tag is present...\n");
                }
            }
        }
        else
        {
            display_error("Invalid File Type\n");
            return NULL;
        }
    }
    else
    {
        display_error("Invalid File Type\n");
        return NULL;
    }
    
    
}

void display_metadata(const TagData *data) {
    // Implementation for displaying metadata
    printf("%s \n",data->album);
    printf("%s \n",data->title);
    printf("%s \n",data->artist);
    printf("%s \n",data->year);
    printf("%s \n",data->comment);

}

/**
TODO: Add documention as sample given
 */
void view_tags(const char *filename) {
    TagData *data = read_id3_tags(filename);
    if (!data) {
        display_error("Failed to read ID3 tags.");
        return;
    }
    display_metadata(data);
    free_tag_data(data);
}

void display_error(const char *message)
{
    printf("%s\n",message);
}

int check_id3_tag_presence(FILE *fptr)
{
    char tag[3];
    //char version[2];
    fread(tag,3,1,fptr);
    if(strcmp(tag,"ID3")==0)
    {
        return 1;
    }
    return 0;
}

int open_file(const char *filename,FILE **fptr)
{
    *fptr=fopen(filename,"r");
    if (*fptr == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", filename);

    	return 0;
    }
    return 1;
}

int collect_metadata(TagData **metadata,FILE *fptr)
{
    //storing the ID3 version
    TagData *temp=*metadata;
    rewind(fptr);
    fseek(fptr,3l,SEEK_SET);
    char version;
    fread(&version,1,1,fptr);
    if((int)version==3)
    {
        temp->version=malloc(2);
        strcpy(temp->version,"3\0");
    }
    else if((int)version==2)
    {
        temp->version=malloc(2);
        strcpy(temp->version,"2\0");
    }
    printf("%s\n",temp->version);
    char tag[5];
    int size;
    int count=0;
    //skipping header
    fseek(fptr,10,SEEK_SET);
    /*
    TPE1 - Artist name
    TIT2 -Title / Song name
    TALB - Album
    TYER - Year
    TCON - Content type
    TCOM - Composer
    */
    while(count<6)
    {
        fread(tag,1,4,fptr);
        tag[4]='\0';
        fread(&size,4,1,fptr);
        size=swap_endian(size);
        if (strncmp(tag,"TIT2", 4) == 0)
        {
            int temp_count;
            (*metadata)->title = (char *)malloc(size * sizeof(char));
            fseek(fptr, 3L, SEEK_CUR);
            temp_count= fread((*metadata)->title, 1, size - 1, fptr);
            if (temp_count != size - 1)
                return 0;
            (*metadata)->title[size - 1] = '\0';
            count++;
        }
        else if(strncmp(tag,"TPE1",4)==0)
        {
            int temp_count;
            (*metadata)->artist = (char *)malloc(size * sizeof(char));
            fseek(fptr, 3L, SEEK_CUR);
            temp_count= fread((*metadata)->artist, 1, size - 1, fptr);
            if (temp_count != size - 1)
                return 0;
            (*metadata)->artist[size - 1] = '\0';
            count++;
        }
        else if(strncmp(tag,"TALB",4)==0)
        {
            int temp_count;
            (*metadata)->album = (char *)malloc(size * sizeof(char));
            fseek(fptr, 3L, SEEK_CUR);
            temp_count= fread((*metadata)->album, 1, size - 1, fptr);
            if (temp_count != size - 1)
                return 0;
            (*metadata)->album[size - 1] = '\0';
            count++;
        }
        else if(strncmp(tag,"TYER",4)==0)
        {
            int temp_count;
            (*metadata)->year = (char *)malloc(size * sizeof(char));
            fseek(fptr, 3L, SEEK_CUR);
            temp_count= fread((*metadata)->year, 1, size - 1, fptr);
            if (temp_count != size - 1)
                return 0;
            (*metadata)->year[size - 1] = '\0';
            count++;
        }
        else if(strncmp(tag,"TCON",4)==0)
        {
            int temp_count;
            (*metadata)->genre = (char *)malloc(size * sizeof(char));
            fseek(fptr, 3L, SEEK_CUR);
            temp_count= fread((*metadata)->genre, 1, size - 1, fptr);
            if (temp_count != size - 1)
                return 0;
            (*metadata)->genre[size - 1] = '\0';
            count++;
        }
        else if(strncmp(tag,"COMM",4)==0)
        {
            int temp_count;
            (*metadata)->comment = (char *)malloc(size * sizeof(char));
            fseek(fptr, 3L, SEEK_CUR);
            temp_count= fread((*metadata)->comment, 1, size - 1, fptr);
            if (temp_count != size - 1)
                return 0;
            (*metadata)->comment[size - 1] = '\0';
            count++;
        }
    }
        

    return 1;
}

int swap_endian(int size)
{
    return ((size>>24 & 0x000000ff)|(size>>8 & 0x0000ff00)|(size<<8 & 0x00ff0000)|(size<<24 & 0xff000000));
}

// int reading_and_storing(char **content,int size,FILE *fptr)
// {
//     int count;
//     (*content)=(char*)malloc(size*sizeof(char));
//     fseek (fptr, 3L, SEEK_CUR);
//     count=fread(content,1,size-1,fptr);
//     if(count!=size-1)
//         return 0;
//     content[size-1]='\0';
//     printf("%d\n",size);
//     printf("%s \n",content);
//     return 1;
// }