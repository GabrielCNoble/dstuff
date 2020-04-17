#ifndef LIST_H
#define LIST_H

#include <stdint.h>

struct list_t
{
    void **buffers;
    uint32_t buffer_size;
    uint32_t elem_size;
    uint32_t cursor;
    uint32_t size;
};


struct list_t create_list(uint32_t elem_size, uint32_t buffer_size);

void destroy_list(struct list_t *list);

void expand_list(struct list_t *list, uint32_t elem_count);

void *get_list_element(struct list_t *list, uint32_t index);

uint32_t add_list_element(struct list_t *list, void *element);

void remove_list_element(struct list_t *list, uint32_t index);

#ifdef DSTUFF_CONTAINERS_LIST_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct list_t create_list(uint32_t elem_size, uint32_t buffer_size)
{
    struct list_t list;
    memset(&list, 0, sizeof(struct list_t));

    list.buffer_size = buffer_size;
    list.elem_size = elem_size;

    expand_list(&list, 1);

    return list;
}

void destroy_list(struct list_t *list)
{
    if(list->buffers)
    {
        for(uint32_t i = 0; i < list->size / list->buffer_size; i++)
        {
            free(list->buffers[i]);
        }

        free(list->buffers);
    }
}

void expand_list(struct list_t *list, uint32_t elem_count)
{
    void **buffers;
    uint32_t buffer_count;
    uint32_t list_buffer_count;

    elem_count = (elem_count + list->buffer_size - 1) & (~(list->buffer_size - 1));

    if(elem_count % list->buffer_size)
    {
        elem_count += list->buffer_size - elem_count % list->buffer_size;
    }

    buffer_count = elem_count / list->buffer_size;
    list_buffer_count = list->size / list->buffer_size;
    list->size += elem_count;
    buffers = (void**)calloc(list->size, sizeof(void *));
    if(list->buffers)
    {
        memcpy(buffers, list->buffers, sizeof(void *) * list_buffer_count);
        free(list->buffers);
    }

    for(uint32_t i = 0; i < buffer_count; i++)
    {
        buffers[i + list_buffer_count] = calloc(list->buffer_size, list->elem_size);
    }

    list->buffers = buffers;
}

void *get_list_element(struct list_t *list, uint32_t index)
{
    char *buffer;
    void *element = NULL;

    if(index < list->size)
    {
        buffer = (char*)list->buffers[index / list->buffer_size];
        element = buffer + (index % list->buffer_size) * list->elem_size;
    }

    return element;
}

uint32_t add_list_element(struct list_t *list, void *element)
{
    uint32_t index = 0xffffffff;
    char *buffer;

    index = list->cursor++;
    if(index >= list->size)
    {
        /* this will add an extra buffer... */
        expand_list(list, 1);
    }

    if(element)
    {
        // printf("%d\n", index / list->buffer_size);
        buffer = (char*)list->buffers[index / list->buffer_size];
        memcpy(buffer + (index % list->buffer_size) * list->elem_size, element, list->elem_size);
    }

    return index;
}

void remove_list_element(struct list_t *list, uint32_t index)
{
    char *buffer0;
    char *buffer1;
    uint32_t last_index;

    if(index < list->cursor)
    {
        if(index < list->cursor - 1)
        {
            last_index = list->cursor - 1;
            buffer0 = (char*)list->buffers[index / list->buffer_size];
            buffer0 += list->elem_size * (index % list->buffer_size);

            buffer1 = (char*)list->buffers[last_index / list->buffer_size];
            buffer1 += list->elem_size * (last_index % list->buffer_size);

            memcpy(buffer0, buffer1, list->elem_size);
        }

        list->cursor--;
    }
}

#endif


#endif
