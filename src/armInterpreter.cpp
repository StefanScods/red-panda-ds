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

// !!!TODO Handle when commands target PC.

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions?lang=en
cycles ARM::dataProcessingDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    // Extract useful parts of the instruction in order to decode.
    // OpCode.
    uint8_t op = readBits(instuct, 21, 24);
    // S - Set Condition Codes
    bool S = readBit(instuct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instuct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instuct, 12, 15);
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--immediate-?lang=en
    if (readBit(instuct, 25)) {  // Bit 25 = 1;
        uint32_t imm12 = readBits(instuct, 0, 11);
        u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
        switch (op) {
            // AND (immediate).
            case 0b0000:
                return ARM_AND(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            // EOR (immediate).
            case 0b0001:
                return ARM_EOR(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            // SUB (immediate).
            case 0b0010:
                return ARM_SUB(Rd, *activeRegs[Rn], immDecoded.data_u32, S);
            // RSB (immediate).
            case 0b0011:
                return ARM_RSB(Rd, *activeRegs[Rn], immDecoded.data_u32, S);
            // ADD (immediate).
            case 0b0100:
                return ARM_ADD(Rd, *activeRegs[Rn], immDecoded.data_u32, S);
            // ADC (immediate).
            case 0b0101:
                return ARM_ADC(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            // SBC (immediate).
            case 0b0110:
                return ARM_SBC(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            // RSC (immediate).
            case 0b0111:
                return ARM_RSC(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            case 0b1000: {
                // TST (immediate).
                if (S == 1) {
                    assert(Rd == 0);
                    return ARM_TST(*activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool);
                }
                // MOVW (immediate).
                // Not supported fall though,
            }
            case 0b1001: {
                // TEQ (immediate).
                if (S == 1) {
                    assert(Rd == 0);
                    return ARM_TEQ(*activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool);
                }
                // MSR (immediate).
                // Not supported fall though,
            }
            case 0b1010: {
                // CMP (immediate).
                if (S == 1) {
                    assert(Rd == 0);
                    return ARM_CMP(*activeRegs[Rn], immDecoded.data_u32);
                }
                // MOVT (immediate).
                // Not supported fall though,
            }
            case 0b1011: {
                // CMN (immediate).
                if (S == 1) {
                    assert(Rd == 0);
                    return ARM_CMN(*activeRegs[Rn], immDecoded.data_u32);
                }
                // MSR (immediate).
                // Not supported fall though,
            }
            // ORR (immediate).
            case 0b1100:
                return ARM_ORR(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            // MOV (immediate).
            case 0b1101:
                assert(Rn == 0);
                return ARM_MOV(Rd, immDecoded.data_u32, immDecoded.data_bool, S);
            // BIC (immediate).
            case 0b1110:
                return ARM_BIC(Rd, *activeRegs[Rn], immDecoded.data_u32, immDecoded.data_bool, S);
            // MVN (immediate).
            case 0b1111:
                assert(Rn == 0);
                return ARM_MVN(Rd, immDecoded.data_u32, immDecoded.data_bool, S);
        }
    } else {  // Bit 25 = 0;
        bool bit4 = readBits(instuct, 20, 24);
    }

    LogError("Unsupported instruction: " << instuct << "!");
    return 1;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Load-store-word-and-unsigned-byte?lang=en
cycles ARM::loadStoreDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    // Extract useful parts of the instruction in order to decode.
    // P - Pre/Post (0=post; add offset after transfer, 1=pre; before trans.)
    bool P = readBit(instuct, 24);
    // U - Up/Down Bit (0=down; subtract offset from base, 1=up; add to base)
    bool U = readBit(instuct, 23);
    /**
     * When above Bit 24 P=0 (Post-indexing, write-back is ALWAYS enabled):
     *   21     T - Memory Management (0=Normal, 1=Force non-privileged access)
     * When above Bit 24 P=1 (Pre-indexing, write-back is optional):
     *   21     W - Write-back bit (0=no write-back, 1=write address into base)
     */
    bool bit21 = readBit(instuct, 21);
    // 1st Operand Register.
    uint8_t Rn = readBits(instuct, 16, 19);
    // Destination / target Register.
    uint8_t Rd = readBits(instuct, 12, 15);
    uint8_t op = readBits(instuct, 20, 24);
    // I - Immediate Offset Flag (0=Immediate, 1=Shifted Register)
    bool I = readBit(instuct, 25);
    // Get the offset.
    uint32_t offset;
    if (I) {  // 1=Shifted Register
        // Shift type.
        uint8_t type = readBits(instuct, 5, 6);
        // Offset Register.
        uint8_t Rm = readBits(instuct, 0, 3);
        assert(Rm != 15);
        // Shift amount.
        uint8_t shiftAmount = readBits(instuct, 7, 11);
        offset = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG)).data_u32;
    } else {  // 0=Immediate.
        offset = readBits(instuct, 0, 11);
    }
    switch (op) {
        // STR (immediate, register)
        case 0b00000:
        case 0b01000:
        case 0b10000:
        case 0b10010:
        case 0b11000:
        case 0b11010:
            return ARM_STR(Rd, Rn, offset, P, U, (!P || bit21));
        // STR - T (immediate, register)
        case 0b00010:
        case 0b01010:
            return ARM_STRT(Rd, Rn, offset, U);
        // LDR (immediate, register)
        case 0b00001:
        case 0b01001:
        case 0b10001:
        case 0b10011:
        case 0b11001:
        case 0b11011:
            return ARM_LDR(Rd, Rn, offset, P, U, (!P || bit21));
        // LDR - T (immediate, register)
        case 0b00011:
        case 0b01011:
            return ARM_LDRT(Rd, Rn, offset, U);
        // STR - Byte (immediate, register)
        case 0b00100:
        case 0b01100:
        case 0b10100:
        case 0b10110:
        case 0b11100:
        case 0b11110:
            return ARM_STRB(Rd, Rn, offset, P, U, (!P || bit21));
        // STR - Byte - T (immediate, register)
        case 0b00110:
        case 0b01110:
            return ARM_STRBT(Rd, Rn, offset, U);
        // LDR - Byte (immediate, register)
        case 0b00101:
        case 0b01101:
        case 0b10101:
        case 0b10111:
        case 0b11101:
        case 0b11111:
            return ARM_LDRB(Rd, Rn, offset, P, U, (!P || bit21));
        // LDR - Byte - T (immediate, register)
        case 0b00111:
        case 0b01111:
            return ARM_LDRBT(Rd, Rn, offset, U);
    }

    LogError("Unsupported instruction: " << instuct << "!");
    return 1;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Branch--branch-with-link--and-block-data-transfer?lang=en
cycles ARM::branchDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    LogError("Unsupported instruction: " << instuct << "!");
    return 1;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Coprocessor-instructions--and-Supervisor-Call?lang=en
cycles ARM::coprocessorAndSupervisorDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    LogError("Unsupported instruction: " << instuct << "!");
    return 1;
}