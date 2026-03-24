#include "mainWindow.h"

#include <QCloseEvent>
#include <format>

#include "app/app.h"
#include "common.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
MainWindow::MainWindow(RedPandaDSApp* app, QWidget* parent)
    : QMainWindow(parent), app(app), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Hook up callback functions.
    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->action_viewarm7, &QAction::triggered, this,
            [this, app]() { app->openARM7Viewer(); });
    connect(ui->action_viewarm9, &QAction::triggered, this,
            [this, app]() { app->openARM9Viewer(); });
    connect(ui->actionView_Memory, &QAction::triggered, this,
            [this, app]() { app->openMemoryViewer(); });
    connect(ui->actionOpen_Disassembly, &QAction::triggered, this,
            [this, app]() { app->openDisassemblyViewer(); });
    // Start keeping track of the frametimes.
    lastFrameStartTime = std::chrono::high_resolution_clock::now();
}
// ==================================================================================================
MainWindow::~MainWindow() {
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void MainWindow::closeEvent(QCloseEvent* event) {
    QApplication::quit();
    event->accept();
}
// ==================================================================================================
void MainWindow::onEmulatorCoreUpdate() {
    // Save the FPS.
    auto endFrameTime = std::chrono::high_resolution_clock::now();
    double timeStep = std::chrono::duration<double>(endFrameTime - lastFrameStartTime).count();
    // Get the frame time going for the next iteration.
    lastFrameStartTime = std::chrono::high_resolution_clock::now();
    // Update the rolling FPS counter.
    FPS_sampleTotal -= FPS_samples[FPS_sampleIndex];
    FPS_sampleTotal += timeStep;
    FPS_samples[FPS_sampleIndex] = timeStep;
    FPS_sampleIndex = (FPS_sampleIndex + 1) % FPS_MAX_SAMPLES;
    if (FPS_sampleCount < FPS_MAX_SAMPLES) FPS_sampleCount++;
    // Compute the FPS.
    double FPS = 0;
    FPS = FPS_sampleCount / FPS_sampleTotal;
    // Update the window title.
    std::string title = "Red Panda DS - " + std::format("{:.2f}", FPS);
    setWindowTitle(title.c_str());

    // Update the screens.
    Core::NDS_LCD* lcd = app->getEmuCore()->getNDS_LCD();
    ui->topScreenWidget->setPixels(*(lcd->getTopScreenDrawBuffer()));
    ui->bottomScreenWidget->setPixels(*(lcd->getBottomScreenDrawBuffer()));
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
