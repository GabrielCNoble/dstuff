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

#ifdef __cplusplus
extern "C"
{
#endif

struct list_t create_list(uint32_t elem_size, uint32_t buffer_size);

void destroy_list(struct list_t *list);

void expand_list(struct list_t *list, uint32_t elem_count);

void *get_list_element(struct list_t *list, uint32_t index);

void get_list_element_value(struct list_t *list, uint32_t index, void *value);

uint32_t add_list_element(struct list_t *list, void *element);

void remove_list_element(struct list_t *list, uint32_t index);

uint32_t find_list_element(struct list_t *list, void *element);

void qsort_list_rec(struct list_t *list, uint32_t left, uint32_t right, int32_t (*compare)(void *a, void *b));

void qsort_list(struct list_t *list, int32_t (*compare)(void *a, void *b));

#ifdef __cplusplus
}
#endif

#ifdef DS_LIST_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ds_mem.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct list_t create_list(uint32_t elem_size, uint32_t buffer_size)
{
    struct list_t list;
    memset(&list, 0, sizeof(struct list_t));

    /* add two extra elements at the start of the first buffer, to serve
    as storage for the pivot and for the temporary element when sorting
    the list */
    list.buffer_size = buffer_size + 2;
    list.elem_size = elem_size;

    expand_list(&list, 1);
    list.buffer_size -= 2;
    list.size -= 2;

    /* the first buffer will get its pointer advanced forward by
    two elements, so the storage for the pivot stays at -2 position, and
    the temporary element stays at -1 */
    list.buffers[0] = (char *)list.buffers[0] + list.elem_size * 2;
    return list;
}

void destroy_list(struct list_t *list)
{
    if(list->buffers)
    {
        list->buffers[0] = (char *)list->buffers[0] - list->elem_size * 2;
        for(uint32_t i = 0; i < list->size / list->buffer_size; i++)
        {
            mem_Free(list->buffers[i]);
        }

        mem_Free(list->buffers);
        
        list->buffers = NULL;
        list->cursor = 0;
        list->elem_size = 0;
        list->size = 0;
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
    buffers = (void**)mem_Calloc(list->size / list->buffer_size, sizeof(void *));
    if(list->buffers)
    {
        memcpy(buffers, list->buffers, sizeof(void *) * list_buffer_count);
        mem_Free(list->buffers);
    }

    for(uint32_t i = 0; i < buffer_count; i++)
    {
        buffers[i + list_buffer_count] = mem_Calloc(list->buffer_size, list->elem_size);
    }

    list->buffers = buffers;
}

void *get_list_element(struct list_t *list, uint32_t index)
{
    char *buffer;
    void *element = NULL;

    if(index < list->cursor)
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

uint32_t find_list_element(struct list_t *list, void *element)
{
    for(uint32_t element_index = 0; element_index < list->cursor; element_index++)
    {
        if(!memcmp(element, get_list_element(list, element_index), list->elem_size))
        {
            return element_index;
        }
    }
        
    return 0xffffffff;
}

void qsort_list_rec(struct list_t *list, uint32_t left, uint32_t right, int32_t (*compare)(void *a, void *b))
{
    uint32_t cur_left = left;
    uint32_t cur_right = right;
    void *middle = (char *)list->buffers[0] - list->elem_size * 2;
    void *temp = (char *)list->buffers[0] - list->elem_size;
    void *left_elem;
    void *right_elem;
    memcpy(middle, get_list_element(list, (right + left) / 2), list->elem_size);

    while(1)
    {
        while(compare(get_list_element(list, cur_left), middle) < 0) cur_left++;
        while(compare(middle, get_list_element(list, cur_right)) < 0) cur_right--;
        if(cur_left >= cur_right) break;

        left_elem = get_list_element(list, cur_left);
        right_elem = get_list_element(list, cur_right);

        memcpy(temp, left_elem, list->elem_size);
        memcpy(left_elem, right_elem, list->elem_size);
        memcpy(right_elem, temp, list->elem_size);

        cur_left++;
        cur_right--;
    }

    if(left < cur_right) qsort_list_rec(list, left, cur_right, compare);
    if(cur_left < right) qsort_list_rec(list, cur_right + 1, right, compare);
}

void qsort_list(struct list_t *list, int32_t (*compare)(void *a, void *b))
{
    if(list->cursor)
    {
        qsort_list_rec(list, 0, list->cursor - 1, compare);
    }
}

#ifdef __cplusplus
}
#endif

#endif


#endif
