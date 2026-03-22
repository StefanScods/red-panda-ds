#include "instructionDisassembler.h"

#include "cpu.h"

// Control print statements.
#define LOG_LEVEL 1
#include <format>

#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
std::string InstructionDisassembly::toString() {
    std::string result = opcode + conditionCode;
    if (!destination.empty()) {
        result += " ";
        result += destination;
    }
    if (!operand1.empty()) {
        if (!destination.empty()) {
            result += ",";
        }
        result += " ";
        result += operand1;
    }
    if (!operand2.empty()) {
        result += ",";
        result += " ";
        result += operand2;
    }
    return result;
}
// ==================================================================================================
std::string getImmString(uint32_t val, bool useHex) {
    std::string imm = "#";
    if (useHex) {
        imm += std::format("0x{:08X}", val);
    } else {
        imm += std::format("{}", val);
    }
    return imm;
}
std::string getImmStringSigned(int32_t val, bool useHex) {
    std::string imm = "#";
    if (useHex) {
        imm += std::format("0x{:08X}", val);
    } else {
        imm += std::format("{}", val);
    }
    return imm;
}
// ==================================================================================================
std::string getRegListString(uint32_t regList) {
    if (regList == 0) return "{}";
    std::string result = "{";
    bool rangeStarted = false;
    bool replaceLast = false;

    for (uint32_t i = 0; i < SP_REGISTER_NUM; i++) {
        bool includeReg = ((regList >> i) & 0b1);
        bool includeNextReg = ((regList >> i + 1) & 0b1);
        bool includeNextNextReg = ((regList >> i + 2) & 0b1);
        bool nextRegSpecial = i + 1 >= SP_REGISTER_NUM;
        bool nextNextRegSpecial = i + 2 >= SP_REGISTER_NUM;
        if (!includeReg) continue;
        if (!rangeStarted) {
            result += g_regNames[i];
            if (includeNextReg && !nextRegSpecial && includeNextNextReg && !nextNextRegSpecial) {
                result += "-";
                rangeStarted = true;
            } else {
                result += ",";
            }
        } else {
            if (!includeNextReg || nextRegSpecial) {
                rangeStarted = false;
                result += g_regNames[i] + ",";
            }
        }
    }
    for (uint32_t i = SP_REGISTER_NUM; i <= PC_REGISTER_NUM; i++) {
        bool includeReg = ((regList >> i) & 0b1);
        if (!includeReg) continue;
        result += g_regNames[i] + ",";
    }
    result.back() = '}';
    return result;
}
// ==================================================================================================
// ARM Disassemble
// ==================================================================================================
InstructionDisassembly dissembleARMInstruction(uint32_t instruction, bool useHex) {
    InstructionDisassembly result;
    result.opcode = UNDEFINED_INSTRUCTION_OPCODE;
    // Extract the condition code.
    result.conditionCode = ConditionMnemonics::toString(
        ConditionMnemonics::ConditionMnemonics(readBits(instruction, 28, 31)));
    // Begin decoding the rest of the instruction.
    switch (readBits(instruction, 26, 27)) {
        // Data-processing and miscellaneous instructions.
        case 0b00: {
            dissembleARMInstruction_data(result, instruction, useHex);
            break;
        }
        // Load/store word and unsigned byte.
        case 0b01:
            dissembleARMInstruction_loadStore(result, instruction, useHex);
            break;
        // 	Branch, branch with link, and block data transfer.
        case 0b10: {
            dissembleARMInstruction_branchAndBlockTransfer(result, instruction, useHex);
            break;
        }
        // Coprocessor instructions, and Supervisor Call
        case 0b11: {
            dissembleARMInstruction_coProc(result, instruction, useHex);
            break;
        }
        default:
            break;
    }
    // Clear the result if undefined.
    if (result.opcode == UNDEFINED_INSTRUCTION_OPCODE) {
        result = InstructionDisassembly();
        result.opcode = UNDEFINED_INSTRUCTION_OPCODE;
    }
    result.rawData = instruction;
    return result;
}
// ==================================================================================================
void dissembleARMInstruction_data(InstructionDisassembly& result, uint32_t instruction,
                                  bool useHex) {
    if (!readBit(instruction, 25)) {
        if (readBits(instruction, 4, 7) == 0b1001) {
            if (readBit(instruction, 24)) {
                dissembleARMInstruction_data_syncronizationPrimatives(result, instruction, useHex);
            } else {
                dissembleARMInstruction_data_multAndMultAccumulate(result, instruction, useHex);
            }
        } else if ((readBits(instruction, 4, 7) & 0b1001) == 0b1001) {
            dissembleARMInstruction_data_extraLoadAndStore(result, instruction, useHex);
        } else {
            switch (readBits(instruction, 20, 24)) {
                case 0b00000:
                case 0b00001:
                case 0b00010:
                case 0b00011:
                case 0b00100:
                case 0b00101:
                case 0b00110:
                case 0b00111:
                case 0b01000:
                case 0b01001:
                case 0b01010:
                case 0b01011:
                case 0b01100:
                case 0b01101:
                case 0b01110:
                case 0b01111:
                    dissembleARMInstruction_data_dataProcessing(result, instruction, useHex);
                    break;
                case 0b10000:
                    dissembleARMInstruction_data_misc_halfwordMultiply(result, instruction, useHex);
                    break;
                case 0b10001:
                    dissembleARMInstruction_data_dataProcessing(result, instruction, useHex);
                    break;
                case 0b10010:
                    dissembleARMInstruction_data_misc_halfwordMultiply(result, instruction, useHex);
                    break;
                case 0b10011:
                    dissembleARMInstruction_data_dataProcessing(result, instruction, useHex);
                    break;
                case 0b10100:
                    dissembleARMInstruction_data_misc_halfwordMultiply(result, instruction, useHex);
                    break;
                case 0b10101:
                    dissembleARMInstruction_data_dataProcessing(result, instruction, useHex);
                    break;
                case 0b10110:
                    dissembleARMInstruction_data_misc_halfwordMultiply(result, instruction, useHex);
                    break;
                case 0b10111:
                case 0b11000:
                case 0b11001:
                case 0b11010:
                case 0b11011:
                case 0b11100:
                case 0b11101:
                case 0b11110:
                case 0b11111:
                    dissembleARMInstruction_data_dataProcessing(result, instruction, useHex);
                    break;
                default:
                    break;
            }
        }
    } else {
        switch (readBits(instruction, 20, 24)) {
            case 0b10000:
                result.opcode = "MOVW";
                result.comment = "Unsupported";
                break;
            case 0b10100:
                result.opcode = "MOVT";
                result.comment = "Unsupported";
                break;
            case 0b10010:
            case 0b10110:
                dissembleARMInstruction_data_msrAndHints(result, instruction, useHex);
                break;
            default:
                dissembleARMInstruction_data_dataProcessing(result, instruction, useHex);
                break;
        }
    }
}
// ==================================================================================================
void dissembleARMInstruction_data_dataProcessing(InstructionDisassembly& result,
                                                 uint32_t instruction, bool useHex) {
    // Process the second operand.
    if (readBit(instruction, 25)) {
        // Intermediate.
        uint32_t imm12 = readBits(instruction, 0, 11);
        u32AndBool immDecoded = ARMExpandImm_C(imm12, 0);
        result.operand2 = getImmString(immDecoded.data_u32, useHex);
    } else {
        if (readBit(instruction, 4)) {
            // Register shifted register.
            result.operand2 = g_regNames[readBits(instruction, 0, 3)] + ", ";
            switch (readBits(instruction, 5, 6)) {
                case 0b00:
                    result.operand2 += "LSL ";
                    break;
                case 0b01:
                    result.operand2 += "LSR ";
                    break;
                case 0b10:
                    result.operand2 += "ASR ";
                    break;
                case 0b11:
                    result.operand2 += "ROR ";
                    break;
                default:
                    break;
            }
            result.operand2 += g_regNames[readBits(instruction, 8, 11)];
        } else {
            result.operand2 = g_regNames[readBits(instruction, 0, 3)];
            uint32_t imm5 = readBits(instruction, 7, 11);
            if (imm5 > 0) {
                switch (readBits(instruction, 5, 6)) {
                    case 0b00:
                        result.operand2 += ", LSL ";
                        break;
                    case 0b01:
                        result.operand2 += ", LSR ";
                        break;
                    case 0b10:
                        result.operand2 += ", ASR ";
                        break;
                    case 0b11:
                        result.operand2 += ", ROR ";
                        break;
                    default:
                        break;
                }
                result.operand2 += getImmString(imm5, useHex);
            } else if (readBits(instruction, 5, 6) == 0b11) {
                result.operand2 += ", RRX";
            }
        }
    }
    result.operand1 = g_regNames[readBits(instruction, 16, 19)];
    result.destination = g_regNames[readBits(instruction, 12, 15)];
    switch (readBits(instruction, 20, 24)) {
        case 0b00000:
            result.opcode = "AND";
            break;
        case 0b00001:
            result.opcode = "ANDS";
            break;
        case 0b00010:
            result.opcode = "EOR";
            break;
        case 0b00011:
            result.opcode = "EORS";
            break;
        case 0b00100:
            result.opcode = "SUB";
            break;
        case 0b00101:
            result.opcode = "SUBS";
            break;
        case 0b00110:
            result.opcode = "RSB";
            break;
        case 0b00111:
            result.opcode = "RSBS";
            break;
        case 0b01000:
            result.opcode = "ADD";
            break;
        case 0b01001:
            result.opcode = "ADDS";
            break;
        case 0b01010:
            result.opcode = "ADC";
            break;
        case 0b01011:
            result.opcode = "ADCS";
            break;
        case 0b01100:
            result.opcode = "SBC";
            break;
        case 0b01101:
            result.opcode = "SBCS";
            break;
        case 0b01110:
            result.opcode = "RSC";
            break;
        case 0b01111:
            result.opcode = "RSCS";
            break;
        case 0b10001:
            result.opcode = "TST";
            result.destination = "";
            break;
        case 0b10011:
            result.opcode = "TEQ";
            result.destination = "";
            break;
        case 0b10101:
            result.opcode = "CMP";
            result.destination = "";
            break;
        case 0b10111:
            result.opcode = "CMN";
            result.destination = "";
            break;
        case 0b11000:
            result.opcode = "ORR";
            break;
        case 0b11001:
            result.opcode = "ORRS";
            break;
        case 0b11010:
            result.opcode = "MOV";
            result.operand1 = "";
            break;
        case 0b11011:
            result.opcode = "MOVS";
            result.operand1 = "";
            break;
        case 0b11100:
            result.opcode = "BIC";
            break;
        case 0b11101:
            result.opcode = "BICS";
            break;
        case 0b11110:
            result.opcode = "MVN";
            result.operand1 = "";
            break;
        case 0b11111:
            result.opcode = "MVNS";
            result.operand1 = "";
            break;
        default:
            break;
    }
}
// ==================================================================================================
void dissembleARMInstruction_data_multAndMultAccumulate(InstructionDisassembly& result,
                                                        uint32_t instruction, bool useHex) {
    result.destination = g_regNames[readBits(instruction, 16, 19)];
    result.operand1 = g_regNames[readBits(instruction, 0, 3)];
    result.operand2 = g_regNames[readBits(instruction, 8, 11)];
    switch (readBits(instruction, 21, 23)) {
        case 0b000:
            result.opcode = "MUL";
            break;
        case 0b001:
            result.opcode = "MLA";
            result.operand2 += ", " + g_regNames[readBits(instruction, 12, 15)];
            break;
        case 0b100:
            result.opcode = "UMULL";
            result.destination =
                g_regNames[readBits(instruction, 12, 15)] + ", " + result.destination;
            break;
        case 0b101:
            result.opcode = "UMLAL";
            result.destination =
                g_regNames[readBits(instruction, 12, 15)] + ", " + result.destination;
            break;
        case 0b110:
            result.opcode = "SMULL";
            result.destination =
                g_regNames[readBits(instruction, 12, 15)] + ", " + result.destination;
            break;
        case 0b111:
            result.opcode = "SMLAL";
            result.destination =
                g_regNames[readBits(instruction, 12, 15)] + ", " + result.destination;
            break;
        default:
            break;
    }
    if (readBit(instruction, 20)) {
        result.opcode += "S";
    }
}
// ==================================================================================================
void dissembleARMInstruction_data_syncronizationPrimatives(InstructionDisassembly& result,
                                                           uint32_t instruction, bool useHex) {
    if ((readBits(instruction, 20, 23) & 01011) != 0b0000) return;
    result.opcode = "SWP";
    if (readBit(instruction, 22)) result.opcode += "B";
    result.destination = g_regNames[readBits(instruction, 12, 15)];
    result.operand1 = g_regNames[readBits(instruction, 0, 3)];
    result.operand2 = "[" + g_regNames[readBits(instruction, 16, 19)] + "]";
}
// ==================================================================================================
void dissembleARMInstruction_data_extraLoadAndStore(InstructionDisassembly& result,
                                                    uint32_t instruction, bool useHex) {
    result.destination = g_regNames[readBits(instruction, 12, 15)];
    result.operand2 = "[" + g_regNames[readBits(instruction, 16, 19)];
    std::string offset = "";
    bool includeOffset = true;
    if (readBit(instruction, 22)) {
        uint32_t imm = (readBits(instruction, 8, 11) << 4) | readBits(instruction, 0, 3);
        includeOffset = imm != 0;
        offset += getImmString(imm, useHex);
        if (!readBit(instruction, 23)) {
            offset.insert(1, 1, '-');
        }
    } else {
        offset += (readBit(instruction, 23) ? "" : "-") + g_regNames[readBits(instruction, 0, 3)];
    }
    bool privedged = false;
    if (readBit(instruction, 24)) {
        if (includeOffset) result.operand2 += ", " + offset;
        result.operand2 += "]";
        if (readBit(instruction, 21)) result.operand2 += "!";
    } else {
        result.operand2 += "]";
        if (includeOffset) result.operand2 += ", " + offset;
        privedged = readBit(instruction, 21);
    }
    uint32_t opcode = (readBit(instruction, 20) << 2) | readBits(instruction, 5, 6);
    switch (opcode) {
        case 0b001:
            result.opcode = "STRH";
            break;
        case 0b010:
            result.opcode = "LDRD";
            result.operand1 = g_regNames[readBits(instruction, 12, 15) + 1];
            break;
        case 0b011:
            result.opcode = "STRD";
            result.operand1 = g_regNames[readBits(instruction, 12, 15) + 1];
            break;
        case 0b101:
            result.opcode = "LDRH";
            break;
        case 0b110:
            result.opcode = "LDRSB";
            break;
        case 0b111:
            result.opcode = "LDRSH";
            break;
        default:
            break;
    }
    if (privedged) {
        result.opcode += "T";
        result.comment = "Unsupported";
    }
}
// ==================================================================================================

