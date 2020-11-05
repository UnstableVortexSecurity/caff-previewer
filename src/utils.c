//
// Created by marcsello on 05/11/2020.
//

#include "utils.h"

bool contains(const uint8_t* data, uint64_t data_len, uint8_t what) {
    for (uint64_t i = 0; i < data_len; i++) {
        if (data[i] == what) {
            return true;
        }
    }
    return false;
}
