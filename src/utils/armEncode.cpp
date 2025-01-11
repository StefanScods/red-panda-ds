/**
 * @file armEncode.cpp
 * @brief Covert written instuctions into their encoded binary equivelent.
 * WIP!!!!
 */
#include "armEncode.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

uint32_t armEncodeASM(std::string instruction) {
    LogError("Unable to encode " << instruction << "!");
    return 0;
}
