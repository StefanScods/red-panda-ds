/**
 * @file armInterpreter_data.cpp
 * @brief Implements arm data processing instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include <bit>

#include "logger.h"

// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions?lang=en
cycles ARM::dataProcessingDecodeAndExecute(uint32_t instruct, uint8_t cond) {
    // Extract useful parts of the instruction in order to decode.
    // OpCode.
    uint8_t op1 = readBits(instruct, 20, 25);
    uint8_t op2 = readBits(instruct, 4, 7);
    uint16_t op = op1 << 4 | op2;
    switch (op) {
        // AND (Register)
        case 0b0000000000:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000000001:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000000010:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000000011:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000000100:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000000101:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000000110:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000000111:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000001000:
            return ARM_AND_REG(instruct);
        // MUL
        case 0b0000001001:
            return ARM_MUL(instruct);
        // AND (Register)
        case 0b0000001010:
            return ARM_AND_REG(instruct);
        // STRH (register)
        case 0b0000001011:
            return ARM_STRH_REG(instruct);
        // AND (Register)
        case 0b0000001100:
            return ARM_AND_REG(instruct);
        // LDRD (Register)
        case 0b0000001101:
            return ARM_LDRD_REG(instruct);
        // AND (Register)
        case 0b0000001110:
            return ARM_AND_REG(instruct);
        // STRD (Register)
        case 0b0000001111:
            return ARM_STRD_REG(instruct);
        // AND (Register)
        case 0b0000010000:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000010001:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000010010:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000010011:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000010100:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000010101:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000010110:
            return ARM_AND_REG(instruct);
        // AND (Register Shifted)
        case 0b0000010111:
            return ARM_AND_REG_SHIFT(instruct);
        // AND (Register)
        case 0b0000011000:
            return ARM_AND_REG(instruct);
        // MUL
        case 0b0000011001:
            return ARM_MUL(instruct);
        // AND (Register)
        case 0b0000011010:
            return ARM_AND_REG(instruct);
        // LDRH (register)
        case 0b0000011011:
            return ARM_LDRH_REG(instruct);
        // AND (Register)
        case 0b0000011100:
            return ARM_AND_REG(instruct);
        // LDRSB (Register)
        case 0b0000011101:
            return ARM_LDRSB_REG(instruct);
        // AND (Register)
        case 0b0000011110:
            return ARM_AND_REG(instruct);
        // LDRSH (Register)
        case 0b0000011111:
            return ARM_LDRSH_REG(instruct);
        // EOR (Register)
        case 0b0000100000:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000100001:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000100010:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000100011:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000100100:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000100101:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000100110:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000100111:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000101000:
            return ARM_EOR_REG(instruct);
        // MLA
        case 0b0000101001:
            return ARM_MLA(instruct);
        // EOR (Register)
        case 0b0000101010:
            return ARM_EOR_REG(instruct);
        // STRH (register)
        case 0b0000101011:
            return ARM_STRH_REG(instruct);
        // EOR (Register)
        case 0b0000101100:
            return ARM_EOR_REG(instruct);
        // LDRD (Register)
        case 0b0000101101:
            return ARM_LDRD_REG(instruct);
        // EOR (Register)
        case 0b0000101110:
            return ARM_EOR_REG(instruct);
        // STRD (Register)
        case 0b0000101111:
            return ARM_STRD_REG(instruct);
        // EOR (Register)
        case 0b0000110000:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000110001:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000110010:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000110011:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000110100:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000110101:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000110110:
            return ARM_EOR_REG(instruct);
        // EOR (Register Shifted)
        case 0b0000110111:
            return ARM_EOR_REG_SHIFT(instruct);
        // EOR (Register)
        case 0b0000111000:
            return ARM_EOR_REG(instruct);
        // MLA
        case 0b0000111001:
            return ARM_MLA(instruct);
        // EOR (Register)
        case 0b0000111010:
            return ARM_EOR_REG(instruct);
        // LDRH (Register)
        case 0b0000111011:
            return ARM_LDRH_REG(instruct);
        // EOR (Register)
        case 0b0000111100:
            return ARM_EOR_REG(instruct);
        // LDRSB (Register)
        case 0b0000111101:
            return ARM_LDRSB_REG(instruct);
        // EOR (Register)
        case 0b0000111110:
            return ARM_EOR_REG(instruct);
        // LDRSH (Register)
        case 0b0000111111:
            return ARM_LDRSH_REG(instruct);
        // SUB (Register)
        case 0b0001000000:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001000001:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001000010:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001000011:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001000100:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001000101:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001000110:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001000111:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001001000:
            return ARM_SUB_REG(instruct);
        // UMAAL -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0001001001:
            return ARM_UNDEFINED_INST(instruct);
        // SUB (Register)
        case 0b0001001010:
            return ARM_SUB_REG(instruct);
        // STRH (Immediate)
        case 0b0001001011:
            return ARM_STRH_IMM(instruct);
        // SUB (Register)
        case 0b0001001100:
            return ARM_SUB_REG(instruct);
        // LDRD (Immediate)
        case 0b0001001101:
            return ARM_LDRD_IMM(instruct);
        // SUB (Register)
        case 0b0001001110:
            return ARM_SUB_REG(instruct);
        // STRD (Immediate)
        case 0b0001001111:
            return ARM_STRD_IMM(instruct);
        // SUB (Register)
        case 0b0001010000:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001010001:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001010010:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001010011:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001010100:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001010101:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001010110:
            return ARM_SUB_REG(instruct);
        // SUB (Register Shifted)
        case 0b0001010111:
            return ARM_SUB_REG_SHIFT(instruct);
        // SUB (Register)
        case 0b0001011000:
            return ARM_SUB_REG(instruct);
        // Undefined
        case 0b0001011001:
            return ARM_UNDEFINED_INST(instruct);
        // SUB (Register)
        case 0b0001011010:
            return ARM_SUB_REG(instruct);
        // LDRH (Immediate)
        case 0b0001011011:
            return ARM_LDRH_IMM(instruct);
        // SUB (Register)
        case 0b0001011100:
            return ARM_SUB_REG(instruct);
        // LDRSB (Immediate)
        case 0b0001011101:
            return ARM_LDRSB_IMM(instruct);
        // SUB (Register)
        case 0b0001011110:
            return ARM_SUB_REG(instruct);
        // LDRSH (Immediate)
        case 0b0001011111:
            return ARM_LDRSH_IMM(instruct);
        // RSB (Register)
        case 0b0001100000:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001100001:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001100010:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001100011:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001100100:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001100101:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001100110:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001100111:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001101000:
            return ARM_RSB_REG(instruct);
        // MLS -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0001101001:
            return ARM_UNDEFINED_INST(instruct);
        // RSB (Register)
        case 0b0001101010:
            return ARM_RSB_REG(instruct);
        // STRH (Immediate)
        case 0b0001101011:
            return ARM_STRH_IMM(instruct);
        // RSB (Register)
        case 0b0001101100:
            return ARM_RSB_REG(instruct);
        // LDRD (Immediate)
        case 0b0001101101:
            return ARM_LDRD_IMM(instruct);
        // RSB (Register)
        case 0b0001101110:
            return ARM_RSB_REG(instruct);
        // STRD (Immediate)
        case 0b0001101111:
            return ARM_STRD_IMM(instruct);
        // RSB (Register)
        case 0b0001110000:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001110001:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001110010:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001110011:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001110100:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001110101:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001110110:
            return ARM_RSB_REG(instruct);
        // RSB (Register Shifted)
        case 0b0001110111:
            return ARM_RSB_REG_SHIFT(instruct);
        // RSB (Register)
        case 0b0001111000:
            return ARM_RSB_REG(instruct);
        // Undefined
        case 0b0001111001:
            return ARM_UNDEFINED_INST(instruct);
        // RSB (Register)
        case 0b0001111010:
            return ARM_RSB_REG(instruct);
        // LDRH (Immediate)
        case 0b0001111011:
            return ARM_LDRH_IMM(instruct);
        // RSB (Register)
        case 0b0001111100:
            return ARM_RSB_REG(instruct);
        // LDRSB (Immediate)
        case 0b0001111101:
            return ARM_LDRSB_IMM(instruct);
        // RSB (Register)
        case 0b0001111110:
            return ARM_RSB_REG(instruct);
        // LDRSH (Immediate)
        case 0b0001111111:
            return ARM_LDRSH_IMM(instruct);
        // ADD (Register)
        case 0b0010000000:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010000001:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010000010:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010000011:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010000100:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010000101:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010000110:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010000111:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010001000:
            return ARM_ADD_REG(instruct);
        // UMULL
        case 0b0010001001:
            return ARM_UMULL(instruct);
        // ADD (Register)
        case 0b0010001010:
            return ARM_ADD_REG(instruct);
        // STRH (register)
        case 0b0010001011:
            return ARM_STRH_REG(instruct);
        // ADD (Register)
        case 0b0010001100:
            return ARM_ADD_REG(instruct);
        // LDRD (Register)
        case 0b0010001101:
            return ARM_LDRD_REG(instruct);
        // ADD (Register)
        case 0b0010001110:
            return ARM_ADD_REG(instruct);
        // STRD (Register)
        case 0b0010001111:
            return ARM_STRD_REG(instruct);
        // ADD (Register)
        case 0b0010010000:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010010001:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010010010:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010010011:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010010100:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010010101:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010010110:
            return ARM_ADD_REG(instruct);
        // ADD (Register Shifted)
        case 0b0010010111:
            return ARM_ADD_REG_SHIFT(instruct);
        // ADD (Register)
        case 0b0010011000:
            return ARM_ADD_REG(instruct);
        // UMULL
        case 0b0010011001:
            return ARM_UMULL(instruct);
        // ADD (Register)
        case 0b0010011010:
            return ARM_ADD_REG(instruct);
        // LDRH (register)
        case 0b0010011011:
            return ARM_LDRH_REG(instruct);
        // ADD (Register)
        case 0b0010011100:
            return ARM_ADD_REG(instruct);
        // LDRSB (Register)
        case 0b0010011101:
            return ARM_LDRSB_REG(instruct);
        // ADD (Register)
        case 0b0010011110:
            return ARM_ADD_REG(instruct);
        // LDRSH (Register)
        case 0b0010011111:
            return ARM_LDRSH_REG(instruct);
        // ADC (Register)
        case 0b0010100000:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010100001:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010100010:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010100011:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010100100:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010100101:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010100110:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010100111:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010101000:
            return ARM_ADC_REG(instruct);
        // UMLAL
        case 0b0010101001:
            return ARM_UMLAL(instruct);
        // ADC (Register)
        case 0b0010101010:
            return ARM_ADC_REG(instruct);
        // STRH (register)
        case 0b0010101011:
            return ARM_STRH_REG(instruct);
        // ADC (Register)
        case 0b0010101100:
            return ARM_ADC_REG(instruct);
        // LDRD (Register)
        case 0b0010101101:
            return ARM_LDRD_REG(instruct);
        // ADC (Register)
        case 0b0010101110:
            return ARM_ADC_REG(instruct);
        // STRD (Register)
        case 0b0010101111:
            return ARM_STRD_REG(instruct);
        // ADC (Register)
        case 0b0010110000:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010110001:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010110010:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010110011:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010110100:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010110101:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010110110:
            return ARM_ADC_REG(instruct);
        // ADC (Register Shifted)
        case 0b0010110111:
            return ARM_ADC_REG_SHIFT(instruct);
        // ADC (Register)
        case 0b0010111000:
            return ARM_ADC_REG(instruct);
        // UMLAL
        case 0b0010111001:
            return ARM_UMLAL(instruct);
        // ADC (Register)
        case 0b0010111010:
            return ARM_ADC_REG(instruct);
        // LDRH (register)
        case 0b0010111011:
            return ARM_LDRH_REG(instruct);
        // ADC (Register)
        case 0b0010111100:
            return ARM_ADC_REG(instruct);
        // LDRSB (Register)
        case 0b0010111101:
            return ARM_LDRSB_REG(instruct);
        // ADC (Register)
        case 0b0010111110:
            return ARM_ADC_REG(instruct);
        // LDRSH (Register)
        case 0b0010111111:
            return ARM_LDRSH_REG(instruct);
        // SBC (Register)
        case 0b0011000000:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011000001:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011000010:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011000011:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011000100:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011000101:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011000110:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011000111:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011001000:
            return ARM_SBC_REG(instruct);
        // SMULL
        case 0b0011001001:
            return ARM_SMULL(instruct);
        // SBC (Register)
        case 0b0011001010:
            return ARM_SBC_REG(instruct);
        // STRH (Immediate)
        case 0b0011001011:
            return ARM_STRH_IMM(instruct);
        // SBC (Register)
        case 0b0011001100:
            return ARM_SBC_REG(instruct);
        // LDRD (Immediate)
        case 0b0011001101:
            return ARM_LDRD_IMM(instruct);
        // SBC (Register)
        case 0b0011001110:
            return ARM_SBC_REG(instruct);
        // STRD (Immediate)
        case 0b0011001111:
            return ARM_STRD_IMM(instruct);
        // SBC (Register)
        case 0b0011010000:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011010001:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011010010:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011010011:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011010100:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011010101:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011010110:
            return ARM_SBC_REG(instruct);
        // SBC (Register Shifted)
        case 0b0011010111:
            return ARM_SBC_REG_SHIFT(instruct);
        // SBC (Register)
        case 0b0011011000:
            return ARM_SBC_REG(instruct);
        // SMULL
        case 0b0011011001:
            return ARM_SMULL(instruct);
        // SBC (Register)
        case 0b0011011010:
            return ARM_SBC_REG(instruct);
        // LDRH (Immediate)
        case 0b0011011011:
            return ARM_LDRH_IMM(instruct);
        // SBC (Register)
        case 0b0011011100:
            return ARM_SBC_REG(instruct);
        // LDRSB (Immediate)
        case 0b0011011101:
            return ARM_LDRSB_IMM(instruct);
        // SBC (Register)
        case 0b0011011110:
            return ARM_SBC_REG(instruct);
        // LDRSH (Immediate)
        case 0b0011011111:
            return ARM_LDRSH_IMM(instruct);
        // RSC (Register)
        case 0b0011100000:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011100001:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011100010:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011100011:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011100100:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011100101:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011100110:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011100111:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011101000:
            return ARM_RSC_REG(instruct);
        // SMLAL
        case 0b0011101001:
            return ARM_SMLAL(instruct);
        // RSC (Register)
        case 0b0011101010:
            return ARM_RSC_REG(instruct);
        // STRH (Immediate)
        case 0b0011101011:
            return ARM_STRH_IMM(instruct);
        // RSC (Register)
        case 0b0011101100:
            return ARM_RSC_REG(instruct);
        // LDRD (Immediate)
        case 0b0011101101:
            return ARM_LDRD_IMM(instruct);
        // RSC (Register)
        case 0b0011101110:
            return ARM_RSC_REG(instruct);
        // STRD (Immediate)
        case 0b0011101111:
            return ARM_STRD_IMM(instruct);
        // RSC (Register)
        case 0b0011110000:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011110001:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011110010:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011110011:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011110100:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011110101:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011110110:
            return ARM_RSC_REG(instruct);
        // RSC (Register Shifted)
        case 0b0011110111:
            return ARM_RSC_REG_SHIFT(instruct);
        // RSC (Register)
        case 0b0011111000:
            return ARM_RSC_REG(instruct);
        // SMLAL
        case 0b0011111001:
            return ARM_SMLAL(instruct);
        // RSC (Register)
        case 0b0011111010:
            return ARM_RSC_REG(instruct);
        // LDRH (Immediate)
        case 0b0011111011:
            return ARM_LDRH_IMM(instruct);
        // RSC (Register)
        case 0b0011111100:
            return ARM_RSC_REG(instruct);
        // LDRSB (Immediate)
        case 0b0011111101:
            return ARM_LDRSB_IMM(instruct);
        // RSC (Register)
        case 0b0011111110:
            return ARM_RSC_REG(instruct);
        // LDRSH (Immediate)
        case 0b0011111111:
            return ARM_LDRSH_IMM(instruct);
        // MRS
        case 0b0100000000:
            return ARM_MRS(instruct);
        // Undefined
        case 0b0100000001:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0100000010:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0100000011:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0100000100:
            return ARM_UNDEFINED_INST(instruct);
        // QADD (ARM9 only)
        case 0b0100000101:
            return ARM_QADD(instruct);
        // Undefined
        case 0b0100000110:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0100000111:
            return ARM_UNDEFINED_INST(instruct);
        // SMLABB (ARM9 only)
        case 0b0100001000:
            return ARM_SMLABB(instruct);
        // SWP
        case 0b0100001001:
            return ARM_SWP(instruct);
        // SMLATB (ARM9 only)
        case 0b0100001010:
            return ARM_SMLATB(instruct);
        // STRH (register)
        case 0b0100001011:
            return ARM_STRH_REG(instruct);
        // SMLABT (ARM9 only)
        case 0b0100001100:
            return ARM_SMLABT(instruct);
        // LDRD (Register)
        case 0b0100001101:
            return ARM_LDRD_REG(instruct);
        // SMLATT (ARM9 only)
        case 0b0100001110:
            return ARM_SMLATT(instruct);
        // STRD (Register)
        case 0b0100001111:
            return ARM_STRD_REG(instruct);
        // TST (Register)
        case 0b0100010000:
            return ARM_TST_REG(instruct);
        // TST (Register Shifted)
        case 0b0100010001:
            return ARM_TST_REG_SHIFT(instruct);
        // TST (Register)
        case 0b0100010010:
            return ARM_TST_REG(instruct);
        // TST (Register Shifted)
        case 0b0100010011:
            return ARM_TST_REG_SHIFT(instruct);
        // TST (Register)
        case 0b0100010100:
            return ARM_TST_REG(instruct);
        // TST (Register Shifted)
        case 0b0100010101:
            return ARM_TST_REG_SHIFT(instruct);
        // TST (Register)
        case 0b0100010110:
            return ARM_TST_REG(instruct);
        // TST (Register Shifted)
        case 0b0100010111:
            return ARM_TST_REG_SHIFT(instruct);
        // TST (Register)
        case 0b0100011000:
            return ARM_TST_REG(instruct);
        // Undefined
        case 0b0100011001:
            return ARM_UNDEFINED_INST(instruct);
        // TST (Register)
        case 0b0100011010:
            return ARM_TST_REG(instruct);
        // LDRH (register)
        case 0b0100011011:
            return ARM_LDRH_REG(instruct);
        // TST (Register)
        case 0b0100011100:
            return ARM_TST_REG(instruct);
        // LDRSB (Register)
        case 0b0100011101:
            return ARM_LDRSB_REG(instruct);
        // TST (Register)
        case 0b0100011110:
            return ARM_TST_REG(instruct);
        // LDRSH (Register)
        case 0b0100011111:
            return ARM_LDRSH_REG(instruct);
        // MRS (Register)
        case 0b0100100000:
            return ARM_MSR_REG(instruct);
        // BX
        case 0b0100100001:
            return ARM_BX(instruct);
        // BXJ -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0100100010:
            return ARM_UNDEFINED_INST(instruct);
        // BLX (register)
        case 0b0100100011:
            return ARM_BLX_REG(instruct);
        // Undefined
        case 0b0100100100:
            return ARM_UNDEFINED_INST(instruct);
        // QSUB (ARM9 only)
        case 0b0100100101:
            return ARM_QSUB(instruct);
        // Undefined
        case 0b0100100110:
            return ARM_UNDEFINED_INST(instruct);
        // BKPT
        case 0b0100100111:
            return ARM_BKPT(instruct);
        // SMLAWB (ARM9 Only)
        case 0b0100101000:
            return ARM_SMLAWB(instruct);
        // Undefined
        case 0b0100101001:
            return ARM_UNDEFINED_INST(instruct);
        // SMULWB (ARM9 Only)
        case 0b0100101010:
            return ARM_SMULWB(instruct);
        // STRH (register)
        case 0b0100101011:
            return ARM_STRH_REG(instruct);
        // SMLAWT (ARM9 Only)
        case 0b0100101100:
            return ARM_SMLAWT(instruct);
        // LDRD (Register)
        case 0b0100101101:
            return ARM_LDRD_REG(instruct);
        // SMULWT (ARM9 Only)
        case 0b0100101110:
            return ARM_SMULWT(instruct);
        // STRD (Register)
        case 0b0100101111:
            return ARM_STRD_REG(instruct);
        // TEQ (Register)
        case 0b0100110000:
            return ARM_TEQ_REG(instruct);
        // TEQ (Register Shifted)
        case 0b0100110001:
            return ARM_TEQ_REG_SHIFT(instruct);
        // TEQ (Register)
        case 0b0100110010:
            return ARM_TEQ_REG(instruct);
        // TEQ (Register Shifted)
        case 0b0100110011:
            return ARM_TEQ_REG_SHIFT(instruct);
        // TEQ (Register)
        case 0b0100110100:
            return ARM_TEQ_REG(instruct);
        // TEQ (Register Shifted)
        case 0b0100110101:
            return ARM_TEQ_REG_SHIFT(instruct);
        // TEQ (Register)
        case 0b0100110110:
            return ARM_TEQ_REG(instruct);
        // TEQ (Register Shifted)
        case 0b0100110111:
            return ARM_TEQ_REG_SHIFT(instruct);
        // TEQ (Register)
        case 0b0100111000:
            return ARM_TEQ_REG(instruct);
        // Undefined
        case 0b0100111001:
            return ARM_UNDEFINED_INST(instruct);
        // TEQ (Register)
        case 0b0100111010:
            return ARM_TEQ_REG(instruct);
        // LDRH (register)
        case 0b0100111011:
            return ARM_LDRH_REG(instruct);
        // TEQ (Register)
        case 0b0100111100:
            return ARM_TEQ_REG(instruct);
        // LDRSB (Register)
        case 0b0100111101:
            return ARM_LDRSB_REG(instruct);
        // TEQ (Register)
        case 0b0100111110:
            return ARM_TEQ_REG(instruct);
        // LDRSH (Register)
        case 0b0100111111:
            return ARM_LDRSH_REG(instruct);
        // MRS
        case 0b0101000000:
            return ARM_MRS(instruct);
        // Undefined
        case 0b0101000001:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0101000010:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0101000011:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0101000100:
            return ARM_UNDEFINED_INST(instruct);
        // QDADD (ARM9 only)
        case 0b0101000101:
            return ARM_QDADD(instruct);
        // Undefined
        case 0b0101000110:
            return ARM_UNDEFINED_INST(instruct);
        // HVC -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0101000111:
            return ARM_UNDEFINED_INST(instruct);
        // SMLALBB (ARM9 Only)
        case 0b0101001000:
            return ARM_SMLALBB(instruct);
        // SWPB
        case 0b0101001001:
            return ARM_SWPB(instruct);
        // SMLALTB (ARM9 Only)
        case 0b0101001010:
            return ARM_SMLALTB(instruct);
        // STRH (Immediate)
        case 0b0101001011:
            return ARM_STRH_IMM(instruct);
        // SMLALBT (ARM9 Only)
        case 0b0101001100:
            return ARM_SMLALBT(instruct);
        // LDRD (Immediate)
        case 0b0101001101:
            return ARM_LDRD_IMM(instruct);
        // SMLALTT (ARM9 Only)
        case 0b0101001110:
            return ARM_SMLALTT(instruct);
        // STRD (Immediate)
        case 0b0101001111:
            return ARM_STRD_IMM(instruct);
        // CMP (Register)
        case 0b0101010000:
            return ARM_CMP_REG(instruct);
        // CMP (Register Shifted)
        case 0b0101010001:
            return ARM_CMP_REG_SHIFT(instruct);
        // CMP (Register)
        case 0b0101010010:
            return ARM_CMP_REG(instruct);
        // CMP (Register Shifted)
        case 0b0101010011:
            return ARM_CMP_REG_SHIFT(instruct);
        // CMP (Register)
        case 0b0101010100:
            return ARM_CMP_REG(instruct);
        // CMP (Register Shifted)
        case 0b0101010101:
            return ARM_CMP_REG_SHIFT(instruct);
        // CMP (Register)
        case 0b0101010110:
            return ARM_CMP_REG(instruct);
        // CMP (Register Shifted)
        case 0b0101010111:
            return ARM_CMP_REG_SHIFT(instruct);
        // CMP (Register)
        case 0b0101011000:
            return ARM_CMP_REG(instruct);
        // Undefined
        case 0b0101011001:
            return ARM_UNDEFINED_INST(instruct);
        // CMP (Register)
        case 0b0101011010:
            return ARM_CMP_REG(instruct);
        // LDRH (Immediate)
        case 0b0101011011:
            return ARM_LDRH_IMM(instruct);
        // CMP (Register)
        case 0b0101011100:
            return ARM_CMP_REG(instruct);
        // LDRSB (Immediate)
        case 0b0101011101:
            return ARM_LDRSB_IMM(instruct);
        // CMP (Register)
        case 0b0101011110:
            return ARM_CMP_REG(instruct);
        // LDRSH (Immediate)
        case 0b0101011111:
            return ARM_LDRSH_IMM(instruct);
        // MRS
        case 0b0101100000:
            return ARM_MRS(instruct);
        // CLZ
        case 0b0101100001:
            return ARM_CLZ(instruct);
        // Undefined
        case 0b0101100010:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0101100011:
            return ARM_UNDEFINED_INST(instruct);
        // Undefined
        case 0b0101100100:
            return ARM_UNDEFINED_INST(instruct);
        // QDSUB (ARM9 only)
        case 0b0101100101:
            return ARM_QDSUB(instruct);
        // ERET -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0101100110:
            return ARM_UNDEFINED_INST(instruct);
        // SMC / SMI -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0101100111:
            return ARM_UNDEFINED_INST(instruct);
        // SMULBB (ARM9 Only)
        case 0b0101101000:
            return ARM_SMULBB(instruct);
        // Undefined
        case 0b0101101001:
            return ARM_UNDEFINED_INST(instruct);
        // SMULTB (ARM9 Only)
        case 0b0101101010:
            return ARM_SMULTB(instruct);
        // STRH (Immediate)
        case 0b0101101011:
            return ARM_STRH_IMM(instruct);
        // SMULBT (ARM9 Only)
        case 0b0101101100:
            return ARM_SMULBT(instruct);
        // LDRD (Immediate)
        case 0b0101101101:
            return ARM_LDRD_IMM(instruct);
        // SMULTT (ARM9 Only)
        case 0b0101101110:
            return ARM_SMULTT(instruct);
        // STRD (Immediate)
        case 0b0101101111:
            return ARM_STRD_IMM(instruct);
        // CMN (Register)
        case 0b0101110000:
            return ARM_CMN_REG(instruct);
        // CMN (Register Shifted)
        case 0b0101110001:
            return ARM_CMN_REG_SHIFT(instruct);
        // CMN (Register)
        case 0b0101110010:
            return ARM_CMN_REG(instruct);
        // CMN (Register Shifted)
        case 0b0101110011:
            return ARM_CMN_REG_SHIFT(instruct);
        // CMN (Register)
        case 0b0101110100:
            return ARM_CMN_REG(instruct);
        // CMN (Register Shifted)
        case 0b0101110101:
            return ARM_CMN_REG_SHIFT(instruct);
        // CMN (Register)
        case 0b0101110110:
            return ARM_CMN_REG(instruct);
        // CMN (Register Shifted)
        case 0b0101110111:
            return ARM_CMN_REG_SHIFT(instruct);
        // CMN (Register)
        case 0b0101111000:
            return ARM_CMN_REG(instruct);
        // Undefined
        case 0b0101111001:
            return ARM_UNDEFINED_INST(instruct);
        // CMN (Register)
        case 0b0101111010:
            return ARM_CMN_REG(instruct);
        // LDRH (Immediate)
        case 0b0101111011:
            return ARM_LDRH_IMM(instruct);
        // CMN (Register)
        case 0b0101111100:
            return ARM_CMN_REG(instruct);
        // LDRSB (Immediate)
        case 0b0101111101:
            return ARM_LDRSB_IMM(instruct);
        // CMN (Register)
        case 0b0101111110:
            return ARM_CMN_REG(instruct);
        // LDRSH (Immediate)
        case 0b0101111111:
            return ARM_LDRSH_IMM(instruct);
        // ORR (Register)
        case 0b0110000000:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110000001:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110000010:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110000011:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110000100:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110000101:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110000110:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110000111:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110001000:
            return ARM_ORR_REG(instruct);
        // STREX -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0110001001:
            return ARM_UNDEFINED_INST(instruct);
        // ORR (Register)
        case 0b0110001010:
            return ARM_ORR_REG(instruct);
        // STRH (register)
        case 0b0110001011:
            return ARM_STRH_REG(instruct);
        // ORR (Register)
        case 0b0110001100:
            return ARM_ORR_REG(instruct);
        // LDRD (Register)
        case 0b0110001101:
            return ARM_LDRD_REG(instruct);
        // ORR (Register)
        case 0b0110001110:
            return ARM_ORR_REG(instruct);
        // STRD (Register)
        case 0b0110001111:
            return ARM_STRD_REG(instruct);
        // ORR (Register)
        case 0b0110010000:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110010001:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110010010:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110010011:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110010100:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110010101:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110010110:
            return ARM_ORR_REG(instruct);
        // ORR (Register Shifted)
        case 0b0110010111:
            return ARM_ORR_REG_SHIFT(instruct);
        // ORR (Register)
        case 0b0110011000:
            return ARM_ORR_REG(instruct);
        // LDREX -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0110011001:
            return ARM_UNDEFINED_INST(instruct);
        // ORR (Register)
        case 0b0110011010:
            return ARM_ORR_REG(instruct);
        // LDRH (register)
        case 0b0110011011:
            return ARM_LDRH_REG(instruct);
        // ORR (Register)
        case 0b0110011100:
            return ARM_ORR_REG(instruct);
        // LDRSB (Register)
        case 0b0110011101:
            return ARM_LDRSB_REG(instruct);
        // ORR (Register)
        case 0b0110011110:
            return ARM_ORR_REG(instruct);
        // LDRSH (Register)
        case 0b0110011111:
            return ARM_LDRSH_REG(instruct);
        // MOV (Register)
        case 0b0110100000:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110100001:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110100010:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110100011:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110100100:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110100101:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110100110:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110100111:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110101000:
            return ARM_MOV_REG(instruct);
        // STREXD -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0110101001:
            return ARM_UNDEFINED_INST(instruct);
        // MOV (Register)
        case 0b0110101010:
            return ARM_MOV_REG(instruct);
        // STRH (register)
        case 0b0110101011:
            return ARM_STRH_REG(instruct);
        // MOV (Register)
        case 0b0110101100:
            return ARM_MOV_REG(instruct);
        // LDRD (Register)
        case 0b0110101101:
            return ARM_LDRD_REG(instruct);
        // MOV (Register)
        case 0b0110101110:
            return ARM_MOV_REG(instruct);
        // STRD (Register)
        case 0b0110101111:
            return ARM_STRD_REG(instruct);
        // MOV (Register)
        case 0b0110110000:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110110001:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110110010:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110110011:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110110100:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110110101:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110110110:
            return ARM_MOV_REG(instruct);
        // MOV (Register Shifted)
        case 0b0110110111:
            return ARM_MOV_REG_SHIFT(instruct);
        // MOV (Register)
        case 0b0110111000:
            return ARM_MOV_REG(instruct);
        // LRDEXD -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0110111001:
            return ARM_UNDEFINED_INST(instruct);
        // MOV (Register)
        case 0b0110111010:
            return ARM_MOV_REG(instruct);
        // LDRH (register)
        case 0b0110111011:
            return ARM_LDRH_REG(instruct);
        // MOV (Register)
        case 0b0110111100:
            return ARM_MOV_REG(instruct);
        // LDRSB (Register)
        case 0b0110111101:
            return ARM_LDRSB_REG(instruct);
        // MOV (Register)
        case 0b0110111110:
            return ARM_MOV_REG(instruct);
        // LDRSH (Register)
        case 0b0110111111:
            return ARM_LDRSH_REG(instruct);
        // BIC (Register)
        case 0b0111000000:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111000001:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111000010:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111000011:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111000100:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111000101:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111000110:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111000111:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111001000:
            return ARM_BIC_REG(instruct);
        // STREXB -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0111001001:
            return ARM_UNDEFINED_INST(instruct);
        // BIC (Register)
        case 0b0111001010:
            return ARM_BIC_REG(instruct);
        // STRH (Immediate)
        case 0b0111001011:
            return ARM_STRH_IMM(instruct);
        // BIC (Register)
        case 0b0111001100:
            return ARM_BIC_REG(instruct);
        // LDRD (Immediate)
        case 0b0111001101:
            return ARM_LDRD_IMM(instruct);
        // BIC (Register)
        case 0b0111001110:
            return ARM_BIC_REG(instruct);
        // STRD (Immediate)
        case 0b0111001111:
            return ARM_STRD_IMM(instruct);
        // BIC (Register)
        case 0b0111010000:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111010001:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111010010:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111010011:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111010100:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111010101:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111010110:
            return ARM_BIC_REG(instruct);
        // BIC (Register Shifted)
        case 0b0111010111:
            return ARM_BIC_REG_SHIFT(instruct);
        // BIC (Register)
        case 0b0111011000:
            return ARM_BIC_REG(instruct);
        // LDREXB -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0111011001:
            return ARM_UNDEFINED_INST(instruct);
        // BIC (Register)
        case 0b0111011010:
            return ARM_BIC_REG(instruct);
        // LDRH (Immediate)
        case 0b0111011011:
            return ARM_LDRH_IMM(instruct);
        // BIC (Register)
        case 0b0111011100:
            return ARM_BIC_REG(instruct);
        // LDRSB (Immediate)
        case 0b0111011101:
            return ARM_LDRSB_IMM(instruct);
        // BIC (Register)
        case 0b0111011110:
            return ARM_BIC_REG(instruct);
        // LDRSH (Immediate)
        case 0b0111011111:
            return ARM_LDRSH_IMM(instruct);
        // MVN (Register)
        case 0b0111100000:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111100001:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111100010:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111100011:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111100100:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111100101:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111100110:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111100111:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111101000:
            return ARM_MVN_REG(instruct);
        // STREXH -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0111101001:
            return ARM_UNDEFINED_INST(instruct);
        // MVN (Register)
        case 0b0111101010:
            return ARM_MVN_REG(instruct);
        // STRH (Immediate)
        case 0b0111101011:
            return ARM_STRH_IMM(instruct);
        // MVN (Register)
        case 0b0111101100:
            return ARM_MVN_REG(instruct);
        // LDRD (Immediate)
        case 0b0111101101:
            return ARM_LDRD_IMM(instruct);
        // MVN (Register)
        case 0b0111101110:
            return ARM_MVN_REG(instruct);
        // STRD (Immediate)
        case 0b0111101111:
            return ARM_STRD_IMM(instruct);
        // MVN (Register)
        case 0b0111110000:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111110001:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111110010:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111110011:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111110100:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111110101:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111110110:
            return ARM_MVN_REG(instruct);
        // MVN (Register Shifted)
        case 0b0111110111:
            return ARM_MVN_REG_SHIFT(instruct);
        // MVN (Register)
        case 0b0111111000:
            return ARM_MVN_REG(instruct);
        // LDREXH -> Unsupported on DS's arm7 and arm9 cores.
        case 0b0111111001:
            return ARM_UNDEFINED_INST(instruct);
        // MVN (Register)
        case 0b0111111010:
            return ARM_MVN_REG(instruct);
        // LDRH (Immediate)
        case 0b0111111011:
            return ARM_LDRH_IMM(instruct);
        // MVN (Register)
        case 0b0111111100:
            return ARM_MVN_REG(instruct);
        // LDRSB (Immediate)
        case 0b0111111101:
            return ARM_LDRSB_IMM(instruct);
        // MVN (Register)
        case 0b0111111110:
            return ARM_MVN_REG(instruct);
        // LDRSH (Immediate)
        case 0b0111111111:
            return ARM_LDRSH_IMM(instruct);
        // AND (Immediate)
        case 0b1000000000:
        case 0b1000000001:
        case 0b1000000010:
        case 0b1000000011:
        case 0b1000000100:
        case 0b1000000101:
        case 0b1000000110:
        case 0b1000000111:
        case 0b1000001000:
        case 0b1000001001:
        case 0b1000001010:
        case 0b1000001011:
        case 0b1000001100:
        case 0b1000001101:
        case 0b1000001110:
        case 0b1000001111:
        case 0b1000010000:
        case 0b1000010001:
        case 0b1000010010:
        case 0b1000010011:
        case 0b1000010100:
        case 0b1000010101:
        case 0b1000010110:
        case 0b1000010111:
        case 0b1000011000:
        case 0b1000011001:
        case 0b1000011010:
        case 0b1000011011:
        case 0b1000011100:
        case 0b1000011101:
        case 0b1000011110:
        case 0b1000011111:
            return ARM_AND_IMM(instruct);
        // EOR (Immediate)
        case 0b1000100000:
        case 0b1000100001:
        case 0b1000100010:
        case 0b1000100011:
        case 0b1000100100:
        case 0b1000100101:
        case 0b1000100110:
        case 0b1000100111:
        case 0b1000101000:
        case 0b1000101001:
        case 0b1000101010:
        case 0b1000101011:
        case 0b1000101100:
        case 0b1000101101:
        case 0b1000101110:
        case 0b1000101111:
        case 0b1000110000:
        case 0b1000110001:
        case 0b1000110010:
        case 0b1000110011:
        case 0b1000110100:
        case 0b1000110101:
        case 0b1000110110:
        case 0b1000110111:
        case 0b1000111000:
        case 0b1000111001:
        case 0b1000111010:
        case 0b1000111011:
        case 0b1000111100:
        case 0b1000111101:
        case 0b1000111110:
        case 0b1000111111:
            return ARM_EOR_IMM(instruct);
        // SUB (Immediate)
        case 0b1001000000:
        case 0b1001000001:
        case 0b1001000010:
        case 0b1001000011:
        case 0b1001000100:
        case 0b1001000101:
        case 0b1001000110:
        case 0b1001000111:
        case 0b1001001000:
        case 0b1001001001:
        case 0b1001001010:
        case 0b1001001011:
        case 0b1001001100:
        case 0b1001001101:
        case 0b1001001110:
        case 0b1001001111:
        case 0b1001010000:
        case 0b1001010001:
        case 0b1001010010:
        case 0b1001010011:
        case 0b1001010100:
        case 0b1001010101:
        case 0b1001010110:
        case 0b1001010111:
        case 0b1001011000:
        case 0b1001011001:
        case 0b1001011010:
        case 0b1001011011:
        case 0b1001011100:
        case 0b1001011101:
        case 0b1001011110:
        case 0b1001011111:
            return ARM_SUB_IMM(instruct);
        // RSB (Immediate)
        case 0b1001100000:
        case 0b1001100001:
        case 0b1001100010:
        case 0b1001100011:
        case 0b1001100100:
        case 0b1001100101:
        case 0b1001100110:
        case 0b1001100111:
        case 0b1001101000:
        case 0b1001101001:
        case 0b1001101010:
        case 0b1001101011:
        case 0b1001101100:
        case 0b1001101101:
        case 0b1001101110:
        case 0b1001101111:
        case 0b1001110000:
        case 0b1001110001:
        case 0b1001110010:
        case 0b1001110011:
        case 0b1001110100:
        case 0b1001110101:
        case 0b1001110110:
        case 0b1001110111:
        case 0b1001111000:
        case 0b1001111001:
        case 0b1001111010:
        case 0b1001111011:
        case 0b1001111100:
        case 0b1001111101:
        case 0b1001111110:
        case 0b1001111111:
            return ARM_RSB_IMM(instruct);
        // ADD (Immediate)
        case 0b1010000000:
        case 0b1010000001:
        case 0b1010000010:
        case 0b1010000011:
        case 0b1010000100:
        case 0b1010000101:
        case 0b1010000110:
        case 0b1010000111:
        case 0b1010001000:
        case 0b1010001001:
        case 0b1010001010:
        case 0b1010001011:
        case 0b1010001100:
        case 0b1010001101:
        case 0b1010001110:
        case 0b1010001111:
        case 0b1010010000:
        case 0b1010010001:
        case 0b1010010010:
        case 0b1010010011:
        case 0b1010010100:
        case 0b1010010101:
        case 0b1010010110:
        case 0b1010010111:
        case 0b1010011000:
        case 0b1010011001:
        case 0b1010011010:
        case 0b1010011011:
        case 0b1010011100:
        case 0b1010011101:
        case 0b1010011110:
        case 0b1010011111:
            return ARM_ADD_IMM(instruct);
        // ADC (Immediate)
        case 0b1010100000:
        case 0b1010100001:
        case 0b1010100010:
        case 0b1010100011:
        case 0b1010100100:
        case 0b1010100101:
        case 0b1010100110:
        case 0b1010100111:
        case 0b1010101000:
        case 0b1010101001:
        case 0b1010101010:
        case 0b1010101011:
        case 0b1010101100:
        case 0b1010101101:
        case 0b1010101110:
        case 0b1010101111:
        case 0b1010110000:
        case 0b1010110001:
        case 0b1010110010:
        case 0b1010110011:
        case 0b1010110100:
        case 0b1010110101:
        case 0b1010110110:
        case 0b1010110111:
        case 0b1010111000:
        case 0b1010111001:
        case 0b1010111010:
        case 0b1010111011:
        case 0b1010111100:
        case 0b1010111101:
        case 0b1010111110:
        case 0b1010111111:
            return ARM_ADC_IMM(instruct);
        // SBC (Immediate)
        case 0b1011000000:
        case 0b1011000001:
        case 0b1011000010:
        case 0b1011000011:
        case 0b1011000100:
        case 0b1011000101:
        case 0b1011000110:
        case 0b1011000111:
        case 0b1011001000:
        case 0b1011001001:
        case 0b1011001010:
        case 0b1011001011:
        case 0b1011001100:
        case 0b1011001101:
        case 0b1011001110:
        case 0b1011001111:
        case 0b1011010000:
        case 0b1011010001:
        case 0b1011010010:
        case 0b1011010011:
        case 0b1011010100:
        case 0b1011010101:
        case 0b1011010110:
        case 0b1011010111:
        case 0b1011011000:
        case 0b1011011001:
        case 0b1011011010:
        case 0b1011011011:
        case 0b1011011100:
        case 0b1011011101:
        case 0b1011011110:
        case 0b1011011111:
            return ARM_SBC_IMM(instruct);
        // RSC (Immediate)
        case 0b1011100000:
        case 0b1011100001:
        case 0b1011100010:
        case 0b1011100011:
        case 0b1011100100:
        case 0b1011100101:
        case 0b1011100110:
        case 0b1011100111:
        case 0b1011101000:
        case 0b1011101001:
        case 0b1011101010:
        case 0b1011101011:
        case 0b1011101100:
        case 0b1011101101:
        case 0b1011101110:
        case 0b1011101111:
        case 0b1011110000:
        case 0b1011110001:
        case 0b1011110010:
        case 0b1011110011:
        case 0b1011110100:
        case 0b1011110101:
        case 0b1011110110:
        case 0b1011110111:
        case 0b1011111000:
        case 0b1011111001:
        case 0b1011111010:
        case 0b1011111011:
        case 0b1011111100:
        case 0b1011111101:
        case 0b1011111110:
        case 0b1011111111:
            return ARM_RSC_IMM(instruct);
        // MOV (16bit Immediate) -> Unsupported on DS's arm7 and arm9 cores.
        case 0b1100000000:
        case 0b1100000001:
        case 0b1100000010:
        case 0b1100000011:
        case 0b1100000100:
        case 0b1100000101:
        case 0b1100000110:
        case 0b1100000111:
        case 0b1100001000:
        case 0b1100001001:
        case 0b1100001010:
        case 0b1100001011:
        case 0b1100001100:
        case 0b1100001101:
        case 0b1100001110:
        case 0b1100001111:
            ARM_UNDEFINED_INST(instruct);
        // TES (Immediate)
        case 0b1100010000:
        case 0b1100010001:
        case 0b1100010010:
        case 0b1100010011:
        case 0b1100010100:
        case 0b1100010101:
        case 0b1100010110:
        case 0b1100010111:
        case 0b1100011000:
        case 0b1100011001:
        case 0b1100011010:
        case 0b1100011011:
        case 0b1100011100:
        case 0b1100011101:
        case 0b1100011110:
        case 0b1100011111:
            return ARM_TST_IMM(instruct);
        // MSR (Immediate).
        case 0b1100100000:
        case 0b1100100001:
        case 0b1100100010:
        case 0b1100100011:
        case 0b1100100100:
        case 0b1100100101:
        case 0b1100100110:
        case 0b1100100111:
        case 0b1100101000:
        case 0b1100101001:
        case 0b1100101010:
        case 0b1100101011:
        case 0b1100101100:
        case 0b1100101101:
        case 0b1100101110:
        case 0b1100101111:
            ARM_MSR_IMM(instruct);
        // TEQ (Immediate)
        case 0b1100110000:
        case 0b1100110001:
        case 0b1100110010:
        case 0b1100110011:
        case 0b1100110100:
        case 0b1100110101:
        case 0b1100110110:
        case 0b1100110111:
        case 0b1100111000:
        case 0b1100111001:
        case 0b1100111010:
        case 0b1100111011:
        case 0b1100111100:
        case 0b1100111101:
        case 0b1100111110:
        case 0b1100111111:
            return ARM_TEQ_IMM(instruct);
        // MOVT -> Unsupported on DS's arm7 and arm9 cores.
        case 0b1101000000:
        case 0b1101000001:
        case 0b1101000010:
        case 0b1101000011:
        case 0b1101000100:
        case 0b1101000101:
        case 0b1101000110:
        case 0b1101000111:
        case 0b1101001000:
        case 0b1101001001:
        case 0b1101001010:
        case 0b1101001011:
        case 0b1101001100:
        case 0b1101001101:
        case 0b1101001110:
        case 0b1101001111:
            ARM_UNDEFINED_INST(instruct);
        // CMP (Immediate)
        case 0b1101010000:
        case 0b1101010001:
        case 0b1101010010:
        case 0b1101010011:
        case 0b1101010100:
        case 0b1101010101:
        case 0b1101010110:
        case 0b1101010111:
        case 0b1101011000:
        case 0b1101011001:
        case 0b1101011010:
        case 0b1101011011:
        case 0b1101011100:
        case 0b1101011101:
        case 0b1101011110:
        case 0b1101011111:
            return ARM_CMP_IMM(instruct);
        // MSR (Immediate).
        case 0b1101100000:
        case 0b1101100001:
        case 0b1101100010:
        case 0b1101100011:
        case 0b1101100100:
        case 0b1101100101:
        case 0b1101100110:
        case 0b1101100111:
        case 0b1101101000:
        case 0b1101101001:
        case 0b1101101010:
        case 0b1101101011:
        case 0b1101101100:
        case 0b1101101101:
        case 0b1101101110:
        case 0b1101101111:
            ARM_MSR_IMM(instruct);
        // CMN (Immediate)
        case 0b1101110000:
        case 0b1101110001:
        case 0b1101110010:
        case 0b1101110011:
        case 0b1101110100:
        case 0b1101110101:
        case 0b1101110110:
        case 0b1101110111:
        case 0b1101111000:
        case 0b1101111001:
        case 0b1101111010:
        case 0b1101111011:
        case 0b1101111100:
        case 0b1101111101:
        case 0b1101111110:
        case 0b1101111111:
            return ARM_CMN_IMM(instruct);
        // ORR (Immediate)
        case 0b1110000000:
        case 0b1110000001:
        case 0b1110000010:
        case 0b1110000011:
        case 0b1110000100:
        case 0b1110000101:
        case 0b1110000110:
        case 0b1110000111:
        case 0b1110001000:
        case 0b1110001001:
        case 0b1110001010:
        case 0b1110001011:
        case 0b1110001100:
        case 0b1110001101:
        case 0b1110001110:
        case 0b1110001111:
        case 0b1110010000:
        case 0b1110010001:
        case 0b1110010010:
        case 0b1110010011:
        case 0b1110010100:
        case 0b1110010101:
        case 0b1110010110:
        case 0b1110010111:
        case 0b1110011000:
        case 0b1110011001:
        case 0b1110011010:
        case 0b1110011011:
        case 0b1110011100:
        case 0b1110011101:
        case 0b1110011110:
        case 0b1110011111:
            return ARM_ORR_IMM(instruct);
        // MOV (Immediate)
        case 0b1110100000:
        case 0b1110100001:
        case 0b1110100010:
        case 0b1110100011:
        case 0b1110100100:
        case 0b1110100101:
        case 0b1110100110:
        case 0b1110100111:
        case 0b1110101000:
        case 0b1110101001:
        case 0b1110101010:
        case 0b1110101011:
        case 0b1110101100:
        case 0b1110101101:
        case 0b1110101110:
        case 0b1110101111:
        case 0b1110110000:
        case 0b1110110001:
        case 0b1110110010:
        case 0b1110110011:
        case 0b1110110100:
        case 0b1110110101:
        case 0b1110110110:
        case 0b1110110111:
        case 0b1110111000:
        case 0b1110111001:
        case 0b1110111010:
        case 0b1110111011:
        case 0b1110111100:
        case 0b1110111101:
        case 0b1110111110:
        case 0b1110111111:
            return ARM_MOV_IMM(instruct);
        // BIC (Immediate)
        case 0b1111000000:
        case 0b1111000001:
        case 0b1111000010:
        case 0b1111000011:
        case 0b1111000100:
        case 0b1111000101:
        case 0b1111000110:
        case 0b1111000111:
        case 0b1111001000:
        case 0b1111001001:
        case 0b1111001010:
        case 0b1111001011:
        case 0b1111001100:
        case 0b1111001101:
        case 0b1111001110:
        case 0b1111001111:
        case 0b1111010000:
        case 0b1111010001:
        case 0b1111010010:
        case 0b1111010011:
        case 0b1111010100:
        case 0b1111010101:
        case 0b1111010110:
        case 0b1111010111:
        case 0b1111011000:
        case 0b1111011001:
        case 0b1111011010:
        case 0b1111011011:
        case 0b1111011100:
        case 0b1111011101:
        case 0b1111011110:
        case 0b1111011111:
            return ARM_BIC_IMM(instruct);
        // MVN (Immediate)
        case 0b1111100000:
        case 0b1111100001:
        case 0b1111100010:
        case 0b1111100011:
        case 0b1111100100:
        case 0b1111100101:
        case 0b1111100110:
        case 0b1111100111:
        case 0b1111101000:
        case 0b1111101001:
        case 0b1111101010:
        case 0b1111101011:
        case 0b1111101100:
        case 0b1111101101:
        case 0b1111101110:
        case 0b1111101111:
        case 0b1111110000:
        case 0b1111110001:
        case 0b1111110010:
        case 0b1111110011:
        case 0b1111110100:
        case 0b1111110101:
        case 0b1111110110:
        case 0b1111110111:
        case 0b1111111000:
        case 0b1111111001:
        case 0b1111111010:
        case 0b1111111011:
        case 0b1111111100:
        case 0b1111111101:
        case 0b1111111110:
        case 0b1111111111:
            return ARM_MVN_IMM(instruct);
        default:
            break;
    }
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Data-processing
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--register-shifted-register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--immediate-?lang=en
// ==================================================================================================
// ==================================================================================================
// AND
// ==================================================================================================
cycles ARM::ARM_AND(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing AND");
    uint32_t result = opp1 & opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_AND_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_AND(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_AND_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_AND(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_AND_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_AND(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// EOR
// ==================================================================================================
cycles ARM::ARM_EOR(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing EOR");
    uint32_t result = opp1 ^ opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_EOR_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_EOR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_EOR_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_EOR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_EOR_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_EOR(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// SUB
// ==================================================================================================
cycles ARM::ARM_SUB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags) {
    LogDebug("Executing SUB");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, 1);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_SUB_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SUB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_SUB_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SUB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_SUB_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_SUB(Rd, operand1, operand2, S);
}
// ==================================================================================================
// RSB
// ==================================================================================================
cycles ARM::ARM_RSB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags) {
    LogDebug("Executing RSB");
    u32WithCarryAndOverflow result = ARMAddWithCarry(~opp1, opp2, 1);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_RSB_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_RSB_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_RSB_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_RSB(Rd, operand1, operand2, S);
}
// ==================================================================================================
// ADD
// ==================================================================================================
cycles ARM::ARM_ADD(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags) {
    LogDebug("Executing ADD");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, 0);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_ADD_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADD(Rd, operand1, operand2, S);
}
cycles ARM::ARM_ADD_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADD(Rd, operand1, operand2, S);
}
cycles ARM::ARM_ADD_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_ADD(Rd, operand1, operand2, S);
}
// ==================================================================================================
// ADC
// ==================================================================================================
cycles ARM::ARM_ADC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing ADC");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, carry);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_ADC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ADC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ADC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_ADC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// SBC
// ==================================================================================================
cycles ARM::ARM_SBC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing SBC");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, carry);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_SBC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SBC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_SBC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SBC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_SBC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_SBC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// RSC
// ==================================================================================================
cycles ARM::ARM_RSC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing RSC");
    u32WithCarryAndOverflow result = ARMAddWithCarry(~opp1, opp2, carry);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_RSC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_RSC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_RSC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_RSC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// TST
// ==================================================================================================
cycles ARM::ARM_TST(uint32_t opp1, uint32_t opp2, bool carry) {
    LogDebug("Executing TST");
    LogDebug("Updating CPU flags...");
    uint32_t result = opp1 & opp2;
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    writeBit(cpsr, carry, C_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_TST_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TST(operand1, operand2, carry);
}
cycles ARM::ARM_TST_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TST(operand1, operand2, carry);
}
cycles ARM::ARM_TST_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_TST(operand1, operand2, carry);
}
// ==================================================================================================
// TEQ
// ==================================================================================================
cycles ARM::ARM_TEQ(uint32_t opp1, uint32_t opp2, bool carry) {
    LogDebug("Executing TEQ");
    LogDebug("Updating CPU flags...");
    uint32_t result = opp1 ^ opp2;
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    writeBit(cpsr, carry, C_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_TEQ_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TEQ(operand1, operand2, carry);
}
cycles ARM::ARM_TEQ_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TEQ(operand1, operand2, carry);
}
cycles ARM::ARM_TEQ_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_TEQ(operand1, operand2, carry);
}
// ==================================================================================================
// CMP
// ==================================================================================================
cycles ARM::ARM_CMP(uint32_t opp1, uint32_t opp2) {
    LogDebug("Executing CMP");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, 1);
    LogDebug("Updating CPU flags...");
    writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
    writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
    writeBit(cpsr, result.carry, C_FLAG);
    writeBit(cpsr, result.overflow, V_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_CMP_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMP(operand1, operand2);
}
cycles ARM::ARM_CMP_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMP(operand1, operand2);
}
cycles ARM::ARM_CMP_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_CMP(operand1, operand2);
}
// ==================================================================================================
// CMN
// ==================================================================================================
cycles ARM::ARM_CMN(uint32_t opp1, uint32_t opp2) {
    LogDebug("Executing CMN");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, 0);
    LogDebug("Updating CPU flags...");
    writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
    writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
    writeBit(cpsr, result.carry, C_FLAG);
    writeBit(cpsr, result.overflow, V_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_CMN_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMN(operand1, operand2);
}
cycles ARM::ARM_CMN_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMN(operand1, operand2);
}
cycles ARM::ARM_CMN_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_CMN(operand1, operand2);
}
// ==================================================================================================
// ORR
// ==================================================================================================
cycles ARM::ARM_ORR(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing ORR");
    uint32_t result = opp1 | opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_ORR_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ORR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ORR_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ORR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ORR_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_ORR(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// MOV
// ==================================================================================================
cycles ARM::ARM_MOV(uint32_t desReg, uint32_t srcValue, bool carry, bool setFlags) {
    LogDebug("Executing MOV");
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(srcValue, 31), N_FLAG);
        writeBit(cpsr, srcValue == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = srcValue;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_MOV_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MOV(Rd, operand, carry, S);
}
cycles ARM::ARM_MOV_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MOV(Rd, operand, carry, S);
}
cycles ARM::ARM_MOV_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_MOV(Rd, operand, carry, S);
}
// ==================================================================================================
// BIC
// ==================================================================================================
cycles ARM::ARM_BIC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing BIC");
    uint32_t result = opp1 & ~opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_BIC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_BIC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_BIC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_BIC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_BIC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_BIC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// MVN
// ==================================================================================================
cycles ARM::ARM_MVN(uint32_t desReg, uint32_t opp1, bool carry, bool setFlags) {
    LogDebug("Executing MVN");
    uint32_t result = ~opp1;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_MVN_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MVN(Rd, operand, carry, S);
}
cycles ARM::ARM_MVN_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.;
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MVN(Rd, operand, carry, S);
}
cycles ARM::ARM_MVN_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_MVN(Rd, operand, carry, S);
}
// ==================================================================================================
// Multiply and Multiply Accumulate
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Multiply-and-multiply-accumulate?lang=en
// ==================================================================================================
cycles ARM::ARM_MUL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rm];
    uint32_t result = operand1 * operand2;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_MLA(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rs = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rm];
    uint32_t addend = *activeRegs[Rs];
    uint32_t result = operand1 * operand2 + addend;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_UMAAL(uint32_t instruct) {
    // Note: Not supported by the DS's arm7 or arm9 cores.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint64_t operand1 = *activeRegs[Rn];
    uint64_t operand2 = *activeRegs[Rm];
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    uint64_t result = operand1 * operand2 + addend1 + addend2;
    // Save the result.
    LogDebug(result);
    *activeRegs[RdHi] = (uint32_t)(result >> 32);
    *activeRegs[RdLo] = (uint32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_MLS(uint32_t instruct) {
    // Note: Not supported by the DS's arm7 or arm9 cores.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rs = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rm];
    uint32_t addend = *activeRegs[Rs];
    uint32_t result = addend - operand1 * operand2;
    // Save the result.
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_UMULL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint64_t operand1 = *activeRegs[Rn];
    uint64_t operand2 = *activeRegs[Rm];
    uint64_t result = operand1 * operand2;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (uint32_t)(result >> 32);
    *activeRegs[RdLo] = (uint32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_UMLAL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint64_t operand1 = *activeRegs[Rn];
    uint64_t operand2 = *activeRegs[Rm];
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    uint64_t result = operand1 * operand2 + (addend1 << 32 | addend2);
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (uint32_t)(result >> 32);
    *activeRegs[RdLo] = (uint32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_SMULL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    int64_t result = operand1 * operand2;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (int32_t)(result >> 32);
    *activeRegs[RdLo] = (int32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_SMLAL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    int64_t result = operand1 * operand2 + addend;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (int32_t)(result >> 32);
    *activeRegs[RdLo] = (int32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
// ==================================================================================================
// Halfword multiply and multiply accumulate
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Halfword-multiply-and-multiply-accumulate?lang=en
// ==================================================================================================
// ==================================================================================================
// SMLA
// ==================================================================================================
cycles ARM::ARM_SMLABB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLABT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLATB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLATT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLAWB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    int64_t addend = ((int32_t)(*activeRegs[Ra]) << 16);
    // Compute the result.
    int64_t result = operand1 * operand2 + addend;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_SMLAWT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int64_t addend = ((int32_t)(*activeRegs[Ra]) << 16);
    // Compute the result.
    int64_t result = operand1 * operand2 + addend;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_SMLA(uint32_t desReg, int32_t opp1, int32_t opp2, int32_t addend) {
    // Note: Only supported by the DS's arm9 core.
    int32_t result = opp1 * opp2 + addend;
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;
}
// ==================================================================================================
// SMUL
// ==================================================================================================
cycles ARM::ARM_SMULBB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULBT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULTB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULTT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULWB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int16_t)(*activeRegs[Rn]);
    int64_t operand2 = (int16_t)(*activeRegs[Rm]);
    int64_t result = operand1 * operand2;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_SMULWT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int16_t)(*activeRegs[Rn]);
    int64_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int64_t result = operand1 * operand2;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_SMUL(uint32_t desReg, int32_t opp1, int32_t opp2) {
    // Note: Only supported by the DS's arm9 core.
    int32_t result = opp1 * opp2;
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;
}
// ==================================================================================================
// SMLAL
// ==================================================================================================
cycles ARM::ARM_SMLALBB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLALBT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm] >> 16);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLALTB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn] >> 16);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLALTT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn] >> 16);
    int64_t operand2 = (int32_t)(*activeRegs[Rm] >> 16);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLAL(uint32_t desRegLow, uint32_t desRegHigh, int64_t opp1, int64_t opp2,
                      int64_t addend) {
    // Note: Only supported by the DS's arm9 core.
    int64_t result = opp1 * opp2 + addend;
    *activeRegs[desRegHigh] = (int32_t)(result >> 32);
    *activeRegs[desRegLow] = (int32_t)(result);
    fixupIfTargetingPC(desRegHigh);
    fixupIfTargetingPC(desRegLow);
    return 1;
}
// ==================================================================================================
// SWP
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SWP--SWPB?lang=en
// ==================================================================================================
cycles ARM::ARM_SWP(uint32_t instruct) {
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rt2 = readBits(instruct, 0, 3);
    busPayload dataRead = readBus(*activeRegs[Rn]);
    busPayload dataWrite = writeBus(*activeRegs[Rn], *activeRegs[Rt2]);
    *activeRegs[Rt] = dataRead.data;
    fixupIfTargetingPC(Rt);
    return 1;
}
// ==================================================================================================
cycles ARM::ARM_SWPB(uint32_t instruct) {
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rt2 = readBits(instruct, 0, 3);
    busPayload dataRead = readBus(*activeRegs[Rn], 8);
    busPayload dataWrite = writeBus(*activeRegs[Rn], (*activeRegs[Rt2] & 0xFF), 8);
    *activeRegs[Rt] = (dataRead.data & 0xFF);
    fixupIfTargetingPC(Rt);
    return 1;
}
// ==================================================================================================
// MRS
// ==================================================================================================
cycles ARM::ARM_MRS(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// MSR
// ==================================================================================================
cycles ARM::ARM_MSR_REG(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
cycles ARM::ARM_MSR_IMM(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// CLZ
// ==================================================================================================
cycles ARM::ARM_CLZ(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// QUAD
// ==================================================================================================
cycles ARM::ARM_QADD(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// QSUB
// ==================================================================================================
cycles ARM::ARM_QSUB(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// QDADD
// ==================================================================================================
cycles ARM::ARM_QDADD(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// QDSUB
// ==================================================================================================
cycles ARM::ARM_QDSUB(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
