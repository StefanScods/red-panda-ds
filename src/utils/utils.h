#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

/**
 * @brief A helper function to perform a subtraction while removing potential underflows.
 *
 *
 * @tparam T An unsigned type
 * @returns `op1` - `op2` if `op1` >= `op2` else 0
 */
template <typename T>
inline T underflowSafeSubtract(T op1, T op2) {
    return (op1 >= op2) ? (op1 - op2) : 0;
}

/**
 * @brief Convert a value into a hexadecimal string .
 *
 *
 * @tparam T An unsigned type
 *
 * @param val The number value to convert
 * @param size The size of the string -> how many hexidecimal places after the 0x.
 * Defaults to 8 -> 0x00000000
 *
 * @returns `std::string`
 */
template <typename T>
std::string hexString(T val, int size = 8) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(size)  // Force 0 padding
           << std::hex << val;
    return stream.str();
}

/**
 * @brief Read a bit in the passed data.
 *
 * @param data The data to read bit from.
 * @param pos The index of the bit to read (0 indexed).
 */
inline bool readBit(uint32_t data, uint16_t pos) {
    return (data >> pos) & 0b1;
}
/**
 * @brief Write a bit to the passed data.
 *
 * @param data The data to write to.
 * @param value The value to write.
 * @param pos The index of the bit to write to (0 indexed).
 */
inline void writeBit(uint32_t &data, bool value, uint16_t pos) {
    data = ((data) & ~(0b1 << pos)) | (((uint32_t)value) << pos);
}
/**
 * @brief Read a section of bits from data.
 *
 * @param data The data to read bits from.
 * @param rangeStart The first bit to begin reading from (inclusive).
 * @param rangeEnd The last bit to end reading from (inclusive).
 */
inline uint32_t readBits(uint32_t data, uint16_t rangeStart, uint16_t rangeEnd) {
    uint8_t size = rangeEnd - rangeStart + 1;
    return (data >> rangeStart) & (uint32_t)((uint64_t)(1 << size) - 1);
}
/**
 * @brief Write a section of bits to data.
 *
 * @param data The data to write to.
 * @param value The value to write.
 * @param rangeStart The first bit to begin writing to (inclusive).
 * @param rangeEnd The last bit to end reading writing to (inclusive).
 */
inline void writeBits(uint32_t &data, uint32_t value, uint16_t rangeStart, uint16_t rangeEnd) {
    uint8_t size = rangeEnd - rangeStart + 1;
    data =
        ((data) & ~(((uint32_t)((uint64_t)(1 << size) - 1)) << rangeStart)) | (value << rangeStart);
}

#endif