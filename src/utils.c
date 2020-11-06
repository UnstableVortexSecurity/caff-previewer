//
// Created by marcsello on 05/11/2020.
//

#include <stdio.h>

#include "utils.h"


bool contains(const uint8_t *data, uint64_t data_len, uint8_t what) {
    for (uint64_t i = 0; i < data_len; i++) {
        if (data[i] == what) {
            return true;
        }
    }
    return false;
}

uint8_t read_file_to_mem(mm_ctx context, const char *filename, uint64_t max_len, uint8_t **data, uint64_t *data_len) {
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
        return FILE_READ_COULD_NOT_OPEN;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) {
        fclose(fp);
        return FILE_SEEK_ERROR;
    }

    uint64_t fsize = ftell(fp);

    if (fsize > max_len) {
        fclose(fp);
        return FILE_READ_TOO_BIG;
    }

    if (fseek(fp, 0L, SEEK_SET) != 0) {
        fclose(fp);
        return FILE_SEEK_ERROR;
    }

    uint8_t *contents = (uint8_t *) magic_malloc(context, fsize);

    if (contents == NULL) {
        fclose(fp);
        return FILE_READ_COULD_NOT_ALLOCATE;
    }

    uint64_t bytes_read = fread(contents, 1, fsize, fp);

    if (bytes_read != fsize) {
        fclose(fp);
        return FILE_READ_ERROR;
    }

    *data = contents;
    *data_len = fsize;

    fclose(fp);
    return FILE_READ_SUCCESS;
}