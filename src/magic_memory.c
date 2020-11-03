//
// Created by marcsello on 03/11/2020.
//
#include <stdlib.h>

#include "magic_memory.h"


magic_memory_context_t *magic_memory_begin(void) {
    magic_memory_context_t *new_mm = (magic_memory_context_t *) malloc(sizeof(magic_memory_context_t));

    if (new_mm == NULL) {
        return NULL;
    }

    new_mm->ptr = NULL;
    new_mm->next = NULL;

    return new_mm;
}

void *magic_malloc(magic_memory_context_t *magic_memory, size_t size) {

    magic_memory_context_t *mm = (magic_memory_context_t *) malloc(sizeof(magic_memory_context_t));

    if (mm == NULL) {
        return NULL;
    }

    void *new_ptr = malloc(size);

    if (new_ptr == NULL) {
        free(mm);
        return NULL;
    }

    mm->ptr = new_ptr;
    mm->next = NULL;

    magic_memory_context_t *p = magic_memory;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = mm;
    return new_ptr;
}

void magic_free(magic_memory_context_t *magic_memory, void *ptr) {

    magic_memory_context_t *p = magic_memory;
    while (p != NULL) {
        if (p->ptr == ptr) {
            free(p->ptr);
            p->ptr = NULL;
        }
        p = p->next;
    }

}


void magic_cleanup(magic_memory_context_t *magic_memory) {

    magic_memory_context_t *p = magic_memory;
    while (p != NULL) {

        if (p->ptr != NULL) {
            free(p->ptr); // Free up the block this chain element points to
        }
        magic_memory_context_t* p_old = p;
        p = p->next;
        free(p_old); // Free up the chain piece itself
    }


}