//
// Created by marcsello on 03/11/2020.
//

#ifndef CAFF_PREVIEWER_MAGIC_MEMORY_H
#define CAFF_PREVIEWER_MAGIC_MEMORY_H

/**
 * MagicMemory is a super-simple helper library to help fighting with common memory management mistakes in C.
 * In C you have to allocate and free every memory block manually. While this giving a great freedom which might be
 * highly d desirable in certain use cases, most of the time all it does is causing headaches (double free, memory leak, etc.)
 *
 * The goal of MagicMemory is to provide a safety net for developers by trying to dodge the most common issues.
 * By keeping certain conventions, MagicMemory can help writing more reliable code.
 *
 * MagicMemory keeps track of all memory blocks allocated. And provides a simple way to free up all those allocated
 * memory block at once. This way the developer does not have to manually call free avoiding both double free and
 * the abscence of a free. The storage structure for the allocations is called a context. The context must be provided
 * for each MagicMemory function call.
 *                                                                                                  -- Marcsello
 */

#include <stddef.h>

typedef struct magic_memory_context_t {
    void* next;
    void *ptr;
} magic_memory_context_t;

typedef magic_memory_context_t* mm_ctx;

mm_ctx magic_memory_begin(void);
void* magic_malloc(mm_ctx context, size_t size);

void magic_free(mm_ctx context, void* ptr);
void magic_cleanup(mm_ctx context);

#endif //CAFF_PREVIEWER_MAGIC_MEMORY_H
