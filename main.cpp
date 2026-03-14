#include "armEncode.h"
#include "cpu.h"
#include "interconnect.h"
#include "profiler.h"
#include "utils.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

int main(int argc, char** argv) {
    Interconnect bus;
    bus.init();
    ARM7TDMI arm7;
    arm7.reset();
    bus.bindARM7(&arm7);
    ARM7TDMI arm9;
    arm9.reset();
    bus.bindARM7(&arm9);

    // writeProgramToMemory(
    //     ".arm:\n"
    //     "MOVs R0, #1\n",
    //     MAIN_RAM_START, &bus, arm9.isARM7());

    // writeProgramToMemory(
    //     "MOVs R0, #1\n",
    //     MAIN_RAM_START, &bus, arm9.isARM7());
    arm7.setThumbMode(1);
    writeProgramToMemory(
        ".thumb\n"
        "mov r0, r0\n"
        "mov r0, r0\n"
        "b .\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    LogMsg((uint32_t)arm7.readReg(0));

    return 0;
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif