#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct ds_record_data_t
{
    char name[64];
    uint32_t size;
    char data[];
};

struct ds_record_t
{
    struct ds_record_t *next;
    struct ds_record_t *prev;
    struct ds_record_data_t data;
};

struct ds_section_data_t
{
    char name[64];
    uint32_t size;
    uint32_t record_count;
};

struct ds_section_t
{
    struct ds_section_data_t data;
    struct ds_record_t *records;
    struct ds_record_t *last_record;
};


long file_size(FILE *file);

void read_file(FILE *file, void **buffer, uint32_t *buffer_size);

void write_file(void **buffer, long *buffer_size);

int file_exists(char *file_name);


struct ds_record_t *ds_append_record(struct ds_section_t *section, char *record_name, uint32_t size, void *data);

void ds_drop_record(struct ds_section_t *section, char *record_name);

struct ds_record_t *ds_find_record(struct ds_section_t *section, char *record_name);

void ds_free_section(struct ds_section_t *section);

void ds_serialize_section(struct ds_section_t *section, void **buffer, uint32_t *buffer_size);

struct ds_section_t ds_unserialize_section(void *buffer);


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


struct ds_record_t *ds_append_record(struct ds_section_t *section, char *record_name, uint32_t size, void *data)
{
    struct ds_record_t *record;
    
    record = mem_Calloc(1, sizeof(struct ds_record_t) + size);
    record->data.size = size;
    strcpy(record->data.name, record_name);
    memcpy(record->data.data, data, size);
    
    if(!section->records)
    {
        section->records = record;
    }
    else
    {
        section->last_record->next = record;
        record->prev = section->last_record;
    }
    section->last_record = record;
    section->data.size += sizeof(struct ds_record_t) + record->data.size;
    section->data.record_count++;
    
    return record;
}

void ds_drop_record(struct ds_section_t *section, char *record_name)
{
    struct ds_record_t *record;
    record = ds_find_record(section, record_name);
    
    if(record)
    {
        if(record->next)
        {
            record->next->prev = record->prev;
        }
        else
        {
            section->last_record = record->prev;
        }
        
        if(record->prev)
        {
            record->prev->next = record->next;
        }
        else
        {
            section->records = record->next;
        }
    }
}

struct ds_record_t *ds_find_record(struct ds_section_t *section, char *record_name)
{
    struct ds_record_t *record;
    record = section->records;
    while(record)
    {
        if(!strcmp(record_name, record->data.name))
        {
            break;
        }
        record = record->next;
    }
    
    return record;
}

void ds_free_section(struct ds_section_t *section)
{
    struct ds_record_t *record = section->records;
    struct ds_record_t *first_record = record;
    
    while(record)
    {
        struct ds_record_t *next_record = record->next;
        mem_Free(record);
        record = next_record;
    }
    
    section->records = NULL;
    section->last_record = NULL;
    section->data.record_count = 0;
    section->data.size = 0;
}

void ds_serialize_section(struct ds_section_t *section, void **buffer, uint32_t *buffer_size)
{
    uint32_t size = 0;
    char *output;
    size += sizeof(struct ds_section_data_t) + section->data.size;
    
    output = mem_Calloc(1, size);
    *buffer = output;
    *buffer_size = size;
    memcpy(output, &section->data, sizeof(struct ds_section_data_t));
    output += sizeof(struct ds_section_data_t);
    
    struct ds_record_t *record = section->records;
    while(record)
    {
        memcpy(output, &record->data, sizeof(struct ds_record_data_t) + record->data.size);
        output += sizeof(struct ds_record_data_t) + record->data.size;
        record = record->next;
    }
}

struct ds_section_t ds_unserialize_section(void *buffer)
{
    char *input = buffer;
    struct ds_section_data_t *section_data = (struct ds_section_data_t *)input;
    struct ds_section_t section = {};
    
    section.data = *section_data;
    input += sizeof(struct ds_section_data_t);
    
    for(uint32_t record_index = 0; record_index < section_data->record_count; record_index++)
    {
        struct ds_record_data_t *record_data = (struct ds_section_data_t *)input;
        input += sizeof(struct ds_record_data_t) + record_data->size;
        
        struct ds_record_t *record = mem_Calloc(1, sizeof(struct ds_record_t) + record_data->size);
        record->data = *record_data;
        memcpy(record->data.data, record_data->data, record_data->size);
        
        if(!section.records)
        {
            section.records = record;
        }
        else
        {
            section.last_record->next = record;
            record->prev = section.last_record;
        }
        section.last_record = record;
    }
    
    return section;
}

#endif

#endif // FILE_H






