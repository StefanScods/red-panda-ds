/**
 * @file armInterpreter/arm_misc.cpp
 * @brief Implements ARM mode instuction set for instructions which do not fit in any other
 * category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

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
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MCR--MCR2?lang=en
// ==================================================================================================
cycles ARM::ARM_MCR(uint32_t instruct) {
    // Get the ARM9 interface.
    if (isARM7()) return ARM_UNDEFINED_INST(instruct);
    ARM946ES* arm9 = dynamic_cast<ARM946ES*>(this);
    if (arm9 == nullptr) {
        LogErrorPrefixed("Expected ARM9 CPU!", "MRC");
        return 1;
    }
    uint8_t coproc = readBits(instruct, 8, 11);
    uint8_t op1 = readBits(instruct, 21, 23);
    uint8_t op2 = readBits(instruct, 5, 7);
    uint8_t Cn = readBits(instruct, 16, 19);
    uint8_t Cm = readBits(instruct, 0, 3);
    uint8_t Rt = readBits(instruct, 12, 15);

    busPayload result;
    if (coproc == 15) {
        result = arm9->writeToCP15(Cn, Cm, op1, op2, *activeRegs[Rt]);
    } else {
        LogErrorPrefixed("Unsupported coprocessor number: " << (uint32_t)coproc, "MRC");
        return 1;
    }

    // Return the num cycles the command took.
    return result.numCycles;
}
// ==================================================================================================
// MRRC
// ==================================================================================================
cycles ARM::ARM_MRRC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// MRC
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MRC--MRC2?lang=en
// ==================================================================================================
cycles ARM::ARM_MRC(uint32_t instruct) {
    // Get the ARM9 interface.
    if (isARM7()) return ARM_UNDEFINED_INST(instruct);
    ARM946ES* arm9 = dynamic_cast<ARM946ES*>(this);
    if (arm9 == nullptr) {
        LogErrorPrefixed("Expected ARM9 CPU!", "MRC");
        return 1;
    }
    uint8_t coproc = readBits(instruct, 8, 11);
    uint8_t op1 = readBits(instruct, 21, 23);
    uint8_t op2 = readBits(instruct, 5, 7);
    uint8_t Cn = readBits(instruct, 16, 19);
    uint8_t Cm = readBits(instruct, 0, 3);
    uint8_t Rt = readBits(instruct, 12, 15);

    busPayload result;
    if (coproc == 15) {
        result = arm9->readFromCP15(Cn, Cm, op1, op2);
    } else {
        LogErrorPrefixed("Unsupported coprocessor number: " << (uint32_t)coproc, "MRC");
        return 1;
    }

    // Set the target reg.
    if (Rt == PC_REGISTER_NUM) {
        setAPSR(result.data);
    } else {
        *activeRegs[Rt] = result.data;
    }

    // Return the num cycles the command took.
    return result.numCycles;
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

}  // namespace Core
}  // namespace RedPandaDS
