#include "app.h"

#include <QFile>
#include <QFontDatabase>

#include "core/events.h"

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
    assert(!running);
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
    QFile fontFile(":/PixelCode.ttf");
    if (!fontFile.open(QIODevice::ReadOnly)) {
        LogWarning("Could not load application font!");
    } else {
        QByteArray fontData = fontFile.readAll();
        int id = QFontDatabase::addApplicationFontFromData(fontData);
        fontFile.close();
    }
    // Apply a global style sheet to the entire application.
    QFile styleSheetFile(":/main.qss");
    if (!styleSheetFile.open(QFile::ReadOnly)) {
        LogError("Could not load application style sheet!");
        return false;
    }
    QString styleSheetContent = QLatin1String(styleSheetFile.readAll());
    app->setStyleSheet(styleSheetContent);

    return true;
}
// ==================================================================================================
int RedPandaDSApp::run() {
    // Initialize the emulator.
    core->init();

    timer->start(APPLICATION_REFRESH_RATE);

    // Start the emulation thread.
    running = true;
    emulationThread = std::thread(&RedPandaDSApp::emulationThreadBody, this);

    // Show the main window.
    mainWindow->show();
    return app->exec();
}
// ==================================================================================================
bool RedPandaDSApp::exit() {
    LogMsg("Exiting RedPandaDS...");
    // Exit the emulation thread.
    running = false;
    if (emulationThread.joinable()) {
        emulationThread.join();
    }
    return true;
}
// ==================================================================================================
void RedPandaDSApp::emulationThreadBody() {
    LogDebug("Starting emulation thread.");
    auto targetTime = std::chrono::high_resolution_clock::now();

    // Add the first frame to the event queue.
    core->addEventToQueue<Core::StandardFrameEvent>(0);
    // For now set PC to main ram.
    core->getARM7Core()->setPC(MAIN_RAM_START);
    core->getARM9Core()->setPC(MAIN_RAM_START);

    uint32_t aaaaa = 0;
    while (running) {
        targetTime += FPS_targetFrameTime;

        // Do work.
        core->runApplicationFrame();

        Core::NDS_LCD* lcd = core->getNDS_LCD();
        auto& bottom = *lcd->getBottomScreenWorkBuffer();
        auto& top = *lcd->getTopScreenWorkBuffer();

        for (uint32_t i = 0; i < Core::DS_LCD_WIDTH; i++) {
            for (uint32_t j = 0; j < Core::DS_LCD_HEIGHT; j++) {
                uint32_t index = j * Core::DS_LCD_WIDTH + i;
                bottom[index] = 0xff000000;
                top[index] = 0xff000000;
            }
        }

        lcd->swapBuffers();

        uint32_t x = aaaaa % Core::DS_LCD_WIDTH;
        uint32_t y = aaaaa % Core::DS_LCD_HEIGHT;
        aaaaa++;

        top[(50) * Core::DS_LCD_WIDTH + (x)] = 0xff00ffff;
        top[(50 + 1) * Core::DS_LCD_WIDTH + (x)] = 0xff00ffff;
        top[(50) * Core::DS_LCD_WIDTH + (x + 1)] = 0xff00ffff;
        top[(50 + 1) * Core::DS_LCD_WIDTH + (x + 1)] = 0xff00ffff;

        bottom[(y)*Core::DS_LCD_WIDTH + (50)] = 0xff00ffff;
        bottom[(y + 1) * Core::DS_LCD_WIDTH + (50)] = 0xff00ffff;
        bottom[(y)*Core::DS_LCD_WIDTH + (50 + 1)] = 0xff00ffff;
        bottom[(y + 1) * Core::DS_LCD_WIDTH + (50 + 1)] = 0xff00ffff;

        // auto start = std::chrono::high_resolution_clock::now();
        // auto target = start + std::chrono::microseconds(15000);
        // while (std::chrono::high_resolution_clock::now() < target) {
        //     std::this_thread::yield();
        // }

        // Target 60 FPS.
        std::this_thread::sleep_until(targetTime);
        // Let the rest of the emulator know the core has finished a "frame".
        QMetaObject::invokeMethod(
            mainWindow, [this] { mainWindow->onEmulatorCoreUpdate(); }, Qt::QueuedConnection);
    }
    LogDebug("Exiting emulation thread.");
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
