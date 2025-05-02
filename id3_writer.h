#ifndef ID3_WRITER_H
#define ID3_WRITER_H

#include "id3_utils.h"

/**
 * @brief Writes the ID3 tags to an MP3 file.
 * 
 * @param filename The name of the MP3 file.
 * @param data Pointer to the TagData structure containing the ID3 tags.
 */


/**
TODO: Add documention as sample given above
 */
int edit_tag(const char *filename, const char *tag, const char *value);


int verify_arguments(const char *filename, const char *tag, const char *value);

int write_id3(FILE *fptr,const char *tag, const char *value);

int copy_header(FILE *fptr,FILE *t_fptr);

int search_and_edit(FILE *fptr,FILE *t_fptr,const char *tag, const char *value);

void copy_content(FILE *fptr,FILE *t_fptr,int flag,const char *value);

int copy_remaining_data(FILE *fptr,FILE *t_fptr);
#endif // ID3_WRITER_H
