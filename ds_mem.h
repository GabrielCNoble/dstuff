#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define MEM_GUARD_POINTERS 24

struct mem_header_t
{
    void *guard[MEM_GUARD_POINTERS];
    size_t size;
    uint32_t freed;
    uint32_t line;
    char file[256];
    struct mem_header_t *next;
    struct mem_header_t *prev;
};

struct mem_tail_t
{
    void *guard[MEM_GUARD_POINTERS];
};

//#define INSTRUMENT_MEMORY
//#define CHECK_DOUBLE_FREE



struct mem_header_t *mem_GetAllocHeader(void *memory);

struct mem_tail_t *mem_GetAllocTail(void *memory);

void *mem_InitHeaderAndTail(void *memory, uint32_t size, uint32_t line, char *file);

void mem_CheckGuardImp(void *memory);

void mem_CheckGuardsImp();

void *mem_MallocImp(size_t size, uint32_t line, char *file);

void *mem_CallocImp(size_t num, size_t size, uint32_t line, char *file);

void mem_FreeImp(void *memory, uint32_t line, char *file);


#ifdef INSTRUMENT_MEMORY

#define mem_CheckGuard(memory) mem_CheckGuardImp(memory)

#define mem_CheckGuards() mem_CheckGuardsImp()

#define mem_Malloc(size) mem_MallocImp(size, __LINE__, __FILE__)

#define mem_Calloc(num, size) mem_CallocImp(num, size, __LINE__, __FILE__)

#define mem_Free(memory) mem_FreeImp(memory, __LINE__, __FILE__)

#else

#define mem_CheckGuard(memory)

#define mem_CheckGuards()

#define mem_Malloc(size) malloc(size)

#define mem_Calloc(num, size) calloc(num, size)

#define mem_Free(memory) free(memory)

#endif

#ifdef DS_MEMORY_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct mem_header_t *mem_headers = NULL;
struct mem_header_t *mem_last_header = NULL;

struct mem_header_t *mem_GetAllocHeader(void *memory)
{
    return (struct mem_header_t *)((char *)memory - sizeof(struct mem_header_t));
}

struct mem_tail_t *mem_GetAllocTail(void *memory)
{
    struct mem_header_t *header;
    header = mem_GetAllocHeader(memory);
    return (struct mem_tail_t *)((char *)memory + header->size);
}

void *mem_InitHeaderAndTail(void *memory, uint32_t size, uint32_t line, char *file)
{
    struct mem_header_t *header;
    struct mem_tail_t *tail;

    header = memory;
    header->size = size;
    header->line = line;
    strcpy(header->file, file);

    for(uint32_t i = 0; i < MEM_GUARD_POINTERS; i++)
    {
        header->guard[i] = memory;
    }

    memory = (char *)memory + sizeof(struct mem_header_t);

    tail = (struct mem_tail_t *)((char *)memory + size);

    for(uint32_t i = 0; i < MEM_GUARD_POINTERS; i++)
    {
        tail->guard[i] = header->guard[i];
    }

    if(!mem_headers)
    {
        mem_headers = header;
    }
    else
    {
        mem_last_header->next = header;
        header->prev = mem_last_header;
    }
    mem_last_header = header;

    return memory;
}

void mem_CheckGuardImp(void *memory)
{
    struct mem_header_t *header;
    struct mem_tail_t *tail;

    header = mem_GetAllocHeader(memory);
    tail = mem_GetAllocTail(memory);

    for(uint32_t i = 0; i < MEM_GUARD_POINTERS; i++)
    {
        if(header->guard[i] != header)
        {
            printf("mem_CheckGuard: allocation line %d, file %s has corrupt header guard bytes!\n", header->line, header->file);
            break;
        }
    }

    for(uint32_t i = 0; i < MEM_GUARD_POINTERS; i++)
    {
        if(tail->guard[i] != header)
        {
            printf("mem_CheckGuard: allocation at line %d, file %s has corrupt tail guard bytes!\n", header->line, header->file);
            break;
        }
    }
}

void mem_CheckGuardsImp()
{
    struct mem_header_t *header;
    header = mem_headers;
    while(header)
    {
        mem_CheckGuard(header + 1);
        header = header->next;
    }
}

void *mem_MallocImp(size_t size, uint32_t line, char *file)
{
    void *memory;
    memory = malloc(sizeof(struct mem_header_t) + sizeof(struct mem_tail_t) + size);
    return mem_InitHeaderAndTail(memory, size, line, file);
}

void *mem_CallocImp(size_t num, size_t size, uint32_t line, char *file)
{
    void *memory;
    size_t total_size = sizeof(struct mem_header_t) + sizeof(struct mem_tail_t) + size * num;
    memory = calloc(1, total_size);
    return mem_InitHeaderAndTail(memory, num * size, line, file);
}

void mem_FreeImp(void *memory, uint32_t line, char *file)
{
    struct mem_header_t *header = mem_GetAllocHeader(memory);
    printf("freeing allocation from (line %d, file %s) at (line %d, file %s)\n", header->line, header->file, line, file);
    mem_CheckGuardImp(memory);

    #ifdef CHECK_DOUBLE_FREE

    if(header->freed)
    {
        printf("double free of allocation from (line %d, file %s) at (line %d, file %s)\n", header->line, header->file, line, file);
        return;
    }
    else
    {
        header->freed = 1;
    }

    #else

    if(header->prev)
    {
        header->prev->next = header->next;
    }
    else
    {
        mem_headers = header->next;
    }

    if(header->next)
    {
        header->next->prev = header->prev;
    }
    else
    {
        mem_last_header = header->prev;
    }
    free(header);

    #endif
}

#endif

#endif

