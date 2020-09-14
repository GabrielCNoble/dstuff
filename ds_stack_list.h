#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <stdint.h>

struct stack_list_t
{
    void **buffers;
    uint32_t buffer_size;
    uint32_t elem_size;
    uint32_t cursor;
    uint32_t size;
    uint32_t used;

    uint32_t *free_stack;
    uint32_t free_stack_top;
};

#ifdef __cplusplus
extern "C"
{
#endif

struct stack_list_t create_stack_list(uint32_t elem_size, uint32_t buffer_size);

void destroy_stack_list(struct stack_list_t *stack_list);

void expand_stack_list(struct stack_list_t *stack_list, uint32_t elem_count);

void *get_stack_list_element(struct stack_list_t *stack_list, uint32_t index);

uint32_t add_stack_list_element(struct stack_list_t *stack_list, void *element);

void remove_stack_list_element(struct stack_list_t *stack_list, uint32_t index);

#ifdef __cplusplus
}
#endif

#ifdef DS_STACK_LIST_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ds_mem.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct stack_list_t create_stack_list(uint32_t elem_size, uint32_t buffer_size)
{
    struct stack_list_t stack_list;
    memset(&stack_list, 0, sizeof(struct stack_list_t));

    stack_list.buffer_size = buffer_size;
    stack_list.elem_size = elem_size;
    stack_list.free_stack_top = 0xffffffff;
    stack_list.used = 0;

    expand_stack_list(&stack_list, 1);

    return stack_list;
}

void destroy_stack_list(struct stack_list_t *stack_list)
{
    uint32_t buffer_count = stack_list->size / stack_list->buffer_size;

    for(uint32_t i = 0; i < buffer_count; i++)
    {
        mem_Free(stack_list->buffers[i]);
    }

    mem_Free(stack_list->free_stack);
}

void expand_stack_list(struct stack_list_t *stack_list, uint32_t elem_count)
{
    void **buffers;
    uint32_t *free_stack;
    uint32_t buffer_count;
    uint32_t list_buffer_count;

    elem_count = (elem_count + stack_list->buffer_size - 1) & (~(stack_list->buffer_size - 1));

    if(elem_count % stack_list->buffer_size)
    {
        elem_count += stack_list->buffer_size - elem_count % stack_list->buffer_size;
    }

    buffer_count = elem_count / stack_list->buffer_size;
    list_buffer_count = stack_list->size / stack_list->buffer_size;

    stack_list->size += elem_count;

    buffers = (void**)mem_Calloc(stack_list->size, sizeof(void *));
    free_stack = (uint32_t*)mem_Calloc(stack_list->size, sizeof(uint32_t));

    if(stack_list->buffers)
    {
        memcpy(buffers, stack_list->buffers, sizeof(void *) * list_buffer_count);
        mem_Free(stack_list->buffers);
        mem_Free(stack_list->free_stack);
    }

    for(uint32_t i = 0; i < buffer_count; i++)
    {
        buffers[i + list_buffer_count] = mem_Calloc(stack_list->buffer_size, stack_list->elem_size);
    }

    stack_list->buffers = buffers;
    stack_list->free_stack = free_stack;
}

void *get_stack_list_element(struct stack_list_t *stack_list, uint32_t index)
{
    char *buffer;
    void *element = NULL;

    if(index < stack_list->size)
    {
        buffer = (char*)stack_list->buffers[index / stack_list->buffer_size];
        element = buffer + (index % stack_list->buffer_size) * stack_list->elem_size;
    }

    return element;
}

uint32_t add_stack_list_element(struct stack_list_t *stack_list, void *element)
{
    uint32_t index = 0xffffffff;
    char *buffer;

    if(stack_list->free_stack_top < 0xffffffff)
    {
        index = stack_list->free_stack[stack_list->free_stack_top];
        stack_list->free_stack_top--;
    }
    else
    {
        index = stack_list->cursor++;

        if(index >= stack_list->size)
        {
            /* this will add an extra buffer... */
            expand_stack_list(stack_list, 1);
        }
    }

    if(element && index < 0xffffffff)
    {
        buffer = (char*)stack_list->buffers[index / stack_list->buffer_size];
        memcpy(buffer + (index % stack_list->buffer_size) * stack_list->elem_size, element, stack_list->elem_size);
    }
    
    stack_list->used++;

    return index;
}

void remove_stack_list_element(struct stack_list_t *stack_list, uint32_t index)
{
    if(index < stack_list->cursor)
    {
        stack_list->free_stack_top++;
        stack_list->free_stack[stack_list->free_stack_top] = index;
        stack_list->used--;
    }
}

#ifdef __cplusplus
}
#endif

#endif

#endif
