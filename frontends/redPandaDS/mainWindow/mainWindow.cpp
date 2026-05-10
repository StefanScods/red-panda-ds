#include "mainWindow.h"

#include <QCloseEvent>
#include <QResizeEvent>
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
    ui->topScreenWidget->setParent(ui->centralwidget);
    ui->bottomScreenWidget->setParent(ui->centralwidget);
    ui->topScreenWidget->setWindowFlags(Qt::Widget);
    ui->bottomScreenWidget->setWindowFlags(Qt::Widget);
    ui->topScreenWidget->show();
    ui->bottomScreenWidget->show();

    // Hook up callback functions.

    connect(ui->actionOpen_ROM, &QAction::triggered, this, [this, app]() { app->openROM(); });

    connect(ui->actionReset, &QAction::triggered, this, [this, app]() { app->resetEmulation(); });
    connect(ui->actionPause_Emulation, &QAction::triggered, this,
            [this, app]() { app->getEmuCore()->togglePausedState(); });
    connect(ui->actionStop_Emulation, &QAction::triggered, this,
            [this, app]() { app->stopEmulation(); });

    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);

    connect(ui->action_viewarm7, &QAction::triggered, this,
            [this, app]() { app->openARM7Viewer(); });
    connect(ui->action_viewarm9, &QAction::triggered, this,
            [this, app]() { app->openARM9Viewer(); });
    connect(ui->actionView_Memory, &QAction::triggered, this,
            [this, app]() { app->openMemoryViewer(); });
    connect(ui->actionOpen_Disassembly, &QAction::triggered, this,
            [this, app]() { app->openDisassemblyViewer(); });
    connect(ui->actionOpen_Cartridge_Info, &QAction::triggered, this,
            [this, app]() { app->openCartridgeInfo(); });
    // Hook up set screen size actions  callback functions.
    connect(ui->action1x, &QAction::triggered, this, [this]() { this->setScreenSizeFactor(1); });
    connect(ui->action2x, &QAction::triggered, this, [this]() { this->setScreenSizeFactor(2); });
    connect(ui->action3x, &QAction::triggered, this, [this]() { this->setScreenSizeFactor(3); });
    connect(ui->action4x, &QAction::triggered, this, [this]() { this->setScreenSizeFactor(4); });
    connect(ui->actionHorizontal, &QAction::triggered, this,
            [this]() { this->setLCDLayout(true); });
    connect(ui->actionVertical, &QAction::triggered, this, [this]() { this->setLCDLayout(false); });

    handleCoreExecutionModeChange();
    // Set the default screen size and layout.
    setScreenSizeFactor(4);
    setLCDLayout(true);

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
    Core::NDS_Cartridge* cart = app->getEmuCore()->getNDS_Cartridge();
    std::string title = app->APP_TITLE;
    if (cart->isOpen()) {
        title += " - " + cart->getHeader().getGameTitle() + " - " + std::format("{:.1f}", FPS);
    }
    setWindowTitle(title.c_str());

    // Update the screens.
    Core::NDS_LCD* lcd = app->getEmuCore()->getNDS_LCD();
    ui->topScreenWidget->setPixels(*(lcd->getTopScreenDrawBuffer()));
    ui->bottomScreenWidget->setPixels(*(lcd->getBottomScreenDrawBuffer()));
}
// ==================================================================================================
void MainWindow::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    layoutMainWidget();
}
// ==================================================================================================
void MainWindow::handleCoreExecutionModeChange() {
    Core::ApplicationState::ApplicationState appState = app->getEmuCore()->getState();
    switch (appState) {
        case Core::ApplicationState::stopped:
            ui->actionPause_Emulation->setText("Pause Emulation");
            ui->actionPause_Emulation->setEnabled(false);
            ui->actionReset->setEnabled(false);
            ui->actionStop_Emulation->setEnabled(false);
            ui->actionOpen_Cartridge_Info->setEnabled(false);
            break;
        case Core::ApplicationState::running:
            ui->actionPause_Emulation->setText("Pause Emulation");
            ui->actionPause_Emulation->setEnabled(true);
            ui->actionReset->setEnabled(true);
            ui->actionStop_Emulation->setEnabled(true);
            ui->actionOpen_Cartridge_Info->setEnabled(true);
            break;
        case Core::ApplicationState::paused:
            ui->actionPause_Emulation->setText("Continue Emulation");
            ui->actionPause_Emulation->setEnabled(true);
            ui->actionReset->setEnabled(true);
            ui->actionStop_Emulation->setEnabled(true);
            ui->actionOpen_Cartridge_Info->setEnabled(true);
            break;
        default:
            LogErrorPrefixed("Unsupported core state " << appState, "MainWindow");
            break;
    }
}
// ==================================================================================================
void MainWindow::layoutMainWidget() {
    int margin = 0;
    // Get the new widget size.
    QSize newSize = ui->centralwidget->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();

    int screenW, screenH;
    if (screenHorizontalLayout) {
        if (screenSizeFactor == 0) {
            // If no size factor is set, calculate the screen size based on the new size.
            setMinimumWidth(Core::DS_LCD_WIDTH * 2 + screenGap);
            setMinimumHeight(Core::DS_LCD_HEIGHT);

            const double targetAspectRatio = ((double)Core::DS_LCD_WIDTH) / Core::DS_LCD_HEIGHT;
            // Calculate the new size of the screens.
            int availW = (newWidth - 2 * margin) / 2 - screenGap;
            int availH = newHeight - 2 * margin;
            double currentAspect = ((double)availW) / availH;
            if (currentAspect > targetAspectRatio) {
                // Too wide -> height is limitor.
                screenH = std::max(availH, Core::DS_LCD_HEIGHT);
                screenW = std::floor(screenH * targetAspectRatio);
            } else {
                // Too tall -> width is limitor.
                screenW = std::max(availW, Core::DS_LCD_WIDTH);
                screenH = std::floor(screenW / targetAspectRatio);
            }
        } else {
            // Use the size factor to calculate the new window size.
            setMinimumWidth(screenSizeFactor * Core::DS_LCD_WIDTH * 2 + screenGap);
            setMinimumHeight(screenSizeFactor * Core::DS_LCD_HEIGHT);

            screenW = screenSizeFactor * Core::DS_LCD_WIDTH;
            screenH = screenSizeFactor * Core::DS_LCD_HEIGHT;
        }

        // Calculate the position to render the screens.
        int totalScreensW = 2 * screenW + screenGap;
        int x = std::max(0, (newWidth - totalScreensW) / 2);
        int y = std::max(0, (newHeight - screenH) / 2);

        // Set the screen position + size.
        ui->topScreenWidget->setGeometry(x, y, screenW, screenH);
        ui->bottomScreenWidget->setGeometry(x + screenW + screenGap, y, screenW, screenH);
    } else {
        if (screenSizeFactor == 0) {
            // If no size factor is set, calculate the screen size based on the new size.
            setMinimumWidth(Core::DS_LCD_WIDTH);
            setMinimumHeight(Core::DS_LCD_HEIGHT * 2 + screenGap);

            const double targetAspectRatio = ((double)Core::DS_LCD_WIDTH) / Core::DS_LCD_HEIGHT;
            // Calculate the new size of the screens.
            int availW = (newWidth - 2 * margin);
            int availH = (newHeight - 2 * margin) / 2 - screenGap;
            double currentAspect = ((double)availW) / availH;
            if (currentAspect > targetAspectRatio) {
                // Too wide -> height is limitor.
                screenH = std::max(availH, Core::DS_LCD_HEIGHT);
                screenW = std::floor(screenH * targetAspectRatio);
            } else {
                // Too tall -> width is limitor.
                screenW = std::max(availW, Core::DS_LCD_WIDTH);
                screenH = std::floor(screenW / targetAspectRatio);
            }
        } else {
            // Use the size factor to calculate the new window size.
            setMinimumWidth(screenSizeFactor * Core::DS_LCD_WIDTH);
            setMinimumHeight(screenSizeFactor * Core::DS_LCD_HEIGHT * 2 + screenGap);

            screenW = screenSizeFactor * Core::DS_LCD_WIDTH;
            screenH = screenSizeFactor * Core::DS_LCD_HEIGHT;
        }

        // Calculate the position to render the screens.
        int totalScreensH = 2 * screenH + screenGap;
        int x = std::max(0, (newWidth - screenW) / 2);
        int y = std::max(0, (newHeight - totalScreensH) / 2);

        // Set the screen position + size.
        ui->topScreenWidget->setGeometry(x, y, screenW, screenH);
        ui->bottomScreenWidget->setGeometry(x, y + screenH + screenGap, screenW, screenH);
    }
}
// ==================================================================================================
void MainWindow::setScreenSizeFactor(uint32_t targetFactor) {
    // Clear the option if set.
    switch (screenSizeFactor) {
        case 1:
            ui->action1x->setChecked(false);
            break;
        case 2:
            ui->action2x->setChecked(false);
            break;
        case 3:
            ui->action3x->setChecked(false);
            break;
        case 4:
            ui->action4x->setChecked(false);
            break;
        default:
            break;
    }
    // Apply the new screen size factor.
    if (screenSizeFactor == targetFactor) {
        // Toggle the current setting off.
        screenSizeFactor = 0;
    } else {
        // Apply the new factor.
        screenSizeFactor = targetFactor;
    }
    // Check the option if set.
    switch (screenSizeFactor) {
        case 1:
            ui->action1x->setChecked(true);
            break;
        case 2:
            ui->action2x->setChecked(true);
            break;
        case 3:
            ui->action3x->setChecked(true);
            break;
        case 4:
            ui->action4x->setChecked(true);
            break;
        default:
            break;
    }
    // Force the new factor to take effect.
    layoutMainWidget();
}
// ==================================================================================================
void MainWindow::setLCDLayout(bool horizontalLayout) {
    screenHorizontalLayout ? ui->actionHorizontal->setChecked(false)
                           : ui->actionVertical->setChecked(false);
    screenHorizontalLayout = horizontalLayout;
    screenHorizontalLayout ? ui->actionHorizontal->setChecked(true)
                           : ui->actionVertical->setChecked(true);
    // Force the new factor to take effect.
    layoutMainWidget();
}
}  // namespace UI
}  // namespace RedPandaDS
