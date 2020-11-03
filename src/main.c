#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "libtarga.h"
#include "magic_memory.h"
#include "pixeldata_utils.h"
#include "ciff_tools.h"

int main() {
    magic_memory_context_t *context = magic_memory_begin();


    FILE *fp = fopen("/home/marcsello/Documents/etyetem/szamitobiztonsag/caff/1.ciff", "rb");

    fseek(fp, 0L, SEEK_END);
    uint64_t fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    uint8_t *ciff_file = (uint8_t *) magic_malloc(context, fsize);
    fread(ciff_file, 1, fsize, fp);
    fclose(fp);

    uint64_t width;
    uint64_t height;
    uint64_t pixel_data_starts;

    uint8_t result = parse_ciff_from_mem(ciff_file, fsize, &width, &height, &pixel_data_starts);

    uint64_t pixel_data_size = width * height * 3;

    if (result == CIFF_PARSE_SUCCESS) {

        uint8_t *flipped_image = (uint8_t *) magic_malloc(context, fsize - pixel_data_starts);
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
