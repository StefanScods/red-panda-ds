/**
 * @file armInterpreter.cpp
 * @brief Implements arm instuction set decoding
 */
#include <cassert>
#include <cstdint>

#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Coprocessor-instructions--and-Supervisor-Call?lang=en
cycles ARM::coprocessorAndSupervisorDecodeAndExecute(uint32_t instruct, uint8_t cond) {
    // Extract useful parts of the instruction in order to decode.
    // OpCode.
    uint8_t op1 = readBits(instruct, 20, 25);
    uint8_t op = readBit(instruct, 4);
    uint8_t opcode = op1 << 1 | op;
    // Check that the coproc is supported.
    uint8_t coproc = readBits(instruct, 9, 11);
    if (coproc == 0b101) {
        LogError("Coprocessor " << coproc << " is not supported!");
    }
    switch (op) {
        // Undefined.
        case 0b0000000:
        case 0b0000001:
        case 0b0000010:
        case 0b0000011:
            return ARM_UNDEFINED_INST(instruct);
        // STC
        case 0b0000100:
        case 0b0000101:
            return ARM_STC(instruct);
        // LDC
        case 0b0000110:
        case 0b0000111:
            return ARM_LDC(instruct);
        // MCRR
        case 0b0001000:
        case 0b0001001:
            return ARM_MCRR(instruct);
        // MRRC
        case 0b0001010:
        case 0b0001011:
            return ARM_MRRC(instruct);
        // STC
        case 0b0001100:
        case 0b0001101:
            return ARM_STC(instruct);
        // LDC
        case 0b0001110:
        case 0b0001111:
            return ARM_LDC(instruct);
        // STC
        case 0b0010000:
        case 0b0010001:
            return ARM_STC(instruct);
        // LDC
        case 0b0010010:
        case 0b0010011:
            return ARM_LDC(instruct);
        // STC
        case 0b0010100:
        case 0b0010101:
            return ARM_STC(instruct);
        // LDC
        case 0b0010110:
        case 0b0010111:
            return ARM_LDC(instruct);
        // STC
        case 0b0011000:
        case 0b0011001:
            return ARM_STC(instruct);
        // LDC
        case 0b0011010:
        case 0b0011011:
            return ARM_LDC(instruct);
        // STC
        case 0b0011100:
        case 0b0011101:
            return ARM_STC(instruct);
        // LDC
        case 0b0011110:
        case 0b0011111:
            return ARM_LDC(instruct);
        // STC
        case 0b0100000:
        case 0b0100001:
            return ARM_STC(instruct);
        // LDC
        case 0b0100010:
        case 0b0100011:
            return ARM_LDC(instruct);
        // STC
        case 0b0100100:
        case 0b0100101:
            return ARM_STC(instruct);
        // LDC
        case 0b0100110:
        case 0b0100111:
            return ARM_LDC(instruct);
        // STC
        case 0b0101000:
        case 0b0101001:
            return ARM_STC(instruct);
        // LDC
        case 0b0101010:
        case 0b0101011:
            return ARM_LDC(instruct);
        // STC
        case 0b0101100:
        case 0b0101101:
            return ARM_STC(instruct);
        // LDC
        case 0b0101110:
        case 0b0101111:
            return ARM_LDC(instruct);
        // STC
        case 0b0110000:
        case 0b0110001:
            return ARM_STC(instruct);
        // LDC
        case 0b0110010:
        case 0b0110011:
            return ARM_LDC(instruct);
        // STC
        case 0b0110100:
        case 0b0110101:
            return ARM_STC(instruct);
        // LDC
        case 0b0110110:
        case 0b0110111:
            return ARM_LDC(instruct);
        // STC
        case 0b0111000:
        case 0b0111001:
            return ARM_STC(instruct);
        // LDC
        case 0b0111010:
        case 0b0111011:
            return ARM_LDC(instruct);
        // STC
        case 0b0111100:
        case 0b0111101:
            return ARM_STC(instruct);
        // LDC
        case 0b0111110:
        case 0b0111111:
            return ARM_LDC(instruct);
        // CDP
        case 0b1000000:
            return ARM_CDP(instruct);
        // MCR
        case 0b1000001:
            return ARM_MCR(instruct);
        // CDP
        case 0b1000010:
            return ARM_CDP(instruct);
        // MRC
        case 0b1000011:
            return ARM_MRC(instruct);
        // CDP
        case 0b1000100:
            return ARM_CDP(instruct);
        // MCR
        case 0b1000101:
            return ARM_MCR(instruct);
        // CDP
        case 0b1000110:
            return ARM_CDP(instruct);
        // MRC
        case 0b1000111:
            return ARM_MRC(instruct);
        // CDP
        case 0b1001000:
            return ARM_CDP(instruct);
        // MCR
        case 0b1001001:
            return ARM_MCR(instruct);
        // CDP
        case 0b1001010:
            return ARM_CDP(instruct);
        // MRC
        case 0b1001011:
            return ARM_MRC(instruct);
        // CDP
        case 0b1001100:
            return ARM_CDP(instruct);
        // MCR
        case 0b1001101:
            return ARM_MCR(instruct);
        // CDP
        case 0b1001110:
            return ARM_CDP(instruct);
        // MRC
        case 0b1001111:
            return ARM_MRC(instruct);
        // CDP
        case 0b1010000:
            return ARM_CDP(instruct);
        // MCR
        case 0b1010001:
            return ARM_MCR(instruct);
        // CDP
        case 0b1010010:
            return ARM_CDP(instruct);
        // MRC
        case 0b1010011:
            return ARM_MRC(instruct);
        // CDP
        case 0b1010100:
            return ARM_CDP(instruct);
        // MCR
        case 0b1010101:
            return ARM_MCR(instruct);
        // CDP
        case 0b1010110:
            return ARM_CDP(instruct);
        // MRC
        case 0b1010111:
            return ARM_MRC(instruct);
        // CDP
        case 0b1011000:
            return ARM_CDP(instruct);
        // MCR
        case 0b1011001:
            return ARM_MCR(instruct);
        // CDP
        case 0b1011010:
            return ARM_CDP(instruct);
        // MRC
        case 0b1011011:
            return ARM_MRC(instruct);
        // CDP
        case 0b1011100:
            return ARM_CDP(instruct);
        // MCR
        case 0b1011101:
            return ARM_MCR(instruct);
        // CDP
        case 0b1011110:
            return ARM_CDP(instruct);
        // MRC
        case 0b1011111:
            return ARM_MRC(instruct);
        // Supervisor Call
        case 0b1100000:
        case 0b1100001:
        case 0b1100010:
        case 0b1100011:
        case 0b1100100:
        case 0b1100101:
        case 0b1100110:
        case 0b1100111:
        case 0b1101000:
        case 0b1101001:
        case 0b1101010:
        case 0b1101011:
        case 0b1101100:
        case 0b1101101:
        case 0b1101110:
        case 0b1101111:
        case 0b1110000:
        case 0b1110001:
        case 0b1110010:
        case 0b1110011:
        case 0b1110100:
        case 0b1110101:
        case 0b1110110:
        case 0b1110111:
        case 0b1111000:
        case 0b1111001:
        case 0b1111010:
        case 0b1111011:
        case 0b1111100:
        case 0b1111101:
        case 0b1111110:
        case 0b1111111:
            return ARM_SVC(instruct);
        default:
            break;
    }

    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// BKPT
// ==================================================================================================
cycles ARM::ARM_BKPT(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// SVC
// ==================================================================================================
cycles ARM::ARM_SVC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// MCRR
// ==================================================================================================
cycles ARM::ARM_MCRR(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// MCR
// ==================================================================================================
cycles ARM::ARM_MCR(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// MRRC
// ==================================================================================================
cycles ARM::ARM_MRRC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// MRC
// ==================================================================================================
cycles ARM::ARM_MRC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// STC
// ==================================================================================================
cycles ARM::ARM_STC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// LDC
// ==================================================================================================
cycles ARM::ARM_LDC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// CDP
// ==================================================================================================
cycles ARM::ARM_CDP(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
