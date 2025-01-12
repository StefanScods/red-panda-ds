#include "armEncode.h"
#include "cpu.h"
#include "interconnect.h"
#include "utils.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

int main(int argc, char **argv) {
    ARM7TDMI arm7;
    Interconnect bus;
    bus.init();
    bus.bindARM7(&arm7);

    arm7.setPC(MAIN_RAM_START);
    bus.write32ARM7(MAIN_RAM_START, armEncodeASM("MOV r8, #2147483648"));
    arm7.fetchAndExecute();
    LogMsg("Contents of r8: " << arm7.readReg(8));

    return 0;
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif