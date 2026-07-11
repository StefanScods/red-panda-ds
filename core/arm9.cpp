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
ARM946ES::ARM946ES(BIOS* d_bios) : ARM(d_bios) {
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
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::TCM] = 0;       // Not cacheable.
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::TCM] = 0;  // Not cacheable.
    // Main RAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 9;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 9;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 20;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::MAIN_RAM] = 23;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::MAIN_RAM] = 23;
    // WRAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::WRAM] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::WRAM] = 11;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::WRAM] = 11;
    // I/O
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::IO] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::IO] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::IO] = 11;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::IO] = 11;
    // Palette RAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 10;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 10;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 5;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 5;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 10;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::PALETTES] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::PALETTES] = 12;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::PALETTES] = 12;
    // VRAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 10;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 10;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 5;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 5;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 10;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::VRAM] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::VRAM] = 12;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::VRAM] = 12;
    // OAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::OAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::OAM] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::OAM] = 12;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::OAM] = 12;
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
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::GBAROM1] = 60;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::GBAROM2] = 60;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::GBAROM1] = 60;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::GBAROM2] = 60;
    // GBA RAM.
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 26;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 20;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 26;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::GBARAM] = 20;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::GBARAM] = 35;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::GBARAM] = 35;
    // BIOS.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 4;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 4;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 2;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 8;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::BIOS] = 2;
    cacheMissPenaltyTimings[ARM9MemoryRegionNum::BIOS] = 10;
    cacheWriteBackPenaltyTimings[ARM9MemoryRegionNum::BIOS] = 10;

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
        //  Main ID Register.
        case 0x00000000: {
            result.data = co_mainIdReg;
            break;
        }
        //  Cache Type and Size.
        case 0x00000001: {
            result.data = co_cacheTypeAndSize;
            break;
        }
        //  TCM Physical Size.
        case 0x00000002: {
            result.data = co_TCMPhysicalSize;
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
        case 0x00070501: {
            invalidateInstructionCacheLine(data);
            break;
        }
        case 0x00070502: {
            invalidateInstructionCacheLine(data, true);
            break;
        }
        case 0x00070600: {
            invalidateDataCache();
            break;
        }
        case 0x00070601: {
            invalidateDataCacheLine(data);
            break;
        }
        case 0x00070602: {
            invalidateDataCacheLine(data, true);
            break;
        }
        case 0x00070a01: {
            cleanDataCacheLine(data, result);
            break;
        }
        case 0x00070a02: {
            cleanDataCacheLine(data, result, true);
            break;
        }
        case 0x00070a04: {
            // Drain write buffer.
            // TODO!!!: No write buffer is currently implemented.
            break;
        }
        case 0x00070e01: {
            cleanDataCacheLine(data, result);
            invalidateDataCacheLine(data);
            break;
        }
        case 0x00070e02: {
            cleanDataCacheLine(data, result, true);
            invalidateDataCacheLine(data, true);
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

    // Inital SP values.
    reg[SP_REGISTER_NUM] = 0x0380FD80;
    regIRQ[0] = 0x0380FF80;
    regSVC[0] = 0x0380FFC0;

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

    // co_cacheDataLockdown = 0;
    // co_cacheInstructionLockdown = 0;
    // co_dataTCMBaseAndVirtualSize = 0;
    // co_instructionTCMBaseAndVirtualSize = 0;
    // std::fill(std::begin(co_processIdRegs), std::end(co_processIdRegs), 0);
    // std::fill(std::begin(co_implDefinedAndDebugRegs), std::end(co_implDefinedAndDebugRegs), 0);
}
// ==================================================================================================
busPayload ARM946ES::readBus(uint32_t address, uint32_t size, bool codeRead) {
    uint32_t& previousAddr = codeRead ? previousCodeAddr : previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is one word, or one
     * halfword greater than the address used in the preceding cycle
     */
    bool sequencial = address == previousAddr + 4 || address == previousAddr + 2;
    // Update the previous address to this read to keep track of sequencial accesses.
    previousAddr = address;

    uint8_t memRegion = (address) >> 24;
    bool skipCacheAndPU = false;

    // Override default behaviour for special cases.
    // Instruction TCM.
    if (co_controlReg.itcmEnable && !co_controlReg.itcmLoadEnable && address >= itcmBase &&
        address < (itcmBase + itcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
        skipCacheAndPU = true;  // TCM has priority over cache.
    }
    // Data TCM.
    if (co_controlReg.dtcmEnable && !co_controlReg.dtcmLoadEnable && address >= dtcmBase &&
        address < (dtcmBase + dtcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
        skipCacheAndPU = true;  // TCM has priority over cache.
    }

    // Handle the Protection unit / Cache if enabled.
    // PU is skipped if accessing TCM.
    if (!skipCacheAndPU && co_controlReg.puEnable) {
        // Handle code reads.
        if (codeRead) {
            // TODO!!!: Check permissions.
            // Check if the instruction cache is active and if the address is cacheable.
            if (co_controlReg.instructionCacheEnable && isCacheable(true, address)) {
                return readFromInstructionCache(address, size);
            }
        }
        // Handle data reads.
        else {
            // TODO!!!: Check permissions.
            // Check if the data cache is active and if the address is cacheable.
            if (co_controlReg.dataCacheEnable && isCacheable(false, address)) {
                return readFromDataCache(address, size);
            }
        }
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
            // 8bit data accesses have the same timings as 16bit data.
            // There are no code 8 bit accesses.
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        default:
            LogError(size << " bit reads are currently unsupported");
            return {0, 0, 0};
    }
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
    // Update the previous address to this write to keep track of sequencial accesses.
    previousAddr = address;

    uint8_t memRegion = (address) >> 24;
    bool skipCacheAndPU = false;

    // Override default behaviour for special cases.
    // Instruction TCM.
    if (co_controlReg.itcmEnable && address >= itcmBase && address < (itcmBase + itcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
        skipCacheAndPU = true;  // TCM has priority over cache.
    }
    // Data TCM.
    if (co_controlReg.dtcmEnable && address >= dtcmBase && address < (dtcmBase + dtcmVirtSize)) {
        memRegion = ARM9MemoryRegionNum::TCM;
        skipCacheAndPU = true;  // TCM has priority over cache.
    }

    // Handle the Protection unit / Cache if enabled.
    // PU is skipped if accessing TCM.
    if (!skipCacheAndPU && co_controlReg.puEnable) {
        // TODO!!!: Check permissions.
        // Check if the data cache is active and if the address is cacheable.
        if (co_controlReg.dataCacheEnable && isCacheable(false, address)) {
            // Check to see if this should write-through.
            int puRegion = findHighestPriorityInstructionPURegion(address);
            assert(puRegion >= 0);
            bool bufferble = readBit(co_puDataWriteBufferabilityBits, puRegion);
            // Write to the cache.
            busPayload cachePayload = writeToDataCache(address, data, size, bufferble);
            // If write back, return now without updating memory.
            if (bufferble) return cachePayload;
            // Else fall-trough and perform a write-through.
        }
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

    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}
// ==================================================================================================
cycles ARM946ES::cycle() {
    cycles cyclesRan = 0;
    justBranched = false;
    justHitBreakpoint = false;

    // Handle halted or stopped states.
    if (state != ProcessorStates::Running) {
        cyclesRan = underflowSafeSubtract(targetCycle, currentCycle);
        // Increment the cpu's current cycles based on the number of cycles elapsed.
        currentCycle += cyclesRan;
        addCyclesElapsed();

        // Handle execution limit.
        if (hasExecutionLimit && executionLimit == 0) {
            hasExecutionLimit = false;
            justHitBreakpoint = true;
        }

        return cyclesRan;
    };

    // CPU is running execute like normal.
    while (currentCycle < targetCycle) {
        // Handle execution limit.
        if (hasExecutionLimit && executionLimit == 0) {
            hasExecutionLimit = false;
            justHitBreakpoint = true;
            break;
        }

        // Clear just branched indicator.
        justBranched = false;
        // Clear the just hit a breakpoint indicator.
        justHitBreakpoint = false;

        // Advance instuction pipeline.
        if (instuctionPipeLine[0].inst == NO_INSTRUCT) {
            advanceInstructionPipeline();
        }

        // Check if we hit a break point.
        if (instuctionPipeLine[0].inst != NO_INSTRUCT && !justHitBreakpoint &&
            breakpoints.contains(instuctionPipeLine[0].addr)) {
            justHitBreakpoint = true;
            break;
        }

        // Execute Stage.
        if (executeCooldown == 0 && instuctionPipeLine[0].inst != NO_INSTRUCT &&
            (instuctionPipeLine[1].inst != NO_INSTRUCT ||
             instuctionPipeLine[2].inst != NO_INSTRUCT)) {
            // Instruction is on the execute stage, execute a new instuction.
            executeCooldown = execute();
            // Decrement execution limit.
            if (hasExecutionLimit && executionLimit >= 0) {
                executionLimit--;
            }
        }

        // Fetch Stage.
        if (fetchCooldown == 0 && instuctionPipeLine[2].inst == NO_INSTRUCT &&
            (instuctionPipeLine[1].inst == NO_INSTRUCT ||
             instuctionPipeLine[0].inst == NO_INSTRUCT)) {
            // Instruction pipeline has space, fetch a new instuction.
            fetchCooldown = fetch();
        }

        // Determine how much to progress the CPU's cycle counter.
        if (fetchCooldown > 0 && executeCooldown > 0) {
            cyclesElapsed = std::min(fetchCooldown, executeCooldown);
        } else if (fetchCooldown > 0) {
            cyclesElapsed = fetchCooldown;
        } else if (executeCooldown > 0) {
            cyclesElapsed = executeCooldown;
        } else {
            // Catch cycles where no work is done -> filling pipeline.
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
    uint8_t virtSizeShift = readBits(data, 1, 5);
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
    uint8_t virtSizeShift = readBits(data, 1, 5);
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
        instructionCache[i].lfsr = 0xACE1;  // Seed the lfsr.
        instructionCache[i].rrCounter = 0;

        instructionCache[i].ways[0].valid = false;
        instructionCache[i].ways[1].valid = false;
        instructionCache[i].ways[2].valid = false;
        instructionCache[i].ways[3].valid = false;
        // All other data can stay as is as long as valid is false;
    }
}
// ===================================================================================================
void ARM946ES::invalidateInstructionCacheLine(uint32_t address, bool setIndexAddressing) {
    // Handle direct set index addressing.
    if (setIndexAddressing) {
        uint32_t set = (address >> 5) & 0x3F;
        uint32_t way = (address >> 30) & 3;
        instructionCache[set].ways[way].valid = false;
        return;
    }
    // Else use the address to calculate a tag.
    uint32_t set = readBits(address, 5, 10);
    uint32_t tag = readBits(address, 11, 31);
    // Look for the desired set / tag and mark as invalid.
    ARM946ES_CacheSet& cacheSet = instructionCache[set];
    for (unsigned int i = 0; i < ARM946ES_Cache_LinesPerSet; i++) {
        ARM946ES_CacheLine& line = cacheSet.ways[i];
        if (line.tag != tag) continue;
        if (!line.valid) continue;
        line.valid = false;
        return;
    }
}
// ===================================================================================================
void ARM946ES::invalidateDataCache() {
    for (uint32_t i = 0; i < ARM946ES_DataCache_NumSets; i++) {
        dataCache[i].lfsr = 0xACE1;  // Seed the lfsr.
        dataCache[i].rrCounter = 0;

        dataCache[i].ways[0].valid = false;
        dataCache[i].ways[1].valid = false;
        dataCache[i].ways[2].valid = false;
        dataCache[i].ways[3].valid = false;
        // All other data can stay as is as long as valid is false;
    }
}
// ==================================================================================================
void ARM946ES::invalidateDataCacheLine(uint32_t address, bool setIndexAddressing) {
    // Handle direct set index addressing.
    if (setIndexAddressing) {
        uint32_t set = (address >> 5) & 0x1F;
        uint32_t way = (address >> 30) & 3;
        dataCache[set].ways[way].valid = false;
        return;
    }
    // Else use the address to calculate a tag.
    uint32_t set = readBits(address, 5, 9);
    uint32_t tag = readBits(address, 10, 31);
    // Look for the desired set / tag and mark as invalid.
    ARM946ES_CacheSet& cacheSet = dataCache[set];
    for (unsigned int i = 0; i < ARM946ES_Cache_LinesPerSet; i++) {
        ARM946ES_CacheLine& line = cacheSet.ways[i];
        if (line.tag != tag) continue;
        if (!line.valid) continue;
        line.valid = false;
        return;
    }
}
// ==================================================================================================
void ARM946ES::cleanDataCacheLine(uint32_t address, busPayload& workPayload,
                                  bool setIndexAddressing) {
    const int NUM_BYTES_PER_WORD = sizeof(uint32_t);

    ARM946ES_CacheLine* line = nullptr;
    uint32_t targetAddress = address;
    // Handle direct set index addressing.
    if (setIndexAddressing) {
        uint32_t set = (address >> 5) & 0x1F;
        uint32_t way = (address >> 30) & 3;
        line = &dataCache[set].ways[way];
        // Reconstruct the target address.
        targetAddress = (line->tag << 10) | (set << 5);
    } else {
        // Look for the desired set / tag.
        uint32_t set = readBits(address, 5, 9);
        uint32_t tag = readBits(address, 10, 31);

        bool foundLine = false;

        ARM946ES_CacheSet& cacheSet = dataCache[set];
        for (unsigned int i = 0; i < ARM946ES_Cache_LinesPerSet; i++) {
            ARM946ES_CacheLine& nextLine = cacheSet.ways[i];
            if (nextLine.tag != tag) continue;
            if (!nextLine.valid) continue;

            line = &nextLine;
            foundLine = true;

            break;
        }

        if (!foundLine) return;
    }
    if (line == nullptr || !line->dirty) return;

    uint32_t baseAddress = address & 0xFFFFFFE0;
    uint8_t memRegion = (baseAddress) >> 24;

    // Write back all data in the line to memory.
    for (uint32_t i = 0; i < ARM946ES_CacheLine_Bytes / NUM_BYTES_PER_WORD; i++) {
        uint32_t offset = i * NUM_BYTES_PER_WORD;
        uint32_t targetAddress = baseAddress + offset;
        uint32_t oldData = reinterpret_cast<uint32_t*>(line->data)[i];
        bus->write32ARM9(targetAddress, oldData);
    }
    line->dirty = false;

    workPayload.numCycles += cacheWriteBackPenaltyTimings[memRegion];
}
// ==================================================================================================
bool ARM946ES::isCacheable(bool isInstruction, uint32_t address) {
    if (isInstruction) {
        int puRegion = findHighestPriorityInstructionPURegion(address);
        if (puRegion < 0) return false;
        return readBit(co_puInstructionCachabilityBits, puRegion);
    }

    int puRegion = findHighestPriorityDataPURegion(address);
    if (puRegion < 0) return false;
    return readBit(co_puDataUnifiedCachabilityBits, puRegion);
}
// ==================================================================================================
ARM946ES_CacheLine& ARM946ES::findCacheLineToReplace(ARM946ES_CacheSet& set) {
    // First look for any free / invalid line.
    int targetLine = -1;
    for (unsigned int i = 0; i < ARM946ES_Cache_LinesPerSet; i++) {
        if (set.ways[i].valid) continue;
        targetLine = i;
        break;
    }
    // No free line, we need to evict.
    if (targetLine < 0) {
        if (co_controlReg.cacheReplacement) {  // Round-Robin replacement method.
            // Round-robin
            targetLine = set.rrCounter % ARM946ES_Cache_LinesPerSet;
            set.rrCounter = +1;

        } else {  // Else, Pseudo Random replacement method.
            targetLine = set.lfsr % ARM946ES_Cache_LinesPerSet;
            // Update the lfsr.
            uint16_t bit = (readBit(set.lfsr, 0) ^ readBit(set.lfsr, 2) ^ readBit(set.lfsr, 3) ^
                            readBit(set.lfsr, 5)) &
                           1;
            set.lfsr = (set.lfsr >> 1) | (bit << 15);
        }
    }
    return set.ways[targetLine];
}
// ==================================================================================================
busPayload ARM946ES::fillCacheLine(ARM946ES_CacheLine& cacheLine, uint32_t address) {
    const int NUM_BYTES_PER_WORD = sizeof(uint32_t);

    busPayload payload = {0, 0, 0};
    uint32_t baseAddress = address & 0xFFFFFFE0;
    uint8_t memRegion = (address) >> 24;

    bool writeBack = false;
    if (cacheLine.valid && cacheLine.dirty) {
        // Cache line is dirty, need to write back to memory.
        writeBack = true;
        // Update the cycle penalty.
        payload.numCycles += cacheWriteBackPenaltyTimings[memRegion];
    }
    // Update the cycle penalty.
    payload.numCycles += cacheMissPenaltyTimings[memRegion];

    // Fill the cache line's data.
    for (uint32_t i = 0; i < ARM946ES_CacheLine_Bytes / NUM_BYTES_PER_WORD; i++) {
        uint32_t offset = i * NUM_BYTES_PER_WORD;
        uint32_t targetAddress = baseAddress + offset;
        // Replace the cache data.
        if (writeBack) {
            uint32_t oldData = reinterpret_cast<uint32_t*>(cacheLine.data)[i];
            bus->write32ARM9(targetAddress, oldData);
        }
        uint32_t data = bus->read32ARM9(targetAddress);
        // Copy the data from the bus into the cache.
        reinterpret_cast<uint32_t*>(cacheLine.data)[i] = data;
    }

    // Update metadata.
    cacheLine.dirty = 0;
    cacheLine.valid = 1;
    cacheLine.tag = baseAddress >> 11;

    return payload;
}
// ==================================================================================================
busPayload ARM946ES::readFromCacheSet(ARM946ES_CacheSet& cacheSet, uint32_t address, uint32_t size,
                                      uint32_t tag, uint32_t offset) {
    const uint32_t numBytes = size / 8;
    for (unsigned int i = 0; i < ARM946ES_Cache_LinesPerSet; i++) {
        ARM946ES_CacheLine& line = cacheSet.ways[i];
        if (line.tag != tag) continue;
        if (!line.valid) continue;
        // Cache hit, return the data.
        uint32_t value = 0;
        std::memcpy(&value, &line.data[offset], numBytes);
        return {.data = value,
                .numCycles = data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::TCM],
                .size = size};
    }
    // Cache miss.
    ARM946ES_CacheLine& line = findCacheLineToReplace(cacheSet);
    busPayload payload = fillCacheLine(line, address);
    std::memcpy(&payload.data, &line.data[offset], numBytes);
    payload.size = size;
    return payload;
}
// ==================================================================================================
busPayload ARM946ES::readFromDataCache(uint32_t address, uint32_t size) {
    uint32_t offset = readBits(address, 0, 4);
    uint32_t set = readBits(address, 5, 9);
    uint32_t tag = readBits(address, 10, 31);
    ARM946ES_CacheSet& cacheSet = dataCache[set];
    return readFromCacheSet(cacheSet, address, size, tag, offset);
}
// ==================================================================================================
busPayload ARM946ES::writeToDataCache(uint32_t address, uint32_t data, uint32_t size,
                                      bool writeBack) {
    const uint32_t numBytes = size / 8;

    uint32_t offset = readBits(address, 0, 4);
    uint32_t set = readBits(address, 5, 9);
    uint32_t tag = readBits(address, 10, 31);
    ARM946ES_CacheSet& cacheSet = dataCache[set];

    // First try and see if its already in the cache.
    for (unsigned int i = 0; i < ARM946ES_Cache_LinesPerSet; i++) {
        ARM946ES_CacheLine& line = cacheSet.ways[i];
        if (!line.valid) continue;
        if (line.tag != tag) continue;
        // Cache hit, update the data.
        std::memcpy(&line.data[offset], &data, numBytes);
        // Mark the line as dirty.
        line.dirty = writeBack;

        return {.data = data,
                .numCycles = data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::TCM],
                .size = size};
    }

    // Cache miss.
    ARM946ES_CacheLine& line = findCacheLineToReplace(cacheSet);
    busPayload payload = fillCacheLine(line, address);
    // Fill the new line with the passed data.
    std::memcpy(&line.data[offset], &data, numBytes);
    // Mark the line as dirty.
    line.dirty = writeBack;
    // Return the payload.
    payload.data = data;
    payload.size = size;
    return payload;
}
// ==================================================================================================
busPayload ARM946ES::readFromInstructionCache(uint32_t address, uint32_t size) {
    uint32_t offset = readBits(address, 0, 4);
    uint32_t set = readBits(address, 5, 10);
    uint32_t tag = readBits(address, 11, 31);
    ARM946ES_CacheSet& cacheSet = instructionCache[set];
    return readFromCacheSet(cacheSet, address, size, tag, offset);
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
int ARM946ES::findHighestPriorityDataPURegion(uint32_t address) {
    for (int i = ARM946ES_PU_NumRegions - 1; i >= 0; i--) {
        if (!puDataRegion[i].enabled) continue;
        if (puDataRegion[i].baseAddress > address) continue;
        if (puDataRegion[i].baseAddress + puDataRegion[i].size <= address) continue;
        return i;
    }
    return -1;
}
// ==================================================================================================
int ARM946ES::findHighestPriorityInstructionPURegion(uint32_t address) {
    for (int i = ARM946ES_PU_NumRegions - 1; i >= 0; i--) {
        if (!puInstructionRegion[i].enabled) continue;
        if (puInstructionRegion[i].baseAddress > address) continue;
        if (puInstructionRegion[i].baseAddress + puInstructionRegion[i].size <= address) continue;
        return i;
    }
    return -1;
}
// ==================================================================================================
uint32_t ARM946ES_ControlReg_Layout::read() {
    uint32_t result = 0;
    writeBit(result, 0, puEnable);
    writeBit(result, 2, dataCacheEnable);
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
    puEnable = readBit(data, 0);
    dataCacheEnable = readBit(data, 2);
    endianness = readBit(data, 7);
    if (endianness) {
        LogError("Writing ARM946E-S Endianness to Big Endian is currently unsupported");
    }
    instructionCacheEnable = readBit(data, 12);
    exceptionVectors = readBit(data, 13);
    if (exceptionVectors) {
        LogError("Writing ARM946E-S Exception Vectors to high vectors is currently unsupported");
    }
    cacheReplacement = readBit(data, 14);
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
