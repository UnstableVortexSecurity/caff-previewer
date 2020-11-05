#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "libtarga.h"
#include "magic_memory.h"
#include "pixeldata_utils.h"
#include "ciff_tools.h"
#include "utils.h"

int main() {
    magic_memory_context_t *context = magic_memory_begin();

    uint8_t *ciff_file;
    uint64_t ciff_size;
    uint8_t result = read_file_to_mem(context, "/home/marcsello/Documents/etyetem/szamitobiztonsag/caff/1.ciff",
                                      67108864, &ciff_file, &ciff_size);

    if (result != FILE_READ_SUCCESS) {
        printf("File read failure: %d", result);
        return 1;
    }

    uint64_t width;
    uint64_t height;
    uint64_t pixel_data_starts;

    result = get_pixel_data_from_ciff(ciff_file, ciff_size, &width, &height, &pixel_data_starts);

    uint64_t pixel_data_size = width * height * 3;

    if (result == CIFF_PARSE_SUCCESS) {

        uint8_t *flipped_image = (uint8_t *) magic_malloc(context, ciff_size - pixel_data_starts);
        if (flip_image(ciff_file + pixel_data_starts, flipped_image, pixel_data_size, width, height) !=
            IMAGE_FLIP_SUCCESS) {
            printf("Literally failed to flip the image");
        } else {
            if (!tga_write_raw("test.tga", width, height, flipped_image, TGA_TRUECOLOR_24)) {
                printf("%s", tga_error_string(tga_get_last_error()));
            }
        }


    } else {
        printf("%d", result);
    }


    magic_cleanup(context); // Free all memory used automagically

    return 0;
}
