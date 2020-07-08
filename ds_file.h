#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


long file_size(FILE *file);

void read_file(FILE *file, void **buffer, uint32_t *buffer_size);

void write_file(void **buffer, long *buffer_size);

int file_exists(char *file_name);


#ifdef DS_FILE_IMPLEMENTATION

#include "ds_mem.h"

long file_size(FILE *file)
{
    long size;
    long offset;

    offset = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, offset, SEEK_SET);

    return size;
}

void read_file(FILE *file, void **buffer, uint32_t *buffer_size)
{
    char *file_buffer = NULL;
    long size = 0;

    if(file)
    {
        size = file_size(file);
        file_buffer = (char *)mem_Calloc(size + 1, 1);
        fread(file_buffer, size, 1, file);
        file_buffer[size] = '\0';
    }

    *buffer = (void *)file_buffer;
    *buffer_size = (uint32_t)size;
}

void write_file(void **buffer, long *buffer_size)
{

}

int file_exists(char *file_name)
{
    FILE *file;

    file = fopen(file_name, "r");

    if(file)
    {
        fclose(file);
        return 1;
    }

    return 0;
}

#endif

#endif // FILE_H






