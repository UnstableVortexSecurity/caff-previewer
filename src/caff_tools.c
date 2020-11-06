//
// Created by marcsello on 03/11/2020.
//

#include "caff_tools.h"
#include "ciff_tools.h"
#include <stdbool.h>

// Validator = Parses data and checks it's validity, but not returning the provided data.
// Parser = Parses data and checks it's validity, returns the parsed data.

uint8_t parse_caff_header(uint8_t *data, uint64_t data_len, uint64_t *num_anim) {

    if (data_len != sizeof(caff_header_t)) {
        return CAFF_PARSE_LENGTH_ERROR;
    }

    caff_header_t *header_info = (caff_header_t *) data;

    if (header_info->magic != 0x46464143) {
        return CAFF_PARSE_BAD_MAGIC;
    }

    if (header_info->header_size != 20) {
        return CAFF_PARSE_LENGTH_ERROR;
    }

    if (header_info->num_anim == 0) {
        return CAFF_PARSE_ANIMATION_COUNT_ERROR;
    }

    *num_anim = header_info->num_anim;

    return CAFF_PARSE_SUCCESS; // Sikeres parsolás == Valid formátum

}

uint8_t validate_caff_credits(uint8_t *data, uint64_t data_len) {

    if (data_len < sizeof(credits_header_t)) {
        return CAFF_PARSE_LENGTH_ERROR;
    }

    credits_header_t *header_info = (credits_header_t *) data;

    if (header_info->month > 12 || header_info->month == 0) {
        return CAFF_PARSE_BAD_DATE;
    }

    uint8_t month_lengths[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (header_info->day > month_lengths[header_info->month - 1] || header_info->day == 0) {
        return CAFF_PARSE_BAD_DATE;
    }

    if (header_info->hour > 23) {
        return CAFF_PARSE_BAD_DATE;
    }

    if (header_info->minute > 60) {
        return CAFF_PARSE_BAD_DATE;
    }

    uint64_t calculated_len = header_info->creator_len + sizeof(credits_header_t);

    if (calculated_len != data_len) {
        return CAFF_PARSE_LENGTH_ERROR;
    }

    return CAFF_PARSE_SUCCESS;
}

uint8_t validate_caff_animation(uint8_t *data, uint64_t data_len) {

    if (data_len < sizeof(animation_header_t)) {
        return CAFF_PARSE_LENGTH_ERROR;
    }

    animation_header_t *header_info = (animation_header_t *) data;


    if (header_info->duration == 0) {
        return CAFF_PARSE_NO_DURATION;
    }

    // NOTE: Ciff validation is solved in the CIFF tools
    uint8_t result = validate_ciff(data + sizeof(animation_header_t), data_len - sizeof(animation_header_t));

    if (result != CIFF_PARSE_SUCCESS) {
        return result;
    }

    return CAFF_PARSE_SUCCESS;

}


uint8_t validate_caff_file(uint8_t *data, uint64_t data_len) {

    uint8_t *p = data;

    uint64_t frame_counter = 0;
    uint64_t len_remaining = data_len;
    uint64_t expected_num_anim;
    uint64_t num_anim = 0;
    bool credits_found = false;

    while (len_remaining > 0) {

        if (len_remaining < sizeof(caff_frame_header_t)) {
            return CAFF_PARSE_LENGTH_ERROR;
        }

        caff_frame_header_t *frame_header = (caff_frame_header_t *) p;

        if (!(
                frame_header->id == CAFF_FRAME_HEADER ||
                frame_header->id == CAFF_FRAME_CREDITS ||
                frame_header->id == CAFF_FRAME_ANIMATION)) {
            return CAFF_PARSE_BAD_FRAME;
        }

        if (frame_header->length > len_remaining) {
            return CAFF_PARSE_LENGTH_ERROR;
        }

        if ((frame_counter == 0 && frame_header->id != CAFF_FRAME_HEADER) ||
            (frame_counter > 0 && frame_header->id == CAFF_FRAME_HEADER)) {
            return CAFF_PARSE_HEADER_ERROR;
        }

        uint8_t result;
        switch (frame_header->id) {
            case CAFF_FRAME_HEADER:
                result = parse_caff_header(p + sizeof(struct caff_frame_header_t),
                                           frame_header->length, &expected_num_anim);
                break;
            case CAFF_FRAME_CREDITS:
                result = validate_caff_credits(p + sizeof(struct caff_frame_header_t), frame_header->length);
                credits_found = true;
                break;
            case CAFF_FRAME_ANIMATION:
                result = validate_caff_animation(p + sizeof(struct caff_frame_header_t), frame_header->length);
                num_anim++;
                break;
        }

        if (result != CAFF_PARSE_SUCCESS) {
            return result;
        }

        frame_counter++;
        uint64_t seek_by = frame_header->length + sizeof(caff_frame_header_t);
        len_remaining -= seek_by;
        p += seek_by;
    }

    if (expected_num_anim != num_anim) {
        return CAFF_PARSE_ANIMATION_COUNT_ERROR;
    }

    if (!credits_found) {
        return CAFF_PARSE_NO_CREDITS_ERROR;
    }


    return CAFF_PARSE_SUCCESS;

}

uint8_t parse_caff_get_first_ciff(uint8_t *caff_data, uint64_t caff_data_len, uint8_t **ciff_ptr, uint64_t *ciff_len) {

    uint8_t result = validate_caff_file(caff_data, caff_data_len);

    if (result != CAFF_PARSE_SUCCESS) {
        return result;
    }

    // Seek for the first CIFF header
    uint8_t *p = caff_data;
    uint64_t caff_data_len_remaining = caff_data_len;
    while (caff_data_len_remaining > 0) {
        caff_frame_header_t *frame_header = (caff_frame_header_t *) p;

        if (frame_header->id == CAFF_FRAME_ANIMATION) {
            *ciff_ptr = p + sizeof(caff_frame_header_t) + sizeof(animation_header_t);
            *ciff_len = frame_header->length - sizeof(animation_header_t);
            return CAFF_PARSE_SUCCESS;
        }

        uint64_t seek_by = frame_header->length + sizeof(caff_frame_header_t);
        caff_data_len_remaining -= seek_by;
        p += seek_by;
    }

    return CAFF_PARSE_UNKNOWN_ERROR;
}