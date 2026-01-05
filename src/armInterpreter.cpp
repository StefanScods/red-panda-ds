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
    uint8_t op1 = readBits(instuct, 20, 24);
    uint8_t op2 = readBits(instuct, 4, 7);
    // S - Set Condition Codes
    bool S = readBit(instuct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instuct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instuct, 12, 15);
    // Data processing arguments.
    uint32_t opperand1 = *activeRegs[Rn];
    // Bit 25 determines how these are calculated.
    uint32_t opperand2 = 0;
    bool carry = false;
    // Misc instructions.
    if (readBit(instuct, 25) == 0 && readBit(op2, 0) == 1 && readBit(op2, 3) == 1) {
        LogError("Unsupported instruction: " << instuct << "!");
        return 1;
    }
    // For basic data processing, use bit 25 to determine the second operand.
    if (readBit(instuct, 25) == 1) {
        // Intermediate encodings.
        uint32_t imm12 = readBits(instuct, 0, 11);
        u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));

        opperand2 = immDecoded.data_u32;
        carry = immDecoded.data_bool;
    } else {
        // Register encodings.
        if (readBit(op2, 0) == 0) {  // Instruction bit 4 = 0.
            // Process the second operand.
            uint8_t imm5 = readBits(instuct, 7, 11);
            uint8_t type = readBits(instuct, 5, 6);
            decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
            uint8_t Rm = readBits(instuct, 0, 3);
            u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));

            opperand2 = operandShifted.data_u32;
            carry = operandShifted.data_bool;
        }
        // Register shifted register encoding.
        else {  // Instruction bit 4 = 1.
            // Process the second operand.
            uint8_t Rs = readBits(instuct, 8, 11);
            uint8_t type = readBits(instuct, 5, 6);
            uint8_t Rm = readBits(instuct, 0, 3);
            uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
            u32AndBool operandShifted =
                ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
            opperand2 = operandShifted.data_u32;
            carry = operandShifted.data_bool;
        }
    }
    // Handle each op code as described:
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--register-shifted-register-?lang=en
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--register-?lang=en
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--immediate-?lang=en
    switch (op1) {
        // AND (immediate).
        case 0b00000:
        case 0b00001:
            return ARM_AND(Rd, opperand1, opperand2, carry, S);
        // EOR (immediate).
        case 0b00010:
        case 0b00011:
            return ARM_EOR(Rd, opperand1, opperand2, carry, S);
        // SUB (immediate).
        case 0b00100:
        case 0b00101:
            return ARM_SUB(Rd, opperand1, opperand2, S);
        // RSB (immediate).
        case 0b00110:
        case 0b00111:
            return ARM_RSB(Rd, opperand1, opperand2, S);
        // ADD (immediate).
        case 0b01000:
        case 0b01001:
            return ARM_ADD(Rd, opperand1, opperand2, S);
        // ADC (immediate).
        case 0b01010:
        case 0b01011:
            return ARM_ADC(Rd, opperand1, opperand2, carry, S);
        // SBC (immediate).
        case 0b01100:
        case 0b01101:
            return ARM_SBC(Rd, opperand1, opperand2, carry, S);
        // RSC (immediate).
        case 0b01110:
        case 0b01111:
            return ARM_RSC(Rd, opperand1, opperand2, carry, S);
        // Misc instructions.
        case 0b10000:
        case 0b10010:
        case 0b10100:
        case 0b10110:
            // MOVW (immediate).
            // MSR (immediate).
            // MOVT (immediate).
            // Not supported, break and error.
            break;
        // TST (immediate).
        case 0b10001:
            assert(Rd == 0);
            return ARM_TST(opperand1, opperand2, carry);
        // TEQ (immediate).
        case 0b10011:
            assert(Rd == 0);
            return ARM_TEQ(opperand1, opperand2, carry);
        // CMP (immediate).
        case 0b10101:
            assert(Rd == 0);
            return ARM_CMP(opperand1, opperand2);
        // CMN (immediate).
        case 0b10111:
            assert(Rd == 0);
            return ARM_CMN(opperand1, opperand2);
        // ORR (immediate).
        case 0b11000:
        case 0b11001:
            return ARM_ORR(Rd, opperand1, opperand2, carry, S);
        // MOV (immediate).
        case 0b11010:
        case 0b11011:
            assert(Rn == 0);
            return ARM_MOV(Rd, opperand2, carry, S);
        // BIC (immediate).
        case 0b11100:
        case 0b11101:
            return ARM_BIC(Rd, opperand1, opperand2, carry, S);
        // MVN (immediate).
        case 0b11110:
        case 0b11111:
            assert(Rn == 0);
            return ARM_MVN(Rd, opperand2, carry, S);
        default:
            // Unsupported, break and error.
            break;
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
        // Calculate the offset via a shift operation.
        decodeShiftResult shiftOp = ARMDecodeImmShift(type, shiftAmount);
        offset = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG)).data_u32;
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