void dissembleARMInstruction_data_misc_halfwordMultiply(InstructionDisassembly& result,
                                                        uint32_t instruction, bool useHex) {
    if (readBit(instruction, 7)) {
        dissembleARMInstruction_data_halfwordMultiply(result, instruction, useHex);
    } else {
        dissembleARMInstruction_data_misc(result, instruction, useHex);
    }
}
// ==================================================================================================
void dissembleARMInstruction_data_misc(InstructionDisassembly& result, uint32_t instruction,
                                       bool useHex) {
    switch (readBits(instruction, 4, 6)) {
        case 0b000:
            if (readBit(instruction, 21)) {
                result.opcode = "MSR";
                std::string dest = readBit(instruction, 22) ? "SPSR" : "CPSR";
                uint32_t maskBits = readBits(instruction, 16, 19);
                if (maskBits != 0) dest += "_";
                if (maskBits & 0b1000) dest += "f";
                if (maskBits & 0b0100) dest += "s";
                if (maskBits & 0b0010) dest += "x";
                if (maskBits & 0b0001) dest += "c";
                result.destination = dest;
                result.operand1 = g_regNames[readBits(instruction, 0, 3)];
                break;

            } else {
                result.opcode = "MRS";
                result.destination = g_regNames[readBits(instruction, 12, 15)];
                result.operand1 = readBit(instruction, 22) ? "SPSR" : "CPSR";
                break;
            }
            break;
        case 0b001:
            if (readBits(instruction, 21, 22) == 0b01) {
                result.opcode = "BX";
                result.operand1 = g_regNames[readBits(instruction, 0, 3)];
                break;
            }
            if (readBits(instruction, 21, 22) == 0b11) {
                result.opcode = "CLZ";
                result.destination = g_regNames[readBits(instruction, 12, 15)];
                break;
            }
            break;
        case 0b011:
            if (readBits(instruction, 21, 22) == 0b01) {
                result.opcode = "BLX";
                result.operand1 = g_regNames[readBits(instruction, 0, 3)];
                break;
            }
            break;
        case 0b101:
            result.destination = g_regNames[readBits(instruction, 12, 15)];
            result.operand1 = g_regNames[readBits(instruction, 0, 3)];
            result.operand2 = g_regNames[readBits(instruction, 16, 19)];
            switch (readBits(instruction, 21, 22)) {
                case 0b00:
                    result.opcode = "QADD";
                    break;
                case 0b01:
                    result.opcode = "QSUB";
                    break;
                case 0b10:
                    result.opcode = "QDADD";
                    break;
                case 0b11:
                    result.opcode = "QDSUB";
                    break;
                default:
                    break;
            }
            break;
        case 0b111:
            if (readBits(instruction, 21, 22) == 0b01) {
                result.opcode = "BKPT";
                uint32_t imm = readBits(instruction, 8, 19) << 4 | readBits(instruction, 0, 3);
                result.operand1 = getImmString(imm, useHex);
                break;
            }
            break;
        default:
            break;
    }
}
// ==================================================================================================
void dissembleARMInstruction_data_halfwordMultiply(InstructionDisassembly& result,
                                                   uint32_t instruction, bool useHex) {
    result.destination = g_regNames[readBits(instruction, 16, 19)];
    result.operand1 = g_regNames[readBits(instruction, 0, 3)];
    std::string suffixX = readBit(instruction, 5) ? "T" : "B";
    std::string suffixY = readBit(instruction, 6) ? "T" : "B";
    switch (readBits(instruction, 21, 22)) {
        case 0b00:
            result.opcode = "SMLA" + suffixX + suffixY;
            result.operand2 = g_regNames[readBits(instruction, 8, 11)] + ", " +
                              g_regNames[readBits(instruction, 12, 15)];
            break;
        case 0b01:
            if (!readBit(instruction, 5)) {
                result.opcode = "SMLAW" + suffixY;
                result.operand2 = g_regNames[readBits(instruction, 8, 11)] + ", " +
                                  g_regNames[readBits(instruction, 12, 15)];

            } else {
                result.opcode = "SMULW" + suffixY;
                result.operand2 = g_regNames[readBits(instruction, 8, 11)];
            }
            break;
        case 0b10:
            result.opcode = "SMLAL" + suffixX + suffixY;
            result.operand2 = g_regNames[readBits(instruction, 8, 11)] + ", " +
                              g_regNames[readBits(instruction, 12, 15)];
            break;
        case 0b11:
            result.opcode = "SMUL" + suffixX + suffixY;
            result.operand2 = g_regNames[readBits(instruction, 8, 11)];
            break;
        default:
            break;
    }
}
// ==================================================================================================
void dissembleARMInstruction_data_msrAndHints(InstructionDisassembly& result, uint32_t instruction,
                                              bool useHex) {
    result.opcode = "MSR";
    uint32_t imm12 = readBits(instruction, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, 0);
    std::string dest = readBit(instruction, 22) ? "SPSR" : "CPSR";
    uint32_t maskBits = readBits(instruction, 16, 19);
    if (maskBits != 0) dest += "_";
    if (maskBits & 0b1000) dest += "f";
    if (maskBits & 0b0100) dest += "s";
    if (maskBits & 0b0010) dest += "x";
    if (maskBits & 0b0001) dest += "c";
    result.destination = dest;
    result.operand1 = getImmString(immDecoded.data_u32, useHex);
}
// ==================================================================================================
void dissembleARMInstruction_loadStore(InstructionDisassembly& result, uint32_t instruction,
                                       bool useHex) {
    result.destination = g_regNames[readBits(instruction, 12, 15)];

    std::string offset = "";
    bool includeOffset = true;
    if (readBit(instruction, 25)) {
        offset = (readBit(instruction, 23) ? "" : "-") + g_regNames[readBits(instruction, 0, 3)];
        uint32_t imm5 = readBits(instruction, 7, 11);
        if (imm5 != 0) {
            offset += ", ";
            switch (readBits(instruction, 5, 6)) {
                case 0b00:
                    offset += "LSL ";
                    break;
                case 0b01:
                    offset += "LSR ";
                    break;
                case 0b10:
                    offset += "ASR ";
                    break;
                case 0b11:
                    offset += "ROR ";
                    break;
                default:
                    break;
            }
            offset += getImmString(imm5, useHex);
        }
    } else {
        uint32_t imm = readBits(instruction, 0, 11);
        includeOffset = imm != 0;
        offset = getImmString(imm, useHex);
        if (!readBit(instruction, 23)) {
            offset.insert(1, 1, '-');
        }
    }

    result.operand2 = "[" + g_regNames[readBits(instruction, 16, 19)];
    bool privedged = false;
    if (readBit(instruction, 24)) {
        if (includeOffset) result.operand2 += ", " + offset;
        result.operand2 += "]";
        if (readBit(instruction, 21)) result.operand2 += "!";
    } else {
        result.operand2 += "]";
        if (includeOffset) result.operand2 += ", " + offset;
    }
    if (readBit(instruction, 20)) {
        result.opcode = "LDR";
    } else {
        result.opcode = "STR";
    }
    if (readBit(instruction, 22)) {
        result.opcode += "B";
    }
    if (privedged) {
        result.opcode += "T";
    }
}
// ==================================================================================================
void dissembleARMInstruction_branchAndBlockTransfer(InstructionDisassembly& result,
                                                    uint32_t instruction, bool useHex) {
    result.operand1 =
        g_regNames[readBits(instruction, 16, 19)] + (readBit(instruction, 21) ? "!" : "");
    result.operand2 = getRegListString(readBits(instruction, 0, 15));
    if (readBit(instruction, 22)) result.operand2 += "^";
    switch (readBits(instruction, 20, 25)) {
        case 0b000000:
        case 0b000010:
        case 0b000100:
        case 0b000110:
            result.opcode = "STMDA";
            break;
        case 0b000001:
        case 0b000011:
        case 0b000101:
        case 0b000111:
            result.opcode = "LDMDA";
            break;
        case 0b001000:
        case 0b001010:
        case 0b001100:
        case 0b001110:
            result.opcode = "STMIA";
            break;
        case 0b001001:
        case 0b001011:
        case 0b001101:
        case 0b001111:
            result.opcode = "LDMIA";
            break;
        case 0b010000:
        case 0b010010:
        case 0b010100:
        case 0b010110:
            result.opcode = "STMDB";
            break;
        case 0b010001:
        case 0b010011:
        case 0b010101:
        case 0b010111:
            result.opcode = "LDMDB";
            break;
        case 0b011000:
        case 0b011010:
        case 0b011100:
        case 0b011110:
            result.opcode = "STMIB";
            break;
        case 0b011001:
        case 0b011011:
        case 0b011101:
        case 0b011111:
            result.opcode = "LDMIB";
            break;
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
        case 0b101111: {
            uint32_t imm = readBits(instruction, 0, 23);
            int32_t imms = (((int32_t)(imm << 8)) >> 6) + 8;
            if (readBits(instruction, 28, 31) == 0b1111) {
                result.opcode = "BLX";
                imms = imms | (readBit(instruction, 24) << 1);
            } else {
                result.opcode = "B";
            }
            result.operand1 = ".";
            std::string offset = getImmStringSigned(imms, useHex).substr(1);
            if (imms > 0) offset.insert(0, 1, '+');
            if (imms != 0) result.operand1 += offset;
            result.operand2 = "";
            break;
        }
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
            uint32_t imm = readBits(instruction, 0, 23);
            int32_t imms = (((int32_t)(imm << 8)) >> 6) + 8;
            if (readBits(instruction, 28, 31) == 0b1111) {
                result.opcode = "BLX";
                imms = imms | (readBit(instruction, 24) << 1);
            } else {
                result.opcode = "BL";
            }
            result.operand1 = ".";
            std::string offset = getImmStringSigned(imms, useHex).substr(1);
            if (imms > 0) offset.insert(0, 1, '+');
            if (imms != 0) result.operand1 += offset;
            result.operand2 = "";
            break;
        }
        default:
            break;
    }
}
// ==================================================================================================
void dissembleARMInstruction_coProc(InstructionDisassembly& result, uint32_t instruction,
                                    bool useHex) {
    // OpCode.
    uint8_t op1 = readBits(instruction, 20, 25);
    uint8_t op = readBit(instruction, 4);
    uint8_t opcode = (op1 << 1) | op;
    // Check that the coproc is supported.
    uint8_t coproc = readBits(instruction, 8, 11);
    if (readBits(instruction, 24, 25) != 0b11 && ((coproc & 0b1110) == 0b1010)) {
        LogError("Coprocessor " << coproc << " is not supported!");
    }
    result.destination = "p" + std::to_string(coproc);
    switch (opcode) {
        case 0b0000100:
        case 0b0000101:
        case 0b0001100:
        case 0b0001101:
        case 0b0010000:
        case 0b0010001:
        case 0b0010100:
        case 0b0010101:
        case 0b0011000:
        case 0b0011001:
        case 0b0011100:
        case 0b0011101:
        case 0b0100000:
        case 0b0100001:
        case 0b0100100:
        case 0b0100101:
        case 0b0101000:
        case 0b0101001:
        case 0b0101100:
        case 0b0101101:
        case 0b0110000:
        case 0b0110001:
        case 0b0110100:
        case 0b0110101:
        case 0b0111000:
        case 0b0111001:
        case 0b0111100:
        case 0b0111101: {
            result.opcode = "STC";
            result.operand1 = "c" + std::to_string(readBits(instruction, 12, 15));
            result.operand2 = "[" + g_regNames[readBits(instruction, 16, 19)];
            uint32_t imm = readBits(instruction, 0, 7) << 2;
            bool includeOffset = imm != 0;
            std::string offset = getImmString(imm, useHex);
            if (!readBit(instruction, 23)) {
                offset.insert(1, 1, '-');
            }
            if (readBit(instruction, 24)) {
                if (includeOffset) result.operand2 += ", " + offset;
                result.operand2 += "]";
                if (readBit(instruction, 21)) result.operand2 += "!";
            } else {
                result.operand2 += "]";
                if (includeOffset) result.operand2 += ", " + offset;
            }
            if (readBit(instruction, 22)) result.opcode += "L";
            break;
        }
        case 0b0000110:
        case 0b0000111:
        case 0b0001110:
        case 0b0001111:
        case 0b0010010:
        case 0b0010011:
        case 0b0010110:
        case 0b0010111:
        case 0b0011010:
        case 0b0011011:
        case 0b0011110:
        case 0b0011111:
        case 0b0100010:
        case 0b0100011:
        case 0b0100110:
        case 0b0100111:
        case 0b0101010:
        case 0b0101011:
        case 0b0101110:
        case 0b0101111:
        case 0b0110010:
        case 0b0110011:
        case 0b0110110:
        case 0b0110111:
        case 0b0111010:
        case 0b0111011:
        case 0b0111110:
        case 0b0111111: {
            result.opcode = "LDC";
            result.operand1 = "c" + std::to_string(readBits(instruction, 12, 15));
            result.operand2 = "[" + g_regNames[readBits(instruction, 16, 19)];
            uint32_t imm = readBits(instruction, 0, 7) << 2;
            bool includeOffset = imm != 0;
            std::string offset = getImmString(imm, useHex);
            if (!readBit(instruction, 23)) {
                offset.insert(1, 1, '-');
            }
            if (readBit(instruction, 24)) {
                if (includeOffset) result.operand2 += ", " + offset;
                result.operand2 += "]";
                if (readBit(instruction, 21)) result.operand2 += "!";
            } else {
                result.operand2 += "]";
                if (includeOffset) result.operand2 += ", " + offset;
            }
            if (readBit(instruction, 22)) result.opcode += "L";
            break;
        }
        case 0b0001000:
        case 0b0001001: {
            result.opcode = "MCRR";
            result.operand2 = getImmString(readBits(instruction, 5, 7), useHex) + ", " +
                              g_regNames[readBits(instruction, 12, 15)] + ", " +
                              g_regNames[readBits(instruction, 16, 19)] + ", c" +
                              std::to_string(readBits(instruction, 0, 3));
            break;
        }
        case 0b0001010:
        case 0b0001011: {
            result.opcode = "MRRC";
            result.operand2 = getImmString(readBits(instruction, 5, 7), useHex) + ", " +
                              g_regNames[readBits(instruction, 12, 15)] + ", " +
                              g_regNames[readBits(instruction, 16, 19)] + ", c" +
                              std::to_string(readBits(instruction, 0, 3));
            break;
        }
        case 0b1000000:
        case 0b1000010:
        case 0b1000100:
        case 0b1000110:
        case 0b1001000:
        case 0b1001010:
        case 0b1001100:
        case 0b1001110:
        case 0b1010000:
        case 0b1010010:
        case 0b1010100:
        case 0b1010110:
        case 0b1011000:
        case 0b1011010:
        case 0b1011100:
        case 0b1011110: {
            result.opcode = "CDP";
            result.operand2 = getImmString(readBits(instruction, 20, 23), useHex) + ", c" +
                              std::to_string(readBits(instruction, 12, 15)) + ", c" +
                              std::to_string(readBits(instruction, 16, 19)) + ", c" +
                              std::to_string(readBits(instruction, 0, 3));
            uint32_t opc2 = readBits(instruction, 5, 7);
            if (opc2) {
                result.operand2 += ", " + getImmString(opc2, useHex);
            }
            break;
        }
        case 0b1000001:
        case 0b1000101:
        case 0b1001001:
        case 0b1001101:
        case 0b1010001:
        case 0b1010101:
        case 0b1011001:
        case 0b1011101: {
            result.opcode = "MCR";
            result.operand2 = getImmString(readBits(instruction, 21, 23), useHex) + ", " +
                              g_regNames[readBits(instruction, 12, 15)] + ", c" +
                              std::to_string(readBits(instruction, 16, 19)) + ", c" +
                              std::to_string(readBits(instruction, 0, 3));
            uint32_t opc2 = readBits(instruction, 5, 7);
            if (opc2) {
                result.operand2 += ", " + getImmString(opc2, useHex);
            }
            break;
        }
        case 0b1000011:
        case 0b1000111:
        case 0b1001011:
        case 0b1001111:
        case 0b1010011:
        case 0b1010111:
        case 0b1011011:
        case 0b1011111: {
            result.opcode = "MRC";
            result.operand2 = getImmString(readBits(instruction, 21, 23), useHex) + ", " +
                              g_regNames[readBits(instruction, 12, 15)] + ", c" +
                              std::to_string(readBits(instruction, 16, 19)) + ", c" +
                              std::to_string(readBits(instruction, 0, 3));
            uint32_t opc2 = readBits(instruction, 5, 7);
            if (opc2) {
                result.operand2 += ", " + getImmString(opc2, useHex);
            }
            break;
        }
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
            result.opcode = "SVC";
            result.destination = "";
            result.operand1 = getImmString(readBits(instruction, 0, 23), useHex);
            break;
        default:
            break;
    }
}
// ==================================================================================================
// THUMB Disassemble
// ==================================================================================================
InstructionDisassembly dissembleTHUMBInstruction(uint32_t instruction, bool useHex) {
    InstructionDisassembly result;
    result.opcode = UNDEFINED_INSTRUCTION_OPCODE;
    // Clear the result if undefined.
    if (result.opcode == UNDEFINED_INSTRUCTION_OPCODE) {
        result = InstructionDisassembly();
        result.opcode = UNDEFINED_INSTRUCTION_OPCODE;
    }
    result.rawData = instruction;
    return result;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
