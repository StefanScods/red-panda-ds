#include "armEncode.h"
#include "cpu.h"
#include "interconnect.h"
#include "profiler.h"
#include "utils.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

int main(int argc, char** argv) {
    ARM7TDMI arm7;
    Interconnect bus;
    bus.init();
    bus.bindARM7(&arm7);
    arm7.reset();

    bus.write32ARM7(MAIN_RAM_START, armEncodeASM("MOV R1, #0x80000000\n")[0]);
    bus.write32ARM7(MAIN_RAM_START + 4, armEncodeASM("TEQ R1, #0\n")[0]);
    // bus.write32ARM7(MAIN_RAM_START + 8, armEncodeASM("MOVs R0, R1, LSL R2\n")[0]);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);

    LogMsg(std::hex << arm7.readFlag(Z_FLAG));

    return 0;
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif