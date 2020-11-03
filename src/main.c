#include <stdio.h>
#include <string.h>

#include "magic_memory.h"

int main() {
    magic_memory_t* magic_run = magic_memory_begin();

    char* str = (char*)magic_malloc(magic_run, 120);
    strcpy(str,"Test string! Hello world!!");
    printf("%s",str);
    //magic_free(magic_run, str);
    magic_cleanup(magic_run); // Free all memory used automagically

    return 0;
}
