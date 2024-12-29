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

#endif