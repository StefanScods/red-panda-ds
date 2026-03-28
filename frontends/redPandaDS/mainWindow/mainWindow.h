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
    /**
     * @brief Override the resize event hook.
     *
     * @param event The current resize event.
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief The main logic function for applying the layout to the main widget.
     */
    void layoutMainWidget();
    /**
     * @brief Set the screen size factor to the main widget.
     *
     * @param targetFactor The factor to set (0-4).
     */
    void setScreenSizeFactor(uint32_t targetFactor);
    /**
     * @brief Set the screen (LCD) layout as vertical or horizontal
     *
     * @param horizontalLayout True to apply horizontal layout. False to apply a vertical layout.
     */
    void setLCDLayout(bool horizontalLayout);

    Ui::MainWindow* ui = nullptr;
    RedPandaDSApp* app = nullptr;

    uint32_t screenSizeFactor = 0;
    uint32_t screenGap = 5;
    bool screenHorizontalLayout = true;

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
