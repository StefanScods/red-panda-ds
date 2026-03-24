#ifndef RED_PANDA_DS_UI_MAIN_WINDOW_H
#define RED_PANDA_DS_UI_MAIN_WINDOW_H

#include <QMainWindow>
#include <chrono>

#include "mainWindow/ui_mainWindow.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class MainWindow : public QMainWindow {
public:
    MainWindow(RedPandaDSApp* app, QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onEmulatorCoreUpdate();

private:
    /**
     * @brief Override the close event hook.
     *
     * @param event The current close event.
     */
    void closeEvent(QCloseEvent* event) override;

    Ui::MainWindow* ui = nullptr;
    RedPandaDSApp* app = nullptr;

    // FPS Control.
    std::chrono::high_resolution_clock::time_point lastFrameStartTime;
    static constexpr int FPS_MAX_SAMPLES = 60;
    std::array<double, FPS_MAX_SAMPLES> FPS_samples{};
    int FPS_sampleIndex = 0;
    int FPS_sampleCount = 0;
    double FPS_sampleTotal = 0;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
