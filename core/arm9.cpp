/**
 * @file arm9.cpp
 * @brief Implements ARM946ES
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include <common.h>

#include <cstring>

#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
ARM946ES::ARM946ES() {
    arm9 = true;
    // Memory Access timings. Based on https://problemkaputt.de/gbatek.htm#dsmemorytimings
    // TCM
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::TCM] = 1;
    // Main RAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 9;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 9;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 20;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 2;
    // WRAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 2;
    // I/O
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 2;
    // Palette RAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 10;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 10;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 5;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 5;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 10;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 2;
    // VRAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 10;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 10;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 5;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 5;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 10;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 2;
    // OAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 2;
    // GBA ROM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 38;
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 38;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 38;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 38;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 19;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 19;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 19;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 19;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 38;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 38;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 24;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 24;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 26;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 26;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM1] = 12;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::GBAROM2] = 12;
    // GBA RAM.
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 26;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 20;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 26;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 20;
    // BIOS.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 2;

    // TCM set up.
    itcm = new uint8_t[ITCM_SIZE];
    dtcm = new uint8_t[DTCM_SIZE];
}
// ==================================================================================================
ARM946ES::~ARM946ES() {
    DELETE_DYNAMIC_ARRAY_POINTER(itcm);
    DELETE_DYNAMIC_ARRAY_POINTER(dtcm);
}
// ==================================================================================================
busPayload ARM946ES::readFromCP15(uint8_t Cn, uint8_t Cm, uint8_t op1, uint8_t op2) {
    // Create a result -> temp data represents an error.
    busPayload result;
    result.size = 4;
    result.numCycles = 1;
    result.data = INVALID_MEM_32BIT;
    // Create a unique keep for all coprocessor commands.
    uint32_t op1_32 = op1;
    uint32_t Cn_32 = Cn;
    uint32_t Cm_32 = Cm;
    uint32_t op2_32 = op2;
    uint32_t key = (op1_32 << 24) | (Cn_32 << 16) | (Cm_32 << 8) | op2_32;
    // Match the key to a coprocessor read command.
    switch (key) {
        //  TCM Physical Size
        case 0x00000002: {
            result.data = 0x00140180;
            break;
        }
        //  Control Register.
        case 0x00010000: {
            result.data = co_controlReg.read();
            break;
        }
        default:
            LogError("Cannot read from CoProcessor 15 with the following settings:\n\tCn = "
                     << Cn_32 << "\n\tCm = " << Cm_32 << "\n\top1 = " << op1_32
                     << "\n\top2 = " << op2_32);
            return result;
    }
    return result;
}
// ==================================================================================================
busPayload ARM946ES::writeToCP15(uint8_t Cn, uint8_t Cm, uint8_t op1, uint8_t op2, uint32_t data) {
    // Create a result -> temp data represents an error.
    busPayload result;
    result.size = 4;
    result.numCycles = 1;
    result.data = INVALID_MEM_32BIT;
    // Create a unique keep for all coprocessor commands.
    uint32_t op1_32 = op1;
    uint32_t Cn_32 = Cn;
    uint32_t Cm_32 = Cm;
    uint32_t op2_32 = op2;
    uint32_t key = (op1_32 << 24) | (Cn_32 << 16) | (Cm_32 << 8) | op2_32;
    // Match the key to a coprocessor write command.
    switch (key) {
        //  Control Register.
        case 0x00010000: {
            co_controlReg.write(data);
            break;
        }

        // Protection Unit.
        // Data Region.
        case 0x00060000:
        case 0x00060100:
        case 0x00060200:
        case 0x00060300:
        case 0x00060400:
        case 0x00060500:
        case 0x00060600:
        case 0x00060700: {
            writeToPURegionControl(false, Cm_32, data);
            break;
        }
        // Instruction Region.
        case 0x01060000:
        case 0x01060100:
        case 0x01060200:
        case 0x01060300:
        case 0x01060400:
        case 0x01060500:
        case 0x01060600:
        case 0x01060700: {
            writeToPURegionControl(true, Cm_32, data);
            break;
        }
        // Cachability Bits.
        case 0x00020000: {
            co_puDataUnifiedCachabilityBits = data & 0x000000FF;
            break;
        }
        case 0x00020001: {
            co_puInstructionCachabilityBits = data & 0x000000FF;
            break;
        }
        // Cache Write-Bufferability Bits for Data Protection Regions.
        case 0x00030000: {
            co_puDataWriteBufferabilityBits = data & 0x000000FF;
            break;
        }
        // Access Permission Protection Region.
        case 0x00050000: {
            co_puDataUnifiedAccessPermissions = data & 0x0000FFFF;
            break;
        }
        case 0x00050001: {
            co_puInstructionAccessPermissions = data & 0x0000FFFF;
            break;
        }
        case 0x00050002: {
            co_puDataUnifiedExtendedAccessPermissions = data;
            break;
        }
        case 0x00050003: {
            co_puInstructionExtendedAccessPermissions = data;
            break;
        }

        // Cache control.
        case 0x00070500: {
            invalidateInstructionCache();
            break;
        }
        case 0x00070600: {
            invalidateDataCache();
            break;
        }
        case 0x00070a04: {
            // Drain write buffer.
            // TODO!!!: No write buffer is currently implemented.
            break;
        }

        // Data / Instruction TCM Size/Base.
        case 0x00090100: {
            setDTCMBaseAndSize(data);
            break;
        }
        case 0x00090101: {
            setITCMBaseAndSize(data);
            break;
        }

        default:
            LogError("Cannot write to CoProcessor 15 with the following settings:\n\tCn = "
                     << Cn_32 << "\n\tCm = " << Cm_32 << "\n\top1 = " << op1_32
                     << "\n\top2 = " << op2_32 << "\n\tcoproc key = " << PrintHexPadded(key, 8));
            break;
    }
    return result;
}
// ==================================================================================================
void ARM946ES::reset() {
    ARM::reset();

    // TCM.
    memset(itcm, 0, ITCM_SIZE);
    itcmBase = 0x00000000;
    itcmVirtSize = 0x00000000;
    memset(dtcm, 0, DTCM_SIZE);
    dtcmBase = 0x27C0000;
    dtcmVirtSize = 0x00000000;

    // Cache.
    invalidateInstructionCache();
    invalidateDataCache();

    // Protection Unit.
    for (unsigned int i = 0; i < ARM946ES_PU_NumRegions; i++) {
        writeToPURegionControl(true, i, 0);
        writeToPURegionControl(false, i, 0);
    }

    // CPU Constants.
    co_mainIdReg = 0x41059461;
    co_cacheTypeAndSize = 0x0F0D2112;
    co_TCMPhysicalSize = 0x00140180;

    co_controlReg.write(0);

    co_puDataUnifiedCachabilityBits = 0;
    co_puInstructionCachabilityBits = 0;
    co_puDataWriteBufferabilityBits = 0;

    co_puDataUnifiedAccessPermissions = 0;
    co_puInstructionAccessPermissions = 0;
    co_puDataUnifiedExtendedAccessPermissions = 0;
    co_puInstructionExtendedAccessPermissions = 0;

    std::fill(std::begin(co_puDataUnifiedRegions), std::end(co_puDataUnifiedRegions), 0);

    std::fill(std::begin(co_puInstructionRegions), std::end(co_puInstructionRegions), 0);

    co_cacheDataLockdown = 0;
    co_cacheInstructionLockdown = 0;

    co_dataTCMBaseAndVirtualSize = 0;
    co_instructionTCMBaseAndVirtualSize = 0;

    std::fill(std::begin(co_processIdRegs), std::end(co_processIdRegs), 0);

    std::fill(std::begin(co_implDefinedAndDebugRegs), std::end(co_implDefinedAndDebugRegs), 0);
}
// ==================================================================================================
busPayload ARM946ES::readBus(uint32_t address, uint32_t size, bool codeRead) {
    uint32_t& previousAddr = codeRead ? previousCodeAddr : previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is one word, or one
     * halfword greater than the address used in the preceding cycle
     */
    bool sequencial = address == previousAddr + 4 || address == previousAddr + 2;
    uint8_t memRegion = (address) >> 24;
    // Override mem region for special cases.
    // Instruction TCM.
    if (co_controlReg.itcmEnable && !co_controlReg.itcmLoadEnable && address >= itcmBase &&
        address < (itcmBase + itcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
    }
    // Data TCM.
    if (co_controlReg.dtcmEnable && !co_controlReg.dtcmLoadEnable && address >= dtcmBase &&
        address < (dtcmBase + dtcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
    }

    // Preform the read and determine the cycle map to read.
    cycles* cycleMapSequential = nullptr;
    cycles* cycleMapNonSequential = nullptr;
    uint32_t data;
    switch (size) {
        case (32):  // 32 Bit read.
            data = bus->read32ARM9(address);
            cycleMapSequential =
                codeRead ? code_sequencial32BitAccessTimings : data_sequencial32BitAccessTimings;
            cycleMapNonSequential = codeRead ? code_nonSequencial32BitAccessTimings
                                             : data_nonSequencial32BitAccessTimings;
            break;
        case (16):  // 16 Bit read.
            data = bus->read16ARM9(address);
            cycleMapSequential =
                codeRead ? code_sequencial16BitAccessTimings : data_sequencial16BitAccessTimings;
            cycleMapNonSequential = codeRead ? code_nonSequencial16BitAccessTimings
                                             : data_nonSequencial16BitAccessTimings;
            break;
        case (8):  // 8 Bit read.
            data = bus->read8ARM9(address);
            //  8bit data accesses have the same timings as 16bit data.
            // There are no code 8 bit accesses.
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        default:
            LogError(size << " bit reads are currently unsupported");
            return {0, 0, 0};
    }

    // Update the previous address to this read to keep track of sequencial accesses.
    previousAddr = address;
    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}
// ==================================================================================================
busPayload ARM946ES::writeBus(uint32_t address, uint32_t data, uint32_t size) {
    // Writes always relate to the previous data.
    uint32_t& previousAddr = previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is one word, or one
     * halfword greater than the address used in the preceding cycle
     */
    bool sequencial = address == previousAddr + 4 || address == previousAddr + 2;
    uint8_t memRegion = (address) >> 24;
    // Override mem region for special cases.
    // Instruction TCM.
    if (co_controlReg.itcmEnable && address >= itcmBase && address < (itcmBase + itcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
    }
    // Data TCM.
    if (co_controlReg.dtcmEnable && address >= dtcmBase && address < (dtcmBase + dtcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
    }

    // Preform the read and determine the cycle map to read.
    cycles* cycleMapSequential = nullptr;
    cycles* cycleMapNonSequential = nullptr;
    switch (size) {
        case (32):  // 32 Bit write.
            bus->write32ARM9(address, data);
            cycleMapSequential = data_sequencial32BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial32BitAccessTimings;
            break;
        case (16):  // 16 Bit write.
            bus->write16ARM9(address, data & 0xFFFF);
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        case (8):
            bus->write8ARM9(address, data & 0xFF);
            //  8bit data accesses have the same timings as 16bit data.
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        default:
            LogError(size << " bit writes are currently unsupported");
            return {0, 0, 0};
    }

    // Update the previous address to this write to keep track of sequencial accesses.
    previousAddr = address;
    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}
// ==================================================================================================
cycles ARM946ES::cycle() {
    cycles cyclesRan = 0;
    while (currentCycle < targetCycle) {
        // Clear just branched indicator.
        justBranched = false;
        // Maintain instruction pipeline.
        if (instuctionPipeLine[0] == NO_INSTRUCT && instuctionPipeLine[2] != NO_INSTRUCT) {
            advanceInstructionPipeline();
        }
        // Perform fetches and executes in parallel.
        if (executeCooldown == 0 && instuctionPipeLine[0] != NO_INSTRUCT) {
            if (!justHitBreakpoint) {
                // Check to see if PC is currently equal to a breakpoint.
                uint32_t pcCorrected =
                    pc() - 2 * (getThumbMode() ? THUMB_MODE_INST_SIZE : ARM_MODE_INST_SIZE);
                if (breakpoints.contains(pcCorrected)) {
                    justHitBreakpoint = true;
                    break;
                }
            }
            if (hasExecutionLimit) {
                if (executionLimit == 0) {
                    hasExecutionLimit = false;
                    justHitBreakpoint = true;
                    break;
                }
                executionLimit--;
            }
            // Instruction is on the execute stage, execute a new instuction.
            executeCooldown = execute();
            // Clear the just hit a breakpoint indicator.
            justHitBreakpoint = false;
        }
        if (fetchCooldown == 0 && instuctionPipeLine[2] == NO_INSTRUCT) {
            // Instruction pipeline has space, fetch a new instuction.
            fetchCooldown = fetch();
        }

        // Determine how much to progress the CPU's cycle counter.
        if (fetchCooldown > 0)
            cyclesElapsed = std::min(fetchCooldown, executeCooldown);
        else
            cyclesElapsed = executeCooldown;
        // Catch cycles where no work is done -> filling pipeline.
        if (cyclesElapsed == 0) {
            cyclesElapsed = 1;
        }
        // Increment the cpu's current cycles based on the number of cycles elapsed.
        cyclesRan += cyclesElapsed;
        addCyclesElapsed();
    }

    return cyclesRan;
}
// ==================================================================================================
void ARM946ES::setITCMBaseAndSize(uint32_t data) {
    uint8_t virtSizeShift = readBits(data, 1, 4);
    virtSizeShift = std::min(virtSizeShift, (uint8_t)23);
    virtSizeShift = std::max(virtSizeShift, (uint8_t)3);
    itcmVirtSize = 512 << virtSizeShift;

    itcmBase = readBits(data, 12, 4) << 12;
    if (itcmBase != 0) {
        LogWarning("ITCM base should be fixed at zero?");
    }

    LogDebug("Setting ITCM Base: " << PrintHexPadded(itcmBase, 8));
    LogDebug("Setting ITCM Size: " << PrintHex(itcmVirtSize));
}
// ==================================================================================================
void ARM946ES::setDTCMBaseAndSize(uint32_t data) {
    uint8_t virtSizeShift = readBits(data, 1, 4);
    virtSizeShift = std::min(virtSizeShift, (uint8_t)23);
    virtSizeShift = std::max(virtSizeShift, (uint8_t)3);
    dtcmVirtSize = 512 << virtSizeShift;

    dtcmBase = readBits(data, 12, 4) << 12;

    LogDebug("Setting DTCM Base: " << PrintHexPadded(dtcmBase, 8));
    LogDebug("Setting DTCM Size: " << PrintHex(dtcmVirtSize));
}
// ==================================================================================================
void ARM946ES::invalidateInstructionCache() {
    for (uint32_t i = 0; i < ARM946ES_InstructionCache_NumSets; i++) {
        instructionCache[i].ways[0].valid = false;
        instructionCache[i].ways[1].valid = false;
        instructionCache[i].ways[2].valid = false;
        instructionCache[i].ways[3].valid = false;
        // All other data can stay as is as long as valid is false;
    }
}
// ===================================================================================================
void ARM946ES::invalidateDataCache() {
    for (uint32_t i = 0; i < ARM946ES_DataCache_NumSets; i++) {
        dataCache[i].ways[0].valid = false;
        dataCache[i].ways[1].valid = false;
        dataCache[i].ways[2].valid = false;
        dataCache[i].ways[3].valid = false;
        // All other data can stay as is as long as valid is false;
    }
}
// ==================================================================================================
void ARM946ES::writeToPURegionControl(bool instruction, uint8_t regionNum, uint32_t data) {
    assert(regionNum < ARM946ES_PU_NumRegions);
    ARM946ES_PU_Region* regionToModify =
        instruction ? &puInstructionRegion[regionNum] : &puDataRegion[regionNum];

    regionToModify->enabled = readBit(data, 0);
    regionToModify->size = std::max(2 << 11, 2 << readBits(data, 1, 5));
    regionToModify->baseAddress = 4096 * readBits(data, 12, 31);
}
// ==================================================================================================
uint32_t ARM946ES_ControlReg_Layout::read() {
    uint32_t result = 0;
    writeBit(result, 0, mmuEnable);
    writeBit(result, 2, unifiedCacheEnable);
    writeBit(result, 3, 1);  // Write Buffer (always on).
    writeBit(result, 4, 1);  // Exception Handling (always on).
    writeBit(result, 5, 1);  // 26bit-address faults (always on).
    writeBit(result, 6, 1);  // Abort Model (pre v4) (always on).
    writeBit(result, 7, endianness);
    writeBit(result, 12, instructionCacheEnable);
    writeBit(result, 13, exceptionVectors);
    writeBit(result, 14, cacheReplacement);
    writeBit(result, 15, preARMv5Mode);
    writeBit(result, 16, dtcmEnable);
    writeBit(result, 17, dtcmLoadEnable);
    writeBit(result, 18, itcmEnable);
    writeBit(result, 19, itcmLoadEnable);
    // All other bits are zero.
    return result;
}
// ==================================================================================================
void ARM946ES_ControlReg_Layout::write(uint32_t data) {
    mmuEnable = readBit(data, 0);
    if (mmuEnable) {
        LogError(
            "Writing to ARM946E-S MMU/PU Enable via control register is currently unsupported");
    }

    unifiedCacheEnable = readBit(data, 2);
    if (unifiedCacheEnable) {
        LogError(
            "Writing to ARM946E-S Data/Unified Cache Enable via control register is currently "
            "unsupported");
    }

    endianness = readBit(data, 7);
    if (endianness) {
        LogError("Writing ARM946E-S Endianness to Big Endian is currently unsupported");
    }

    instructionCacheEnable = readBit(data, 12);
    if (instructionCacheEnable) {
        LogError(
            "Writing to ARM946E-S Instruction Cache Enable via control register is currently "
            "unsupported");
    }

    exceptionVectors = readBit(data, 13);
    if (exceptionVectors) {
        LogError("Writing ARM946E-S Exception Vectors to high vectors is currently unsupported");
    }

    cacheReplacement = readBit(data, 14);
    if (cacheReplacement) {
        LogError(
            "Writing ARM946E-S Cache Replacement policy to Round-Robin is currently unsupported");
    }

    preARMv5Mode = readBit(data, 15);
    if (preARMv5Mode) {
        LogError("Writing ARM946E-S Pre-ARMv5 compatibility mode is currently unsupported");
    }
    dtcmEnable = readBit(data, 16);
    dtcmLoadEnable = readBit(data, 17);
    itcmEnable = readBit(data, 18);
    itcmLoadEnable = readBit(data, 19);

    // All other bits are fixed.
};
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
