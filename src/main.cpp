// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

int main(int argc, char **argv) {
    LogMsg("Hello World!");

    return 0;
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif