//
// Created by marcsello on 03/11/2020.
//

#ifndef CAFF_PREVIEWER_CAFF_TOOLS_H
#define CAFF_PREVIEWER_CAFF_TOOLS_H

#include <stdint.h>

#define CAFF_PARSE_SUCCESS 0
#define CAFF_PARSE_LENGTH_ERROR 1
#define CAFF_PARSE_BAD_FRAME 2
#define CAFF_PARSE_HEADER_ERROR 3
#define CAFF_PARSE_ANIMATION_COUNT_ERROR 4
#define CAFF_PARSE_NO_CREDITS_ERROR 5
#define CAFF_PARSE_BAD_MAGIC 6
#define CAFF_PARSE_BAD_DATE 7
#define CAFF_PARSE_NO_DURATION 8

#define CAFF_FRAME_HEADER 0x1
#define CAFF_FRAME_CREDITS 0x2
#define CAFF_FRAME_ANIMATION 0x3



typedef struct __attribute__ ((packed)) caff_frame_header_t {
    uint8_t id;
    uint64_t length;
} caff_frame_header_t;

typedef struct __attribute__ ((packed)) caff_header_t { // Note: CAFF Header lives inside a frame
    uint32_t magic; //Should be to 0x46464143 (note the reversed order)
    uint64_t header_size; // Constant 20???
    uint64_t num_anim;
} caff_header_t;

typedef struct __attribute__ ((packed)) credits_header_t { // Note: CAFF Header lives inside a frame
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t hour;
   uint8_t minute;
   uint64_t creator_len;
} credits_header_t;


typedef struct __attribute__ ((packed)) animation_header_t {
    uint64_t duration;
} animation_header_t;


uint8_t validate_caff_file(uint8_t* data, uint64_t data_len);
uint8_t parse_caff_get_first_ciff(uint8_t* caff_data, uint64_t caff_data_len, uint8_t **ciff_ptr, uint64_t* ciff_len);


#endif //CAFF_PREVIEWER_CAFF_TOOLS_H

