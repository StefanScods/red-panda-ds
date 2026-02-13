/**
 * @file armInterpreter_branch.cpp
 * @brief Implements arm branch instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Branch--branch-with-link--and-block-data-transfer?lang=en
cycles ARM::branchDecodeAndExecute(uint32_t instruct, uint8_t cond) {
    uint8_t op = readBits(instruct, 20, 25);
    switch (op) {
        // Unsupported
        case 0b000000:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000001:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000010:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000011:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000100:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000101:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000110:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b000111:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001000:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001001:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001010:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001011:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001100:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001101:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001110:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b001111:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010000:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010001:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010010:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010011:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010100:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010101:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010110:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b010111:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011000:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011001:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011010:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011011:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011100:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011101:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011110:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b011111:
            return ARM_UNDEFINED_INST(instruct);
        // Branch
        case 0b100000:
        case 0b100001:
        case 0b100010:
        case 0b100011:
        case 0b100100:
        case 0b100101:
        case 0b100110:
        case 0b100111:
        case 0b101000:
        case 0b101001:
        case 0b101010:
        case 0b101011:
        case 0b101100:
        case 0b101101:
        case 0b101110:
        case 0b101111:
            return ARM_B(instruct);
        // Unsupported
        case 0b110000:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110001:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110010:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110011:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110100:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110101:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110110:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b110111:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111000:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111001:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111010:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111011:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111100:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111101:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111110:
            return ARM_UNDEFINED_INST(instruct);
        // Unsupported
        case 0b111111:
            return ARM_UNDEFINED_INST(instruct);
        default:
            break;
    }
    LogError("Unsupported instruction: " << instruct << "!");
    return 1;
}
// ==================================================================================================
// Branch
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/B?lang=en
// ==================================================================================================
cycles ARM::ARM_B(uint32_t instruct) {
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6;
    branch(pc + offset);
    return 1;
}
// ==================================================================================================