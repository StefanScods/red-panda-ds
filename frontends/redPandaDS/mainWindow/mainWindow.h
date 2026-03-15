#ifndef RED_PANDA_DS_UI_MAIN_WINDOW_H
#define RED_PANDA_DS_UI_MAIN_WINDOW_H

#include <QApplication>
#include <QMainWindow>

#include "mainWindow/ui_mainWindow.h"

namespace RedPandaDS {
namespace UI {

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() { delete ui; }

private:
    Ui::MainWindow* ui;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
