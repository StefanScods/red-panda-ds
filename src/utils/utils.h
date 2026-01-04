#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"
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
inline std::string hexString(T val, int size = 8) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(size)  // Force 0 padding
           << std::hex << val;
    return stream.str();
}

/**
 * @brief Read a bit in the passed data.
 *
 * @tparam T An unsigned type
 *
 * @param data The data to read bit from.
 * @param pos The index of the bit to read (0 indexed).
 */
template <typename T = uint32_t>
inline bool readBit(T data, uint8_t pos) {
    return (data >> pos) & 0b1;
}
/**
 * @brief Write a bit to the passed data.
 *
 * @tparam T An unsigned type
 *
 * @param data The data to write to.
 * @param value The value to write.
 * @param pos The index of the bit to write to (0 indexed).
 */
template <typename T = uint32_t>
inline void writeBit(T& data, bool value, uint8_t pos) {
    data = ((data) & ~(0b1 << pos)) | (((uint32_t)value) << pos);
}
/**
 * @brief Read a section of bits from data.
 *
 * @tparam T An unsigned type
 *
 * @param data The data to read bits from.
 * @param rangeStart The first bit to begin reading from (inclusive).
 * @param rangeEnd The last bit to end reading from (inclusive).
 */
template <typename T = uint32_t>
inline uint32_t readBits(T data, uint8_t rangeStart, uint8_t rangeEnd) {
    uint8_t size = rangeEnd - rangeStart + 1;
    return (data >> rangeStart) & (uint32_t)((uint64_t)(1 << size) - 1);
}
/**
 * @brief Write a section of bits to data.
 *
 * @tparam T An unsigned type
 *
 * @param data The data to write to.
 * @param value The value to write.
 * @param rangeStart The first bit to begin writing to (inclusive).
 * @param rangeEnd The last bit to end reading writing to (inclusive).
 */
template <typename T = uint32_t>
inline void writeBits(T& data, T value, uint8_t rangeStart, uint8_t rangeEnd) {
    uint8_t size = rangeEnd - rangeStart + 1;
    data =
        ((data) & ~(((uint32_t)((uint64_t)(1 << size) - 1)) << rangeStart)) | (value << rangeStart);
}

/**
 * @brief Calculate the ROR (Rotate Right) of a value.
 * Note: Function does not safety check that shift <= 31.
 *
 * @param value Value to rotate.
 * @param shift Number of bits to rotate by 0-31.
 * @return u32AndBool.data_u32 - The rotated value.
 *
 * u32AndBool.data_bool - Carry out flag.
 */
inline u32AndBool ROR(uint32_t value, uint8_t shift, uint32_t bitWidth = 32) {
    uint32_t rotatedValue = (value >> shift) | (value << (bitWidth - shift));
    bool carryOut = (value >> (shift - 1)) & 0b1;
    return {rotatedValue, carryOut};
}

/**
 * @brief Calculate the ROL (Rotate Left) of a value.
 * Note: Function does not safety check that shift <= 31.
 *
 * @param value Value to rotate.
 * @param shift Number of bits to rotate by 0-31.
 * @param bitWidth The bit width of the value.
 * @return u32AndBool.data_u32 - The rotated value.
 *
 * u32AndBool.data_bool - Carry out flag.
 */
inline u32AndBool ROL(uint32_t value, uint8_t shift, uint32_t bitWidth = 32) {
    uint32_t rotatedValue = (value << shift) | (value >> (bitWidth - shift));
    bool carryOut = (value >> (bitWidth - shift)) & 0b1;
    return {rotatedValue, carryOut};
}

/**
 * @brief Calculate the ASR (Arithmetic shift right) of a value.
 * Note: Function does not safety check that shift <= 31.
 *
 * @param value Value to rotate.
 * @param shift Number of bits to shift by 0-31.
 * @param bitWidth The bit width of the value.
 * @return u32AndBool.data_u32 - The shifted value.
 *
 * u32AndBool.data_bool - Carry out flag.
 */
inline u32AndBool ASR(uint32_t value, uint8_t shift, uint32_t bitWidth = 32) {
    bool signExtend = readBit(value, bitWidth - 1);
    bool carryOut = readBit(value, shift - 1);
    value = value >> shift;
    if (signExtend) {
        value = ((0 - 1) << bitWidth - shift) | value;
    }
    return {value, carryOut};
}

// Types of ARM shifts
#define ARM_SHIFT_LSL 0b00
#define ARM_SHIFT_LSR 0b01
#define ARM_SHIFT_ASR 0b10
#define ARM_SHIFT_ROR 0b11

/**
 * @brief Implements ARM shift() function.
 * https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Instruction-Details/Shifts-applied-to-a-register/Pseudocode-details-of-instruction-specified-shifts-and-rotates?lang=en
 *
 * @param type Type of shift to apply.
 * @param value Value to shift.
 * @param shift The amount to shift by.
 * @param carryIn The current carry in value.
 * @param bitWidth The bit width of the value.
 * @return u32AndBool.data_u32 - The shifted value.
 *
 * u32AndBool.data_bool - Carry out flag.
 */
