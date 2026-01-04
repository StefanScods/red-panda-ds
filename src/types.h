#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

#define INVALID_MEM_32BIT 0xFFFFFFFF
#define INVALID_MEM_16BIT 0xFFFF
#define INVALID_MEM_8BIT 0xFF

typedef uint64_t cycles;

/**
 * @brief return type of bus reads / writes
 *
 * @param data The value of data read / written
 * @param numCycles The number of cycles this action took
 * @param uint32_t The size of the data in bits (32, 16 or 8)
 */
struct busPayload {
    uint32_t data;
    cycles numCycles;
    uint32_t size;
};

struct u32AndBool {
    uint32_t data_u32;
    bool data_bool;
};
struct u32WithCarryAndOverflow {
    uint32_t data_u32;
    bool carry;
    bool overflow;
};

#endif