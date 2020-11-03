#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "libtarga.h"
#include "magic_memory.h"

int main() {
    magic_memory_context_t *magic_run = magic_memory_begin();

    uint8_t *picture = (uint8_t *) magic_malloc(magic_run, 50 * 50 * 3);
    for (uint16_t i = 0; i < (50*50);i++) {
        picture[(i*3)] = 0;
        picture[(i*3)+1] = (i % 100)*25;
        picture[(i*3)+2] = 255;
    }

    if (!tga_write_raw("test.tga", 50, 50, picture, TGA_TRUECOLOR_24)) {
        printf("%s", tga_error_string(tga_get_last_error()));
    }

    magic_cleanup(magic_run); // Free all memory used automagically

    return 0;
}
