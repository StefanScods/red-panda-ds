#include "mainWindow.h"

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
}
// ==================================================================================================
MainWindow::~MainWindow() {
    if (ui != nullptr) delete ui;
    ui = nullptr;
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