inline u32AndBool ARMShift(uint8_t type, uint32_t value, uint8_t shift, bool carryIn,
                           uint32_t bitWidth = 32) {
    if (!shift) return {value, carryIn};
    switch (type) {
        // Logical shift left.
        case ARM_SHIFT_LSL: {
            bool carryOut = readBit(value, bitWidth - shift);
            value = value << shift;
            return {value, carryOut};
        }
        // Logical shift right.
        case ARM_SHIFT_LSR: {
            bool carryOut = readBit(value, shift - 1);
            value = value >> shift;
            return {value, carryOut};
        }
        // Arithmetic shift right.
        case ARM_SHIFT_ASR: {
            return ASR(value, shift, bitWidth);
        }
        // Rotate Right.
        case ARM_SHIFT_ROR: {
            return ROR(value, shift, bitWidth);
        }
    }
    // Unknown shift type
    return {value, carryIn};
}

/**
 * @brief Implements ARMExpandImm_C - used to extract an 32 bit immediate from a 12 bit encoding.
 * https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Modified-immediate-constants-in-ARM-instructions
 *
 * @param imm12 Raw binary encoding of the encoding.
 * @param carryIn Carry in flag.
 *
 * @return u32AndBool.data_u32 - The decoded immediate value.
 *
 * u32AndBool.data_bool - Carry out flag.
 */
inline u32AndBool ARMExpandImm_C(uint32_t imm12, bool carryIn) {
    uint32_t amount = readBits(imm12, 8, 11) * 2;
    uint32_t imm8 = readBits(imm12, 0, 7);
    return amount == 0 ? u32AndBool{imm8, carryIn} : ROR(imm8, amount);
}
/**
 * @brief Implements AddWithCarry - used to add two numbers with a carry value.
 * https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Application-Level-Programmers--Model/ARM-core-data-types-and-arithmetic/Integer-arithmetic?lang=en
 *
 * @param opperand1 First operand
 * @param opperand2 Second operand
 * @param carryIn
 * @return u32WithCarryAndOverflow
 */
inline u32WithCarryAndOverflow ARMAddWithCarry(uint32_t opperand1, uint32_t opperand2,
                                               bool carryIn) {
    uint64_t unsignedSum = uint64_t(opperand1) + uint64_t(opperand2) + uint64_t(carryIn);
    int64_t signedSum =
        int64_t(int32_t(opperand1)) + int64_t(int32_t(opperand2)) + uint64_t(carryIn);
    uint32_t result = uint32_t(unsignedSum & 0xFFFFFFFF);
    bool carryOut = uint64_t(result) != unsignedSum;
    bool overflow = int64_t(int32_t(result)) != signedSum;
    return {result, carryOut, overflow};
}

/**
 * @brief Splits a string at tokens matched via a regular expression.
 * Use std::regex("\\s+") to match and split on white space.
 *
 * @param input String to split
 * @param splitOnRegex Regex to model where to split
 * @return std::vector<std::string>
 */
inline std::vector<std::string> splitString(const std::string& input,
                                            const std::regex& splitOnRegex) {
    std::vector<std::string> splitTokens;
    std::sregex_token_iterator it(input.begin(), input.end(), splitOnRegex, -1);
    std::sregex_token_iterator end;
    for (; it != end; ++it) {
        splitTokens.push_back(it->str());
    }
    return splitTokens;
}

/**
 * @brief Convert a string to lower case. Modifies the input param directly
 *
 * @param input String to modify
 */
inline void toLowerCase(std::string& input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

/**
 * @brief Convert a string to upper case. Modifies the input param directly
 *
 * @param input String to modify
 */
inline void toUpperCase(std::string& input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c) { return std::toupper(c); });
}

/**
 * @brief Remove all leading and trailing whitespace from a string.
 *
 * @param str String to clean up.
 * @return std::string
 */
inline std::string trim(const std::string& str) {
    std::regex whiteSpaceRegex("^\\s*|\\s*$");
    return std::regex_replace(str, whiteSpaceRegex, "");
}

/**
 * @brief Check whether an element of type `T` exists in an array.
 *
 * @param vec The vector to search
 * @param element Element to find within the vector.
 *
 * @returns bool
 */
template <typename T>
inline bool vecContains(const std::vector<T>& vec, const T& element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

/**
 * @brief Check whether a key exists within a map.
 *
 * @param map The map to search.
 * @param key Key to check existence of.
 * @returns bool
 */
template <typename T1, typename T2>
inline bool mapHasKey(const std::unordered_map<T1, T2>& map, const T1& key) {
    return map.find(key) != map.end();
}

/**
 * @brief Finds the most significant 1 bit in a value. Return value is 0-indexed.
 * Returns 32 if the input is 0.
 *
 * @param value The value to find MSB of.
 * @return uint32_t
 */
inline uint32_t getMSB(uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        if (value & (0b1 << i)) return i;
    }
    return 32;
}

/**
 * @brief Finds the least significant 1 bit in a value. Return value is 0-indexed.
 * Returns 32 if the input is 0.
 *
 * @param value The value to find LSB of.
 * @return uint32_t
 */
inline uint32_t getLSB(uint32_t value) {
    for (int i = 0; i < 32; i++) {
        if (value & (0b1 << i)) return i;
    }
    return 32;
}

#endif