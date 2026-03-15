

#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/utils/armEncode.h"
#include "core/utils/utils.h"
#include "mainWindow/mainWindow.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    RedPandaDS::UI::MainWindow window;
    window.show();
    return app.exec();
}

// Create a clean entry point for windows.
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return main(__argc, __argv);
}
#endif
