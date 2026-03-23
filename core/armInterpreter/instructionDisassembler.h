#ifndef INSTRUCTION_DISASSEMBLER_H
#define INSTRUCTION_DISASSEMBLER_H

#include <cstdint>
#include <string>

namespace RedPandaDS {
namespace Core {

const std::string UNDEFINED_INSTRUCTION_OPCODE = "<UNDEFINED>";

struct InstructionDisassembly {
    uint32_t rawData;
    std::string opcode;
    std::string conditionCode;
    std::string destination;
    std::string operand1;
    std::string operand2;
    std::string comment;

    std::string toString();
};
/**
 * @brief Disassembles a single ARM instruction.
 *
 * @param instruction Instruction to disassemble.
 * @param useHex Controls whether to print immediates in hexidecimal (true) or decimal (false)
 * notation.
 *
 * @return InstructionDisassembly
 */
InstructionDisassembly dissembleARMInstruction(uint32_t instruction, bool useHex = false);
/**
 * @brief Disassembles a single THUMB instruction.
 *
 * @param instruction Instruction to disassemble.
 * @param useHex Controls whether to print immediates in hexidecimal (true) or decimal (false)
 * notation.
 *
 * @return InstructionDisassembly
 */
InstructionDisassembly dissembleTHUMBInstruction(uint32_t instruction, bool useHex = false);

// Helpers.
std::string getImmString(uint32_t val, bool useHex);
std::string getImmStringSigned(int32_t val, bool useHex);
std::string getRegListString(uint32_t regList);
void dissembleARMInstruction_wideEncoding(InstructionDisassembly& result, uint32_t instruction,
                                          bool useHex);
void dissembleARMInstruction_data(InstructionDisassembly& result, uint32_t instruction,
                                  bool useHex);
void dissembleARMInstruction_data_dataProcessing(InstructionDisassembly& result,
                                                 uint32_t instruction, bool useHex);
void dissembleARMInstruction_data_multAndMultAccumulate(InstructionDisassembly& result,
                                                        uint32_t instruction, bool useHex);
void dissembleARMInstruction_data_syncronizationPrimatives(InstructionDisassembly& result,
                                                           uint32_t instruction, bool useHex);
void dissembleARMInstruction_data_extraLoadAndStore(InstructionDisassembly& result,
                                                    uint32_t instruction, bool useHex);
void dissembleARMInstruction_data_misc_halfwordMultiply(InstructionDisassembly& result,
                                                        uint32_t instruction, bool useHex);
void dissembleARMInstruction_data_misc(InstructionDisassembly& result, uint32_t instruction,
                                       bool useHex);
void dissembleARMInstruction_data_halfwordMultiply(InstructionDisassembly& result,
                                                   uint32_t instruction, bool useHex);
void dissembleARMInstruction_data_msrAndHints(InstructionDisassembly& result, uint32_t instruction,
                                              bool useHex);
void dissembleARMInstruction_loadStore(InstructionDisassembly& result, uint32_t instruction,
                                       bool useHex);
void dissembleARMInstruction_branchAndBlockTransfer(InstructionDisassembly& result,
                                                    uint32_t instruction, bool useHex);
void dissembleARMInstruction_coProc(InstructionDisassembly& result, uint32_t instruction,
                                    bool useHex);
void dissembleTHUMBInstruction_shiftAddSubtractMoveCompare(InstructionDisassembly& result,
                                                           uint32_t instruction, bool useHex);
void dissembleTHUMBInstruction_dataProcessing(InstructionDisassembly& result, uint32_t instruction,
                                              bool useHex);
void dissembleTHUMBInstruction_specialDataAndBranch(InstructionDisassembly& result,
                                                    uint32_t instruction, bool useHex);
void dissembleTHUMBInstruction_loadStore(InstructionDisassembly& result, uint32_t instruction,
                                         bool useHex);
void dissembleTHUMBInstruction_misc(InstructionDisassembly& result, uint32_t instruction,
                                    bool useHex);
void dissembleTHUMBInstruction_condBranchAndSupervisorCall(InstructionDisassembly& result,
                                                           uint32_t instruction, bool useHex);
}  // namespace Core
}  // namespace RedPandaDS

#endif
