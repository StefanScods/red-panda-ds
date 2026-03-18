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
    timer = new QTimer(app);
}
// ==================================================================================================
RedPandaDSApp::~RedPandaDSApp() {
    DELETE_DYNAMIC_POINTER(core);
    DELETE_DYNAMIC_POINTER(timer);
    DELETE_DYNAMIC_POINTER(mainWindow);
    DELETE_DYNAMIC_POINTER(arm7Viewer);
    DELETE_DYNAMIC_POINTER(arm9Viewer);
    DELETE_DYNAMIC_POINTER(memoryViewer);
    DELETE_DYNAMIC_POINTER(disassemblyViewer);
    DELETE_DYNAMIC_POINTER(app);
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

    timer->start(APPLICATION_REFRESH_RATE);

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
void RedPandaDSApp::openARM7Viewer() {
    if (arm7Viewer == nullptr) {
        arm7Viewer = new CPUViewer(this, core->getARM7Core());
    }
    arm7Viewer->isHidden() ? arm7Viewer->show() : arm7Viewer->raise();
}
void RedPandaDSApp::openARM9Viewer() {
    if (arm9Viewer == nullptr) {
        arm9Viewer = new CPUViewer(this, core->getARM9Core());
    }
    arm9Viewer->isHidden() ? arm9Viewer->show() : arm9Viewer->raise();
}
void RedPandaDSApp::openDisassemblyViewer() {
    if (disassemblyViewer == nullptr) {
        disassemblyViewer = new DisassemblyViewer(this, core);
    }
    disassemblyViewer->isHidden() ? disassemblyViewer->show() : disassemblyViewer->raise();
}
void RedPandaDSApp::openMemoryViewer() {
    if (memoryViewer == nullptr) {
        memoryViewer = new MemoryViewer(this, core);
    }
    memoryViewer->isHidden() ? memoryViewer->show() : memoryViewer->raise();
}
// ==================================================================================================

}  // namespace UI
}  // namespace RedPandaDS
