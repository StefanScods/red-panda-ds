#include "bios.h"

#include "cpu.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"
#include "memoryDefines.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
// Base BIOS
// ==================================================================================================
BIOS::BIOS() {
}
// ==================================================================================================
BIOS::~BIOS() {
}
// ==================================================================================================
// Base NDS BIOS
// ==================================================================================================
NDS_BIOS::NDS_BIOS() {
}
// ==================================================================================================
NDS_BIOS::~NDS_BIOS() {
}
// ==================================================================================================
void NDS_BIOS::cleanup() {
    cpu->clearInstructionPipeline();
    uint32_t instructionSize = cpu->getThumbMode() ? THUMB_MODE_INST_SIZE : ARM_MODE_INST_SIZE;
    cpu->pc() -= instructionSize;
}
// ==================================================================================================
cycles NDS_BIOS::unknownSWI(uint32_t vector) {
    uint32_t instructionSize = cpu->getThumbMode() ? THUMB_MODE_INST_SIZE : ARM_MODE_INST_SIZE;
    LogError("Unsupported SWI argument " << vector << " at "
                                         << PrintHex(cpu->pc() - instructionSize) << "!");
    return 1;
}
// ==================================================================================================
cycles NDS_BIOS::WaitByLoop() {
    cycles count = *(cpu->activeRegs[0]);
    // Takes ~4 cycles per value of reg0.
    return 4 * count;
}
// ==================================================================================================
cycles NDS_BIOS::IntrWait() {
    cpu->setProcessorState(ProcessorStates::Halted);
    return 1;
}
// ==================================================================================================
// NDS ARM9 BIOS
// ==================================================================================================
NDS_ARM9_BIOS::NDS_ARM9_BIOS() {
}
// ==================================================================================================
NDS_ARM9_BIOS::~NDS_ARM9_BIOS() {
}
// ==================================================================================================
cycles NDS_ARM9_BIOS::handleSWI(uint32_t vector) {
    cleanup();

    switch (vector) {
        case 0x3:
            return WaitByLoop();
        case 0x4:
            return IntrWait();
        default:
            // Fallthrough and return "UNKNOWN".
            break;
    }
    return unknownSWI(vector);
}
// ==================================================================================================
// NDS ARM7 BIOS
// ==================================================================================================
NDS_ARM7_BIOS::NDS_ARM7_BIOS() {
}
// ==================================================================================================
NDS_ARM7_BIOS::~NDS_ARM7_BIOS() {
}
// ==================================================================================================
cycles NDS_ARM7_BIOS::handleSWI(uint32_t vector) {
    cleanup();

    switch (vector) {
        case 0x3:
            return WaitByLoop();
        case 0x4:
            return IntrWait();
        default:
            // Fallthrough and return "UNKNOWN".
            break;
    }
    return unknownSWI(vector);
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
