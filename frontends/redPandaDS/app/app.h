#ifndef RED_PANDA_DS_UI_APP_H
#define RED_PANDA_DS_UI_APP_H

#include <QApplication>
#include <QTimer>

#include "common.h"
#include "core/core.h"
#include "cpuViewer/cpuViewer.h"
#include "mainWindow/mainWindow.h"

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

    // Widget openers.
    void openARM7Viewer();
    void openARM9Viewer();

private:
    QApplication* app = nullptr;
    Core::DSEmuCore* core = nullptr;

    // Windows.
    MainWindow* mainWindow = nullptr;
    CPUViewer* arm7Viewer = nullptr;
    CPUViewer* arm9Viewer = nullptr;

    // Other QT components.
    QTimer* timer = nullptr;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
