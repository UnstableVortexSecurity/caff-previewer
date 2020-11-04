//
// Created by marcsello on 03/11/2020.
//
#include <stdlib.h>

#include "magic_memory.h"


mm_ctx magic_memory_begin(void) {
    /**
     * Creates a new MagicMemory context to be used with MagicMemory functions.
     */

    mm_ctx new_mm_ctx = (magic_memory_context_t *) malloc(sizeof(magic_memory_context_t));

    if (new_mm_ctx == NULL) {
        return NULL;
    }

    new_mm_ctx->ptr = NULL;
    new_mm_ctx->next = NULL; // This is always will be NULL in order to keep the code simple

    return new_mm_ctx;
}

void *magic_malloc(mm_ctx context, size_t size) {
    /**
     * Same as simple malloc, but the pointer is also added to the context.
     * MagicMemory keeps track of all pointers to allocated blocks in a context so they will never be lost.
     */

    magic_memory_context_t *new_ctx_link = (magic_memory_context_t *) malloc(sizeof(magic_memory_context_t));

    if (new_ctx_link == NULL) {
        return NULL;
    }

    void *new_ptr = malloc(size);

    if (new_ptr == NULL) {
        free(new_ctx_link);
        return NULL;
    }

    new_ctx_link->ptr = new_ptr;
    new_ctx_link->next = NULL;

    magic_memory_context_t *p = context;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = new_ctx_link;
    return new_ptr;
}

void magic_free(mm_ctx context, void *ptr) {
    /**
     * Same as free() but it marks the memory block as already freed in the context.
     * So the magic_cleanup() won't double-free.
     */

    magic_memory_context_t *p = context;
    while (p != NULL) {
        if (p->ptr == ptr) {
            free(p->ptr);
            p->ptr = NULL;
        }
        p = p->next;
    }

    // That link of the chain won't be freed up in order to keep the code simple and fast

}


void magic_cleanup(mm_ctx context) {
    /**
     * Free up the context, including all memory blocks in the context as well.
     * This should be called after the context (and the memory blocks in the context) no longer needed.
     */

    magic_memory_context_t *p = context;
    while (p != NULL) {

        if (p->ptr != NULL) {
            free(p->ptr); // Free up the block this chain element points to
        }
        magic_memory_context_t* p_old = p;
        p = p->next;
        free(p_old); // Free up the chain piece itself
    }


}