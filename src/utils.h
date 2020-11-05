//
// Created by marcsello on 05/11/2020.
//

#ifndef CAFF_PREVIEWER_UTILS_H
#define CAFF_PREVIEWER_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include "magic_memory.h"

#define FILE_READ_COULD_NOT_OPEN 1
#define FILE_READ_TOO_BIG 2
#define FILE_READ_COULD_NOT_ALLOCATE 3
#define FILE_READ_ERROR 4
#define FILE_READ_SUCCESS 0

bool contains(const uint8_t *data, uint64_t data_len, uint8_t what);

uint8_t read_file_to_mem(mm_ctx context, const char *filename, uint64_t max_len, uint8_t **data, uint64_t *data_len);

#endif //CAFF_PREVIEWER_UTILS_H
