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

    bus.write32ARM7(MAIN_RAM_START, armEncodeASM("MOV R2, #0x00000005\n")[0]);
    bus.write32ARM7(MAIN_RAM_START + 4, armEncodeASM("MOV R1, #0x00FF0000\n")[0]);
    bus.write32ARM7(MAIN_RAM_START + 8, armEncodeASM("MOVs R0, R1, LSL R2\n")[0]);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);

    LogMsg(std::hex << arm7.readReg(0));

    return 0;
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif