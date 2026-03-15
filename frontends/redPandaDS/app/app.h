#ifndef RED_PANDA_DS_UI_APP_H
#define RED_PANDA_DS_UI_APP_H

#include <QApplication>

#include "core/core.h"
#include "mainWindow/mainWindow.h"

namespace RedPandaDS {
namespace UI {

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

private:
    QApplication* app = nullptr;
    Core::DSEmuCore* core = nullptr;

    // Windows.
    MainWindow* mainWindow = nullptr;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
