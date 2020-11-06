//
// Created by marcsello on 03/11/2020.
//

#include "ciff_tools.h"
#include "utils.h"

uint8_t validate_ciff(const uint8_t* data, uint64_t data_len) {

    // Read out the static part of the header (If we have at least header bytes)
    if (data_len < sizeof(ciff_static_header_t)) {
        return CIFF_PARSE_LENGTH_ERROR;
    }

    // This is just a pointer with different type information
    ciff_static_header_t* header_info = (ciff_static_header_t*)data;

    // Do some preflight checks
    // Check if the magic is valid...
    if (header_info->magic != 0x46464943) {
        return CIFF_PARSE_HEADER_BAD_MAGIC;
    }

    // Check if the fields in the header seems valid (none of the size fields larger than the whole file)
    if ((header_info->header_size > data_len) || (header_info->content_size > data_len)) {
        return CIFF_PARSE_LENGTH_ERROR;
    }

    if ((header_info->header_size+header_info->content_size) > data_len) {
        return CIFF_PARSE_LENGTH_ERROR;
    }

    if (header_info->width == 0 || header_info->height == 0) {
        return CIFF_PARSE_HEADER_DIMENSIONS_INCORRECT;
    }

    // Pre-Calculate some variables and check against those as well
    uint64_t calculated_pixeldata_length_by_header = header_info->width * header_info->height * 3;
    if (data_len < calculated_pixeldata_length_by_header) {
        // The number of pixels defined in the header is larger than the whole file
        return CIFF_PARSE_LENGTH_ERROR;
    }

    if (calculated_pixeldata_length_by_header != header_info->content_size) {
        return CIFF_PARSE_LENGTH_ERROR;
    }

    uint64_t calculated_total_length_by_header = calculated_pixeldata_length_by_header + header_info->header_size;
    if (calculated_total_length_by_header != data_len) {
        // The header + pixel data is not equals the length of the file
        return CIFF_PARSE_LENGTH_ERROR;
    }

    // Do some other checks to validate the header

    // According the specifications, the header must contain a newline character after the static fields
    if (!contains(data+sizeof(ciff_static_header_t),header_info->header_size-sizeof(ciff_static_header_t),'\n')) {
        return CIFF_PARSE_UNKNOWN_ERROR;
    }

    // According to specifications, the last byte of the header must be a \0
    if (data[header_info->header_size-1] != 0) {
        return CIFF_PARSE_UNKNOWN_ERROR;
    }

    return CIFF_PARSE_SUCCESS;
}

uint8_t parse_ciff_get_pixel_data(const uint8_t* data, uint64_t data_len, uint8_t** pixel_data_ptr, uint64_t *pixel_data_len, uint64_t* width, uint64_t* height) {
    /**
     * Ez csak visszad egy pointert, hogy honnan kezdődik a pixel data, miután validálta a képet
     */

    uint8_t result = validate_ciff(data, data_len);
    if (result != CIFF_PARSE_SUCCESS) {
        return result;
    }

    ciff_static_header_t* header_info = (ciff_static_header_t*)data; // This is just a pointer with different type information

    // Set return data
    *width = header_info->width;
    *height = header_info->height;
    *pixel_data_ptr = ((uint8_t*)data) + (header_info->header_size);
    *pixel_data_len = header_info->content_size;
    return CIFF_PARSE_SUCCESS;

}