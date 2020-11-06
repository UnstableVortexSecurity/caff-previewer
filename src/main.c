#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "libtarga.h"
#include "magic_memory.h"
#include "pixeldata_utils.h"
#include "ciff_tools.h"
#include "caff_tools.h"
#include "utils.h"

int main() {
    magic_memory_context_t *context = magic_memory_begin();

    uint8_t *caff_file;
    uint64_t caff_size;
    uint8_t result = read_file_to_mem(context, "/home/marcsello/Documents/etyetem/szamitobiztonsag/caff/2.caff",
                                      67108864, &caff_file, &caff_size);

    if (result != FILE_READ_SUCCESS) {
        printf("File read failure: %d", result);
        return 1;
    }

    uint8_t* ciff_ptr;
    uint64_t ciff_size;

    result = parse_caff_get_first_ciff(caff_file,caff_size,&ciff_ptr, &ciff_size);

    if (result != CAFF_PARSE_SUCCESS) {
        printf("CAFF Parse failure: %d", result);
        return 1;
    }

    uint64_t width;
    uint64_t height;
    uint64_t pixel_data_starts;

    result = get_pixel_data_from_ciff(ciff_ptr, ciff_size, &width, &height, &pixel_data_starts);

    uint64_t pixel_data_size = width * height * 3;

    if (result == CIFF_PARSE_SUCCESS) {

        uint8_t *flipped_image = (uint8_t *) magic_malloc(context, caff_size - pixel_data_starts);
        if (flip_image(ciff_ptr + pixel_data_starts, flipped_image, pixel_data_size, width, height) !=
            IMAGE_FLIP_SUCCESS) {
            printf("Literally failed to flip the image");
        } else {
            if (!tga_write_raw("test.tga", width, height, flipped_image, TGA_TRUECOLOR_24)) {
                printf("%s", tga_error_string(tga_get_last_error()));
            }
        }


    } else {
        printf("Failed to get pixel data: %d", result);
    }


    magic_cleanup(context); // Free all memory used automagically

    return 0;
}
