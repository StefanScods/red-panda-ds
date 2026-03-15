#include "app/app.h"
// Control print statements.
#define LOG_LEVEL 4
#include "logger.h"

int main(int argc, char** argv) {
    std::vector<char*> args;
    RedPandaDS::UI::RedPandaDSApp app(args);
    if (!app.start()) {
        return 1;
    }
    int returnCode = app.run();
    LogDebug("Exiting with return code: " << returnCode);
    app.exit();
    return returnCode;
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif
