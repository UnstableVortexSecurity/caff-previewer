//
// Created by marcsello on 03/11/2020.
//

#ifndef CAFF_PREVIEWER_PIXELDATA_UTILS_H
#define CAFF_PREVIEWER_PIXELDATA_UTILS_H

#include <stdint.h>

#define IMAGE_FLIP_SUCCESS 0
#define IMAGE_FLIP_FAIL 0

uint8_t flip_image(const uint8_t* source, uint8_t* destination, uint64_t data_length, uint64_t width, uint64_t height);

#endif //CAFF_PREVIEWER_PIXELDATA_UTILS_H
