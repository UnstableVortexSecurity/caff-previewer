//
// Created by marcsello on 03/11/2020.
//

#ifndef CAFF_PREVIEWER_CIFF_TOOLS_H
#define CAFF_PREVIEWER_CIFF_TOOLS_H

#include <stdint.h>

#define CIFF_PARSE_UNKNOWN_ERROR 1
#define CIFF_PARSE_HEADER_LENGTHS_INCORRECT 2
#define CIFF_PARSE_HEADER_DIMENSIONS_INCORRECT 3
#define CIFF_PARSE_HEADER_TOO_SHORT 4
#define CIFF_PARSE_HEADER_BAD_MAGIC 5
#define CIFF_PARSE_SUCCESS 0

typedef struct __attribute__ ((packed)) ciff_static_header_t {
    uint32_t magic; // should be equal to 0x46464943 (Because x86 is big-endian, so essentially this is in reverse)
    uint64_t header_size;
    uint64_t content_size;
    uint64_t width;
    uint64_t height;
} ciff_static_header_t;

uint8_t validate_ciff(const uint8_t* data, uint64_t data_len);
uint8_t get_pixel_data_from_ciff(const uint8_t* data, uint64_t data_len, uint64_t* width, uint64_t* height, uint64_t* pixel_data_starts);

#endif //CAFF_PREVIEWER_CIFF_TOOLS_H
