#include "app.h"

#include <QFile>

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
RedPandaDSApp::RedPandaDSApp(std::vector<char*>& args) {
    // Core set up.
    core = new Core::DSEmuCore();
    // QT set up.
    int argc = args.size();
    char** argv = args.data();
    app = new QApplication(argc, argv);
    mainWindow = new MainWindow(this);
}
// ==================================================================================================
RedPandaDSApp::~RedPandaDSApp() {
    if (core != nullptr) delete core;
    core = nullptr;
    if (app != nullptr) delete app;
    app = nullptr;
    if (mainWindow != nullptr) delete mainWindow;
    mainWindow = nullptr;
}
// ==================================================================================================
bool RedPandaDSApp::start() {
    LogMsg("Starting RedPandaDS!");
    if (app == nullptr || core == nullptr) {
        LogError("Failed to start the application!");
        return false;
    }
    // Apply a global style sheet to the entire application.
    QFile styleSheetFile(":/main.qss");
    if (!styleSheetFile.open(QFile::ReadOnly)) {
        LogError("Could not load application style sheet!");
        return false;
    }
    QString styleSheetContent = QLatin1String(styleSheetFile.readAll());
    app->setStyleSheet(styleSheetContent);

    // Initialize the emulator.
    core->init();

    // Show the main window.
    mainWindow->show();
    return true;
}
// ==================================================================================================
int RedPandaDSApp::run() {
    return app->exec();
}
// ==================================================================================================
bool RedPandaDSApp::exit() {
    LogMsg("Exiting RedPandaDS...");
    return true;
}
// ==================================================================================================

}  // namespace UI
}  // namespace RedPandaDS
