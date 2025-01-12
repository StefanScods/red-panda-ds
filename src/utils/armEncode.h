/**
 * @file armEncode.h
 * @brief Covert written instructions into their encoded binary equivelent.
 * WIP!!!!
 */
#ifndef ARMENCODE_H
#define ARMENCODE_H

#include <cstdint>
#include <string>
#include <unordered_map>

#define INVALID_INSTRUCT 0

typedef uint32_t opHint;

/**
 * @brief Checks the operation token if this operation should update flags.
 *
 * @param instruction - First token in the instuction stream.
 * @return bool
 */
bool checkIfUpdateFlag(std::string instruction);

/**
 * @brief Checks if an operand is an immediate value. All immediates are prefixed with #
 *
 * @param operand The operand to check.
 * @return bool
 */
bool checkOperandIsImmediate(std::string operand);

/**
 * @brief Removes the update flags indicator from the instuction token.
 * This function assumes that checkIfUpdateFlag() returns true on the passed input.
 *
 * @param instruction - First token in the instuction stream.
 * @return std::string
 */
std::string removeUpdateFlagFromInstuction(std::string instruction);

/**
 * @brief Helper function which takes a immediate expression and encodes it.
 *
 * @param immExpr Expression to encode.
 * @param size The size of the encoding in bits (12, )
 * @return uint32_t
 */
uint32_t encodeImmValue(std::string immExpr, uint32_t size);

uint32_t encodeDataInstuction(std::string instruction, std::string Rn, std::string Rd,
                              std::string secondOperandExpr, bool updateFlags, bool immediate);

/**
 * @brief
 *
 *
 * @param instruction
 * @return uint32_t
 */
uint32_t armEncodeASM(std::string instruction);

#endif