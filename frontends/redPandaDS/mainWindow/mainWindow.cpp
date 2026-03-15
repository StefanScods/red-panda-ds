#include "mainWindow.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}
// ==================================================================================================

}  // namespace UI
}  // namespace RedPandaDS
