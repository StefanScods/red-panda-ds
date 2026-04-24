#ifndef RED_PANDA_DS_UI_APP_H
#define RED_PANDA_DS_UI_APP_H

#include <QApplication>
#include <QTimer>
#include <array>
#include <chrono>
#include <thread>

#include "common.h"
#include "core/core.h"
#include "cpuViewer/cpuViewer.h"
#include "disassemblyViewer/disassemblyViewer.h"
#include "mainWindow/mainWindow.h"
#include "memoryViewer/memoryViewer.h"

namespace RedPandaDS {
namespace UI {

#define APPLICATION_REFRESH_RATE 15  // ms

class RedPandaDSApp {
public:
    RedPandaDSApp(std::vector<char*>& args);
    ~RedPandaDSApp();

    /**
     * @brief Starts the application. Returns `false` upon failure.
     *
     * @return `bool`
     */
    bool start();
    /**
     * @brief Main execution loop for the application. Returns a non-zero int upon failure.
     *
     * @return `int`
     */
    int run();
    /**
     * @brief Ends the application. Returns `false` upon failure.
     *
     * @return `int`
     */
    bool exit();
    /**
     * @brief Main execution loop of the emulator. Drives the emulator core at a fixed FPS.
     */
    void emulationThreadBody();
    /**
     * @brief Fetch the emulated core.
     *
     * @return Core::DSEmuCore*
     */
    Core::DSEmuCore* getEmuCore() { return core; }
    /**
     * @brief Fetch the QT event timer for refreshing widgets.
     *
     * @return QTimer*
     */
    QTimer* getRefreshTimer() { return timer; }

    /**
     * @brief Main callback function for the fronted to handle changes to the core execution mode.
     */
    void handleCoreExecutionModeChange();

    // Widget openers.
    void openARM7Viewer();
    void openARM9Viewer();
    void openMemoryViewer();
    void openDisassemblyViewer();

signals:
    void emulatorCoreUpdate();

private:
    QApplication* app = nullptr;
    Core::DSEmuCore* core = nullptr;

    // Windows.
    MainWindow* mainWindow = nullptr;
    CPUViewer* arm7Viewer = nullptr;
    CPUViewer* arm9Viewer = nullptr;
    MemoryViewer* memoryViewer = nullptr;
    DisassemblyViewer* disassemblyViewer = nullptr;

    // Other QT components.
    QTimer* timer = nullptr;

    std::thread emulationThread;
    bool running = false;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
