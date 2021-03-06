#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

struct ringbuffer_t
{
    void *buffer;
    uint32_t elem_size;
    uint32_t buffer_size;
    uint32_t free_slots;
    uint32_t next_in;
    uint32_t next_out;\
};


struct ringbuffer_t create_ringbuffer(uint32_t elem_size, uint32_t buffer_size);

void destroy_ringbuffer(struct ringbuffer_t *ringbuffer);

void reset_ringbuffer(struct ringbuffer_t *ringbuffer);

void resize_ringbuffer(struct ringbuffer_t *ringbuffer, uint32_t new_elem_count);

uint32_t add_ringbuffer_element(struct ringbuffer_t *ringbuffer, void *element);

void *peek_ringbuffer_element(struct ringbuffer_t *ringbuffer);

void *get_ringbuffer_element(struct ringbuffer_t *ringbuffer);

#ifdef DS_RINGBUFFER_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ds_mem.h"

struct ringbuffer_t create_ringbuffer(uint32_t elem_size, uint32_t buffer_size)
{
    struct ringbuffer_t ringbuffer = {};

    ringbuffer.buffer_size = buffer_size;
    ringbuffer.elem_size = elem_size;
    ringbuffer.free_slots = buffer_size;
    ringbuffer.buffer = mem_Calloc(elem_size, buffer_size);

    return ringbuffer;
}

void destroy_ringbuffer(struct ringbuffer_t *ringbuffer)
{
    if(ringbuffer)
    {
        mem_Free(ringbuffer->buffer);
        ringbuffer->buffer = NULL;
        ringbuffer->buffer_size = 0;
        ringbuffer->elem_size = 0;
    }
}

void reset_ringbuffer(struct ringbuffer_t *ringbuffer)
{
    if(ringbuffer)
    {
        ringbuffer->next_in = 0;
        ringbuffer->next_out = 0;
    }
}

void resize_ringbuffer(struct ringbuffer_t *ringbuffer, uint32_t new_elem_count)
{
    if(ringbuffer)
    {
        uint32_t elem_count = ringbuffer->buffer_size / ringbuffer->elem_size;
        if(new_elem_count > elem_count)
        {
            uint32_t new_size = ringbuffer->elem_size * new_elem_count;
            ringbuffer->buffer = mem_Realloc(ringbuffer->buffer, new_size);
            ringbuffer->free_slots += new_elem_count - elem_count;
            ringbuffer->buffer_size = new_size;
        }
    }
}

uint32_t add_ringbuffer_element(struct ringbuffer_t *ringbuffer, void *element)
{
    uint32_t index = 0xffffffff;

    if(ringbuffer->free_slots)
    {
        index = ringbuffer->next_in;

        if(element)
        {
            memcpy((char *)ringbuffer->buffer + index * ringbuffer->elem_size, element, ringbuffer->elem_size);
        }

        ringbuffer->next_in = (ringbuffer->next_in + 1) % ringbuffer->buffer_size;
        ringbuffer->free_slots--;
    }

    return index;
}

void *peek_ringbuffer_element(struct ringbuffer_t *ringbuffer)
{
    return (char *)ringbuffer->buffer + ringbuffer->next_out * ringbuffer->elem_size;
}

void *get_ringbuffer_element(struct ringbuffer_t *ringbuffer)
{
    void *element = NULL;

    if(ringbuffer->free_slots < ringbuffer->buffer_size)
    {
        element = (char *)ringbuffer->buffer + ringbuffer->next_out * ringbuffer->elem_size;
        ringbuffer->next_out = (ringbuffer->next_out + 1) % ringbuffer->buffer_size;
        ringbuffer->free_slots++;
    }

    return element;
}


#endif


#endif
