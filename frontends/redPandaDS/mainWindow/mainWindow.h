#ifndef RED_PANDA_DS_UI_MAIN_WINDOW_H
#define RED_PANDA_DS_UI_MAIN_WINDOW_H

#include <QMainWindow>

#include "mainWindow/ui_mainWindow.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class MainWindow : public QMainWindow {
public:
    MainWindow(RedPandaDSApp* app, QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui = nullptr;
    const RedPandaDSApp* app = nullptr;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
