/**
 * @file armEncode.cpp
 * @brief Covert written instructions into their encoded binary equivelent.
 * WIP!!!!
 */
#include "armEncode.h"

#include "utils.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"
const char LOGGER_PREFIX[] = "armEncodeASM";

// ========= Encoder Data =========================================================================

/**
 * @brief Maps a register name to the binary encoding.
 */
std::unordered_map<std::string, uint8_t> regEncodings = {
    {"r0", 0},   {"r1", 1},   {"r2", 2},   {"r3", 3},   {"r4", 4},   {"r5", 5},
    {"r6", 6},   {"r7", 7},   {"r8", 8},   {"r9", 9},   {"r10", 10}, {"r11", 11},
    {"r12", 12}, {"r13", 13}, {"r14", 14}, {"r15", 15}, {"pc", 15},  {"", 0}};

/**
 * @brief The operations which belong to the data group.
 * https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--immediate-?lang=en
 */
std::vector<std::string> dataOperations = {"mov"};

std::unordered_map<std::string, uint8_t> opEncodings = {{"mov", 0b1101}};

#define DEFINE_OP_HINT(NAME, SLOT)             \
    const opHint opHints_##NAME##_slot = SLOT; \
    const opHint opHints_##NAME = (1 << SLOT)

DEFINE_OP_HINT(ONE_OPERAND, 0);
DEFINE_OP_HINT(TWO_OPERAND, 1);
DEFINE_OP_HINT(THREE_OPERAND, 2);
DEFINE_OP_HINT(FOUR_OPERAND, 3);
DEFINE_OP_HINT(SUPPORTS_IMMEDIATE, 4);

std::unordered_map<std::string, opHint> opHints = {
    {"mov", opHints_TWO_OPERAND | opHints_THREE_OPERAND | opHints_SUPPORTS_IMMEDIATE}};

// ========= Helper Functions =====================================================================

bool checkIfUpdateFlag(std::string instruction) {
    return instruction[instruction.length() - 1] == 's';
}

bool checkOperandIsImmediate(std::string operand) {
    return operand[0] == '#';
}

std::string removeUpdateFlagFromInstuction(std::string instruction) {
    return instruction.substr(0, instruction.length() - 1);
}

uint32_t encodeImmValue(std::string immExpr, uint32_t size) {
    // Strip the leading # immediate identifier.
    immExpr = immExpr.substr(1, immExpr.length());
    LogInfoPrefixed("Encoding immediate value " << immExpr << " within " << size << " bits",
                    LOGGER_PREFIX);
    // Convert the string into a number. Use base zero to let std::stoul autodetect.
    uint32_t value = std::stoul(immExpr, nullptr, 0);
    LogDebugPrefixed("Converted " << immExpr << " to " << value, LOGGER_PREFIX);
    switch (size) {
        case 12: {
            if (value == 0) return value;
            // Trival case - value fits in 8 bits - no shift needed.
            if ((value & 0xFF) == value) return value;

            // Determine the rotation needed to store the immediate.
            LogDebugPrefixed("encodeImmValue - 12 bit - rotation needed to encode", LOGGER_PREFIX);
            // Try values 2-30 in steps of 2.
            for (int i = 2; i < 32; i += 2) {
                uint32_t rotatedEncoding = (ROL(value, i).data_u32) & 0xFF;
                if (ROR(rotatedEncoding, i).data_u32 != value) continue;
                LogDebugPrefixed("encodeImmValue - 12 bit - data: " << rotatedEncoding,
                                 LOGGER_PREFIX);
                LogDebugPrefixed("encodeImmValue - 12 bit - rotationNeeded: " << i, LOGGER_PREFIX);
                // Encode the 12 bits.
                writeBits(rotatedEncoding, i >> 1, 8, 11);
                return rotatedEncoding;
            }

            // If there is data loss, print a warning.
            uint32_t lossyValue = value & 0xFF;
            LogWarningPrefixed(
                "Cannot fully encode immediate value. Returned lossy value - " << lossyValue,
                LOGGER_PREFIX);
            return lossyValue;
        }
    }
    LogErrorPrefixed("Unsupported immediate encoding size!", LOGGER_PREFIX);
    return 0;
}

// ========= Encoders =============================================================================

