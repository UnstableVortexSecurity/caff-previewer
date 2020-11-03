//
// Created by marcsello on 03/11/2020.
//

#ifndef CAFF_PREVIEWER_MAGIC_MEMORY_H
#define CAFF_PREVIEWER_MAGIC_MEMORY_H

typedef struct magic_memory_context_t {
    void* next;
    void *ptr;
} magic_memory_context_t;

magic_memory_context_t* magic_memory_begin(void);
void* magic_malloc(magic_memory_context_t* magic_memory, size_t size);

void magic_free(magic_memory_context_t* magic_memory, void* ptr);
void magic_cleanup(magic_memory_context_t* magic_memory);

#endif //CAFF_PREVIEWER_MAGIC_MEMORY_H
