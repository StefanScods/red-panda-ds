#include "mainWindow.h"

#include <QCloseEvent>

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
}  // namespace UI
}  // namespace RedPandaDS
