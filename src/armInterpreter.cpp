/**
 * @file armInterpreter.cpp
 * @brief Implements arm instuction set decoding
 */
#include "cpu.h"
#include "interconnect.h"
#include <cstdint>

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

// !!!TODO Handle when commands update flags.
// !!!TODO Handle when commands target PC.

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions?lang=en
cycles ARM::dataProcessingDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    // Extract useful parts of the instruction in order to decode.
    // OpCode.
    uint8_t op = readBits(instuct, 20, 24);
    // S - Set Condition Codes
    bool S = readBit(instuct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instuct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instuct, 12, 15);
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--immediate-?lang=en
    if (readBit(instuct, 25)) {  // Bit 25 = 1;
        uint32_t imm12 = readBits(instuct, 0, 11);
        u32AndBool immDecoded = ARMExpandImm_C(imm12,  0);
        switch (op) {
            // MOV (immediate).
            case 0b11010:
            case 0b11011:
                return mov32bit(Rd, immDecoded.data_u32);
        }

    } else {  // Bit 25 = 0;
        bool bit4 = readBits(instuct, 20, 24);
    }

    LogError("Unsupported instruction: " << instuct << "!");
    return 1;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Load-store-word-and-unsigned-byte?lang=en
cycles ARM::loadStoreDecodeAndExecute(uint32_t instuct, uint8_t cond) {
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