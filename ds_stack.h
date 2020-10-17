#ifndef DS_STACK_H
#define DS_STACK_H

#include <stdint.h>
#include "ds_mem.h"

struct ds_stack_t
{
    uint32_t top;
    uint32_t elem_size;
    uint32_t buffer_size;
    uint32_t buffer_count;
    void **buffers;
};

struct ds_stack_t ds_create_stack(uint32_t elem_size, uint32_t buffer_size);

void ds_destroy_stack(struct ds_stack_t *stack);

void *ds_pop_stack(struct ds_stack_t *stack);

void ds_push_stack(struct ds_stack_t *stack, void *elem);

void ds_expand_stack(struct ds_stack_t *stack);



#ifdef DS_STACK_IMPLEMENTATION

struct ds_stack_t ds_create_stack(uint32_t elem_size, uint32_t buffer_size)
{
    struct ds_stack_t stack = {};
    
    stack.elem_size = elem_size;
    stack.buffer_size = buffer_size;
    stack.top = 0xffffffff;
    
    ds_expand_stack(&stack);
    
    return stack;
}

void ds_destroy_stack(struct ds_stack_t *stack)
{
    if(stack)
    {
        for(uint32_t buffer_index = 0; buffer_index < stack->buffer_count; buffer_index++)
        {
            mem_Free(stack->buffers[buffer_index]);
        }
        mem_Free(stack->buffers);
        
        stack->buffers = NULL;
        stack->buffer_size = 0;
    }
}

void *ds_pop_stack(struct ds_stack_t *stack)
{
    void *elem = NULL;
    
    if(stack && stack->top != 0xffffffff)
    {
        elem = (char *)stack->buffers[stack->top / stack->buffer_size] + stack->top % stack->buffer_size;
        stack->top--;
    }
    
    return elem;
}

void ds_push_stack(struct ds_stack_t *stack, void *elem)
{
    if(stack && elem)
    {
        stack->top++;
        if(stack->top >= stack->buffer_size * stack->buffer_count)
        {
            ds_expand_stack(stack);
        }
        memcpy((char *)stack->buffers[stack->top / stack->buffer_size] + stack->top % stack->buffer_size, elem, stack->elem_size);
    }
}

void ds_expand_stack(struct ds_stack_t *stack)
{
    if(stack)
    {
        stack->buffers = mem_Realloc(stack->buffers, sizeof(void **) * (stack->buffer_count + 1));
        stack->buffers[stack->buffer_count] = mem_Calloc(1, stack->buffer_size);
        stack->buffer_count++;
    }
}

#endif


#endif // DS_STACK_H