uint32_t encodeDataInstuction(std::string instruction, std::string Rn, std::string Rd,
                              std::string secondOperandExpr, bool updateFlags, bool immediate) {
    LogDebugPrefixed("Decoding instruction as a data instruction", LOGGER_PREFIX);
    LogDebugPrefixed("Rn = " << Rn, LOGGER_PREFIX);
    LogDebugPrefixed("Rd = " << Rd, LOGGER_PREFIX);
    LogDebugPrefixed("secondOperandExpr = " << secondOperandExpr, LOGGER_PREFIX);
    uint32_t encoding = 0;
    // Bit 25 - Immediate 2nd Operand Flag (0=Register, 1=Immediate).
    writeBit(encoding, immediate, 25);
    // Bits 21-24 - Opcode.
    writeBits(encoding, opEncodings[instruction], 21, 24);
    // Bit 20 - Set Condition Codes (0=No, 1=Yes).
    writeBit(encoding, updateFlags, 20);
    // Bits 16-19 - 1st Operand Register.
    writeBits(encoding, regEncodings[Rn], 16, 19);
    // Bits 12-15 - Destination Register.
    writeBits(encoding, regEncodings[Rd], 12, 15);
    // Bits 0-11 - Immediate.
    if (immediate) {
        writeBits(encoding, encodeImmValue(secondOperandExpr, 12), 0, 11);
    } else {
    }

    LogDebugPrefixed("Successfully encoded data instruction: " << encoding, LOGGER_PREFIX);
    return encoding;
}

// ========= Encoder Main =========================================================================

uint32_t armEncodeASM(std::string instruction) {
    LogDebugPrefixed("Encoding " << instruction, LOGGER_PREFIX);
    // Standardize the input.
    toLowerCase(instruction);
    std::istringstream instructionStream(instruction);
    // Extract operation and operands.
    std::string operation;
    instructionStream >> operation;
    std::string operands;
    std::getline(instructionStream >> std::ws, operands, '\0');
    if (operation.empty() || operands.empty()) {
        LogErrorPrefixed("Expecting format <OPERATION> <OPERANDS> got: " << instruction << "!",
                         LOGGER_PREFIX);
        return INVALID_INSTRUCT;
    }
    // Check to see if the operation updates CPU flags.
    bool S = checkIfUpdateFlag(operation);
    if (S) {
        LogDebugPrefixed("Operation will update flags", LOGGER_PREFIX);
        // Strip the indicator off the string.
        operation = removeUpdateFlagFromInstuction(operation);
    }
    LogInfoPrefixed("Got op " << operation, LOGGER_PREFIX);
    // Check if the operation exists.
    if (!mapHasKey(opHints, operation)) {
        LogErrorPrefixed("Unsupported operation " << operation << "!", LOGGER_PREFIX);
        return INVALID_INSTRUCT;
    }
    // Get the hint and encoding.
    opHint hint = opHints[operation];
    uint32_t encoding = opEncodings[operation];
    // Get the operand tokens.
    std::vector<std::string> operandTokens = splitString(operands, std::regex("\\s*,\\s*"));
    for (int i = 0; i < operandTokens.size(); i++) {
        LogDebugPrefixed("Operand " << i << ": " << operandTokens[i], LOGGER_PREFIX);
    }
    bool usingImmediate = false;
    // Error check number of operands.
    switch (operandTokens.size()) {
        case 1: {
            if (!readBit(hint, opHints_ONE_OPERAND_slot)) {
                LogErrorPrefixed("Invalid number of operands needed for " << operation,
                                 LOGGER_PREFIX);
                return INVALID_INSTRUCT;
            }
            break;
        }
        case 2: {
            if (!readBit(hint, opHints_TWO_OPERAND_slot)) {
                LogErrorPrefixed("Invalid number of operands needed for " << operation,
                                 LOGGER_PREFIX);
                return INVALID_INSTRUCT;
            }
            // Get the two operands.
            std::string operand1 = operandTokens[0];
            std::string operand2 = operandTokens[1];

            // Check if the second operand is an immediate.
            usingImmediate = checkOperandIsImmediate(operand2);
            if (usingImmediate && !readBit(hint, opHints_SUPPORTS_IMMEDIATE_slot)) {
                LogErrorPrefixed("Operation " << operation << " does not support using immediates!",
                                 LOGGER_PREFIX);
                return INVALID_INSTRUCT;
            }
            // Check if a command is data processing operation.
            if (vecContains(dataOperations, operation)) {
                return encodeDataInstuction(operation, "", operand1, operand2, S, usingImmediate);
            }

            break;
        }
        case 3: {
            if (!readBit(hint, opHints_THREE_OPERAND_slot)) {
                LogErrorPrefixed("Invalid number of operands needed for " << operation,
                                 LOGGER_PREFIX);
                return INVALID_INSTRUCT;
            }
            break;
        }
        case 4: {
            if (!readBit(hint, opHints_FOUR_OPERAND_slot)) {
                LogErrorPrefixed("Invalid number of operands needed for " << operation,
                                 LOGGER_PREFIX);
                return INVALID_INSTRUCT;
            }
            break;
        }
        default: {
            LogErrorPrefixed("Invalid number of operands needed for " << operation, LOGGER_PREFIX);
            return INVALID_INSTRUCT;
        }
    }

    LogErrorPrefixed("Unable to encode " << instruction << "!", LOGGER_PREFIX);
    return INVALID_INSTRUCT;
}
