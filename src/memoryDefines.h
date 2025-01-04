
#ifndef MEMORYDEFINES_H
#define MEMORYDEFINES_H

#define MAIN_RAM_START 0x02000000
#define MAIN_RAM_SIZE 0x00400000  // 4 Mb
#define MAIN_RAM_MASK 0x003FFFFF

namespace ARM7MemoryRegionNum {
enum ARM7MemoryRegionNum {
    BIOS = 0x00,
    MAIN_RAM = 0x02,
    WRAM = 0x03,
    IO = 0x04,
    VRAM = 0x06,
    GBAROM1 = 0x08,
    GBAROM2 = 0x09,
    GBARAM = 0x0A
};
}
#endif