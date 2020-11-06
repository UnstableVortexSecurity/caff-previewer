//
// Created by marcsello on 05/11/2020.
//

#ifndef CAFF_PREVIEWER_UTILS_H
#define CAFF_PREVIEWER_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#include "magic_memory.h"

#define FILE_READ_ERROR_PREFIX          0x00
#define FILE_READ_COULD_NOT_OPEN        (FILE_READ_ERROR_PREFIX + 0x01)
#define FILE_READ_TOO_BIG               (FILE_READ_ERROR_PREFIX + 0x02)
#define FILE_READ_COULD_NOT_ALLOCATE    (FILE_READ_ERROR_PREFIX + 0x03)
#define FILE_READ_ERROR                 (FILE_READ_ERROR_PREFIX + 0x04)

#define FILE_READ_SUCCESS               0x00

bool contains(const uint8_t *data, uint64_t data_len, uint8_t what);

uint8_t read_file_to_mem(mm_ctx context, const char *filename, uint64_t max_len, uint8_t **data, uint64_t *data_len);

#endif //CAFF_PREVIEWER_UTILS_H
