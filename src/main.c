#include <stdio.h>
#include <stdint.h>

#include "libtarga.h"
#include "magic_memory.h"
#include "pixeldata_utils.h"
#include "ciff_tools.h"
#include "caff_tools.h"
#include "utils.h"

#define LIBTARGA_ERROR_PREFIX 0x40
#define USAGE_ERROR_PREFIX 0x50
#define USAGE_ERROR_WRONG_PARAMETERS (USAGE_ERROR_PREFIX + 0x01)

#define OTHER_ERROR_PREFIX 0x60
#define OTHER_FLIP_COULD_NOT_ALLOCATE (OTHER_ERROR_PREFIX + 0x01)

// Mivel az egyszerűség kedvéért az egész fájlt memóriába olvassuk, jobb ha limitáljuk a méretét
#define CAFF_MAX_SIZE 536870912 // 512MB


uint8_t perform_extraction(mm_ctx context, const char *infile, const char *outfile) {


    // Read the entire CAFF data to memory
    uint8_t *caff_data;
    uint64_t caff_data_len;
    uint8_t result = read_file_to_mem(context, infile, CAFF_MAX_SIZE, &caff_data, &caff_data_len);

    if (result != FILE_READ_SUCCESS) {
        return result;
    }

    // Seek for the first CIFF
    uint8_t *ciff_data;
    uint64_t ciff_data_len;

    result = parse_caff_get_first_ciff(caff_data, caff_data_len, &ciff_data, &ciff_data_len);

    if (result != CAFF_PARSE_SUCCESS) {
        return result;
    }

    // Extract pixel data from the CIFF
    uint64_t width;
    uint64_t height;
    uint8_t *pixel_data;
    uint64_t pixel_data_len;

    result = parse_ciff_get_pixel_data(ciff_data, ciff_data_len, &pixel_data, &pixel_data_len, &width, &height);

    if (result != CIFF_PARSE_SUCCESS) {
        return result;
    }

    // Flip the image as required by libtarga

    uint8_t *flipped_pixel_data = (uint8_t *) magic_malloc(context, pixel_data_len);

    if (flipped_pixel_data == NULL) {
        return OTHER_FLIP_COULD_NOT_ALLOCATE;
    }

    result = flip_image(pixel_data, flipped_pixel_data, pixel_data_len, width, height);

    if (result != IMAGE_FLIP_SUCCESS) {
        return result;
    }

    // Write out the image to a TGA file
    if (!tga_write_raw(outfile, width, height, flipped_pixel_data, TGA_TRUECOLOR_24)) {
        return LIBTARGA_ERROR_PREFIX + tga_get_last_error(); // TGA errors range from 1 to 11
    }

    // And we are done! MagicMemory will take care of cleaning up
    return 0;
}

int main(int argc, char *argv[]) {

    // Check arguments
    if (argc != 3) {
        printf("Usage: caff_previewer [SOURCE FILE] [DESTINATION FILE]\n");
        return USAGE_ERROR_WRONG_PARAMETERS;
    }

    // Open a magic memory context
    mm_ctx context = magic_memory_begin();

    // Run the program
    uint8_t result = perform_extraction(context, argv[1], argv[2]);

    // Free all memory used automagically
    magic_cleanup(context); // In C11 we could have used atexit()
    return result;

}
