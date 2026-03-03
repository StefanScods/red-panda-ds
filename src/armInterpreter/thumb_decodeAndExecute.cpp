/**
 * @file armInterpreter/thumb_decodeAndExecute.cpp
 * @brief Implements THUMB mode instuction set decoding helpers used in execute.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
// Shift (immediate), add, subtract, move, compare
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding/Shift--immediate---add--subtract--move--and-compare?lang=en
// ==================================================================================================
cycles ARM::THUMB_shiftAddSubtractMoveCompareDecodeAndExecute(uint32_t instruct) {
    uint8_t opcode = readBits(instruct, 9, 13);
    switch (opcode) {
        // LSL (immediate)
        case 0b00000:
        case 0b00001:
        case 0b00010:
        case 0b00011:
            return THUMB_LSL_IMM(instruct);
        // LSR (immediate)
        case 0b00100:
        case 0b00101:
        case 0b00110:
        case 0b00111:
            return THUMB_LSR_IMM(instruct);
        // ASR (immediate)
        case 0b01000:
        case 0b01001:
        case 0b01010:
        case 0b01011:
            return THUMB_ASR_IMM(instruct);
        // ADD (register)
        case 0b01100:
            return THUMB_ADD_REG(instruct);
        // SUB (register)
        case 0b01101:
            return THUMB_SUB_REG(instruct);
        // ADD (3-bit immediate)
        case 0b01110:
            return THUMB_ADD_3IMM(instruct);
        // SUB (3-bit immediate)
        case 0b01111:
            return THUMB_SUB_3IMM(instruct);
        // MOV (immediate)
        case 0b10000:
        case 0b10001:
        case 0b10010:
        case 0b10011:
            return THUMB_MOV_IMM(instruct);
        // 	CMP (immediate)
        case 0b10100:
        case 0b10101:
        case 0b10110:
        case 0b10111:
            return THUMB_CMP_IMM(instruct);
        // ADD (8-bit immediate)
        case 0b11000:
        case 0b11001:
        case 0b11010:
        case 0b11011:
            return THUMB_ADD_IMM(instruct);
        // SUB (8-bit immediate)
        case 0b11100:
        case 0b11101:
        case 0b11110:
        case 0b11111:
            return THUMB_SUB_IMM(instruct);
        default:
            break;
    }
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Data-processing
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding/Data-processing?lang=en
// ==================================================================================================
cycles ARM::THUMB_dataProcessingDecodeAndExecute(uint32_t instruct) {
    uint8_t opcode = readBits(instruct, 6, 9);
    switch (opcode) {
        // AND (register)
        case 0b0000:
            return THUMB_AND_REG(instruct);
        // EOR (register)
        case 0b0001:
            return THUMB_EOR_REG(instruct);
        // LSL (register)
        case 0b0010:
            return THUMB_LSL_REG(instruct);
        // LSR (register)
        case 0b0011:
            return THUMB_LSR_REG(instruct);
        // ASR (register)
        case 0b0100:
            return THUMB_ASR_REG(instruct);
        // ADC (register)
        case 0b0101:
            return THUMB_ASC_REG(instruct);
        // SBC (register)
        case 0b0110:
            return THUMB_SBC_REG(instruct);
        // ROR (register)
        case 0b0111:
            return THUMB_ROR_REG(instruct);
        // TST (register)
        case 0b1000:
            return THUMB_TST_REG(instruct);
        // RSB (immediate)
        case 0b1001:
            return THUMB_RSB_REG(instruct);
        // CMP (register)
        case 0b1010:
            return THUMB_CMP_REG(instruct);
        // CMN (register)
        case 0b1011:
            return THUMB_CMN_REG(instruct);
        // ORR (register)
        case 0b1100:
            return THUMB_ORR_REG(instruct);
        // MUL
        case 0b1101:
            return THUMB_MUL(instruct);
        // BIC (register)
        case 0b1110:
            return THUMB_BIC_REG(instruct);
        // MVN (register)
        case 0b1111:
            return THUMB_MVN_REG(instruct);
        default:
            break;
    }
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Special data instructions and branch and exchange
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding/Special-data-instructions-and-branch-and-exchange?lang=en
// ==================================================================================================
cycles ARM::THUMB_specialDataAndBranchDecodeAndExecute(uint32_t instruct) {
    uint8_t opcode = readBits(instruct, 9, 13);
    switch (opcode) {
        // ADD (Low Registers)
        case 0b0000:
            return THUMB_ADD_REG_LOW(instruct);
        // ADD (High Registers)
        case 0b0001:
        case 0b0010:
        case 0b0011:
            return THUMB_ADD_REG_HIGH(instruct);
        // CMP (High Registers)
        case 0b0100:
        case 0b0101:
        case 0b0110:
        case 0b0111:
            return THUMB_CMP_REG_HIGH(instruct);
        // MOV (Low Registers)
        case 0b1000:
            return THUMB_MOV_REG_LOW(instruct);
        // MOV (High Registers)
        case 0b1001:
        case 0b1010:
        case 0b1011:
            return THUMB_MOV_REG_HIGH(instruct);
        // BX
        case 0b1100:
        case 0b1101:
            return THUMB_BX(instruct);
        // BLX (register)
        case 0b1110:
        case 0b1111:
            return THUMB_BLX(instruct);
        default:
            break;
    }
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Load/store single data item
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding/Load-store-single-data-item?lang=en
// ==================================================================================================
cycles ARM::THUMB_loadStoreDecodeAndExecute(uint32_t instruct) {
    uint8_t opA = readBits(instruct, 12, 15);
    switch (opA) {
        // All register based offset load / stores.
        case 0b0101: {
            uint8_t Rt = readBits(instruct, 0, 2);
            uint8_t Rn = readBits(instruct, 3, 5);
            uint8_t Rm = readBits(instruct, 6, 8);
            uint32_t offset = *activeRegs[Rm];
            // Decode the inner cases.
            uint8_t opB = readBits(instruct, 9, 11);
            switch (opB) {
                // Store Register
                case 0b000:
                    return THUMB_STR(Rt, Rn, offset);
                // Store Register Halfword
                case 0b001:
                    return THUMB_STRH(Rt, Rn, offset);
                // Store Register Byte
                case 0b010:
                    return THUMB_STRB(Rt, Rn, offset);
                // Load Register Signed Byte
                case 0b011:
                    return THUMB_LDRSB(Rt, Rn, offset);
                // Load Register
                case 0b100:
                    return THUMB_LDR(Rt, Rn, offset);
                // Load Register Halfword
                case 0b101:
                    return THUMB_LDRH(Rt, Rn, offset);
                // Load Register Byte
                case 0b110:
                    return THUMB_LDRB(Rt, Rn, offset);
                // Load Register Signed Halfword
                case 0b111:
                    return THUMB_LDRSH(Rt, Rn, offset);
                default:
                    break;
            }
            break;  // Invalid instruction.
        }
        // Word store with immediate offset.
        case 0b0110: {
            uint8_t Rt = readBits(instruct, 0, 2);
            uint8_t Rn = readBits(instruct, 3, 5);
            uint8_t offset = readBits(instruct, 6, 8) << 2;
            return readBit(instruct, 13) ? THUMB_LDR(Rt, Rn, offset) : THUMB_STR(Rt, Rn, offset);
        }
        // Byte store with immediate offset.
        case 0b0111: {
            uint8_t Rt = readBits(instruct, 0, 2);
            uint8_t Rn = readBits(instruct, 3, 5);
            uint8_t offset = readBits(instruct, 6, 8) << 2;
            return readBit(instruct, 13) ? THUMB_LDRB(Rt, Rn, offset) : THUMB_STRB(Rt, Rn, offset);
        }
        // Half word store with immediate offset.
        case 0b1000: {
            uint8_t Rt = readBits(instruct, 0, 2);
            uint8_t Rn = readBits(instruct, 3, 5);
            uint8_t offset = readBits(instruct, 6, 8) << 2;
            return readBit(instruct, 13) ? THUMB_LDRH(Rt, Rn, offset) : THUMB_STRH(Rt, Rn, offset);
        }
        // Word store with SP + immediate offset.
        case 0b1001: {
            uint8_t Rt = readBits(instruct, 8, 10);
            uint8_t offset = readBits(instruct, 0, 7) << 2;
            return readBit(instruct, 13) ? THUMB_LDR(Rt, SP_REGISTER_NUM, offset)
                                         : THUMB_STR(Rt, SP_REGISTER_NUM, offset);
        }
        default:
            break;
    }
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Miscellaneous 16-bit instructions
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding/Miscellaneous-16-bit-instructions?lang=en
// ==================================================================================================
cycles ARM::THUMB_miscDecodeAndExecute(uint32_t instruct) {
    uint8_t opcode = readBits(instruct, 7, 11);
    switch (opcode) {
        // ADD (SP plus immediate)
        case 0b00000:
            return THUMB_ADD_SP_IMM(instruct);
        // SUB (SP minus immediate)
        case 0b00001:
            return THUMB_SUB_SP_IMM(instruct);
        // CBNZ -> Unsupported on DS's arm7 and arm9 cores.
        case 0b00010:
        case 0b00011:
            return THUMB_UNDEFINED_INST(instruct);
        // SXTH -> Unsupported on DS's arm7 and arm9 cores.
        // SXTB -> Unsupported on DS's arm7 and arm9 cores.
        case 0b00100:
            return THUMB_UNDEFINED_INST(instruct);
        // UXTH -> Unsupported on DS's arm7 and arm9 cores.
        // UXTB -> Unsupported on DS's arm7 and arm9 cores.
        case 0b00101:
            return THUMB_UNDEFINED_INST(instruct);
        // CBNZ -> Unsupported on DS's arm7 and arm9 cores.
        case 0b00110:
        case 0b00111:
            return THUMB_UNDEFINED_INST(instruct);
        // PUSH
        case 0b01000:
        case 0b01001:
        case 0b01010:
        case 0b01011:
            return THUMB_PUSH(instruct);
        // SETEND -> Unsupported on DS's arm7 and arm9 cores.
        // CPS -> Unsupported on DS's arm7 and arm9 cores.
        case 0b01100:
            return THUMB_UNDEFINED_INST(instruct);
        // Undefined.
        case 0b01101:
        case 0b01110:
        case 0b01111:
        case 0b10000:
        case 0b10001:
            return THUMB_UNDEFINED_INST(instruct);
        // CBNZ -> Unsupported on DS's arm7 and arm9 cores.
        case 0b10010:
        case 0b10011:
            return THUMB_UNDEFINED_INST(instruct);
        // REV -> Unsupported on DS's arm7 and arm9 cores.
        // REV16 -> Unsupported on DS's arm7 and arm9 cores.
        case 0b10100:
            return THUMB_UNDEFINED_INST(instruct);
        // REVSH -> Unsupported on DS's arm7 and arm9 cores.
        case 0b10101:
            return THUMB_UNDEFINED_INST(instruct);
        // CBNZ -> Unsupported on DS's arm7 and arm9 cores.
        case 0b10110:
        case 0b10111:
            return THUMB_UNDEFINED_INST(instruct);
        // POP
        case 0b11000:
        case 0b11001:
        case 0b11010:
        case 0b11011:
            return THUMB_POP(instruct);
        // BKPT
        case 0b11100:
        case 0b11101:
            return THUMB_BKPT(instruct);
        // If-Then, and hints -> Unsupported on DS's arm7 and arm9 cores.
        case 0b11110:
        case 0b11111:
            return THUMB_UNDEFINED_INST(instruct);
        default:
            break;
    }
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Conditional branch, and Supervisor Call
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding/Conditional-branch--and-Supervisor-Call?lang=en
// ==================================================================================================
cycles ARM::THUMB_condBranchAndSupervisorCallDecodeAndExecute(uint32_t instruct) {
    uint8_t condition = readBits(instruct, 11, 8);
    switch (condition) {
        // Permanently undefined
        case ConditionMnemonics::AL: {
            return THUMB_UNDEFINED_INST(instruct);
        }
        // Supervisor Call.
        case ConditionMnemonics::SPECIAL: {
            return THUMB_SVC(instruct);
        }
        // All other cases are conditional branch operations.
        default:
            return THUMB_B_COND(instruct);
    }
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
