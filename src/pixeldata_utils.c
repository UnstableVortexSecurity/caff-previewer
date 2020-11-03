//
// Created by marcsello on 03/11/2020.
//

#include "pixeldata_utils.h"

uint8_t flip_image(const uint8_t *source, uint8_t *destination, uint64_t data_length, uint64_t width, uint64_t height) {

    if ((width*height*3) != data_length) {
        return IMAGE_FLIP_FAIL;
    }

    for (uint64_t i = 0; i < height; i++) {
        for (uint64_t j = 0; j < width; j++) {
            destination[(i * width + j) * 3] = source[((height - i - 1) * width + j) * 3];
            destination[(i * width + j) * 3 + 1] = source[((height - i - 1) * width + j) * 3 + 1];
            destination[(i * width + j) * 3 + 2] = source[((height - i - 1) * width + j) * 3 + 2];
        }
    }

    return IMAGE_FLIP_SUCCESS;
}