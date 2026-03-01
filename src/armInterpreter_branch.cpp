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
        // Store Multiple Decrement After
        case 0b000000:
        case 0b000010:
            return ARM_STMDA(instruct);
        // 	Load Multiple Decrement After
        case 0b000001:
        case 0b000011:
            return ARM_LDMDA(instruct);
        // Store Multiple Increment After
        case 0b001000:
        case 0b001010:
            return ARM_STMIA(instruct);
        // 	Load Multiple Increment After
        case 0b001001:
        case 0b001011:
            return ARM_LDMIA(instruct);
        // Store Multiple Decrement Before
        case 0b010000:
        case 0b010010:
            return ARM_STMDB(instruct);
        // 	Load Multiple Decrement Before
        case 0b010001:
        case 0b010011:
            return ARM_LDMDB(instruct);
        // Store Multiple Increment Before
        case 0b011000:
        case 0b011010:
            return ARM_STMIB(instruct);
        // 	Load Multiple Increment Before
        case 0b011001:
        case 0b011011:
            return ARM_LDMIB(instruct);
        // Store Multiple (user registers)
        case 0b000100:
        case 0b000110:
        case 0b001100:
        case 0b001110:
        case 0b010100:
        case 0b010110:
        case 0b011100:
        case 0b011110:
            return ARM_STM_USER_REG(instruct);
        // Load Multiple (user registers).
        case 0b000101:
        case 0b000111:
        case 0b001101:
        case 0b001111:
        case 0b010101:
        case 0b010111:
        case 0b011101:
        case 0b011111:
            return ARM_LDM_USER_REG(instruct);
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
            if (cond == ConditionMnemonics::SPECIAL) return ARM_BLX_IMM(instruct);
            return ARM_B(instruct);
        // Branch Link and Exchange
        case 0b110000:
        case 0b110001:
        case 0b110010:
        case 0b110011:
        case 0b110100:
        case 0b110101:
        case 0b110110:
        case 0b110111:
        case 0b111000:
        case 0b111001:
        case 0b111010:
        case 0b111011:
        case 0b111100:
        case 0b111101:
        case 0b111110:
        case 0b111111: {
            if (cond == ConditionMnemonics::SPECIAL) return ARM_BLX_IMM(instruct);
            return ARM_BL(instruct);
        }

        default:
            break;
    }
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Branch
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/B?lang=en
// ==================================================================================================
cycles ARM::ARM_B(uint32_t instruct) {
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6;
    branch(pc() + offset);
    return 1;
}
// ==================================================================================================
// Branch Link and Exchange
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BL--BLX--immediate-?lang=en//
// ==================================================================================================
cycles ARM::ARM_BL(uint32_t instruct) {
    lr() = pc() - 4;  // Point to the previous instruction.
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6;
    branch(pc() + offset);
    return 1;
}
// ==================================================================================================
cycles ARM::ARM_BLX_IMM(uint32_t instruct) {
    lr() = pc() - 4;  // Point to the previous instruction.
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    bool H = readBit(instruct, 24);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6 | (H << 1);
    branch(pc() + offset + 1);  // Offset by + 1 to enter thumb mode.
    return 1;
}
// ==================================================================================================
cycles ARM::ARM_BLX_REG(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
cycles ARM::ARM_BX(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
