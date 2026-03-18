#ifndef RED_PANDA_DS_UI_CPU_VIEWER_H
#define RED_PANDA_DS_UI_CPU_VIEWER_H

#include <QGraphicsOpacityEffect>

#include "core/cpu.h"
#include "cpuViewer/ui_cpuViewer.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class CPUViewer : public QWidget {
public:
    CPUViewer(RedPandaDSApp* app, Core::ARM* cpu, QWidget* parent = nullptr);
    ~CPUViewer();

    /**
     * @brief Callback function to refresh the widget with new content from the emulator core.
     */
    void update();
    /**
     * @brief Callback function to open the detailed view window.
     */
    void toggleDetailedView();

private:
    Ui::CPUViewer* ui = nullptr;
    const RedPandaDSApp* app = nullptr;
    Core::ARM* cpu = nullptr;

    QGraphicsOpacityEffect* flagNEffect = nullptr;
    QGraphicsOpacityEffect* flagZEffect = nullptr;
    QGraphicsOpacityEffect* flagCEffect = nullptr;
    QGraphicsOpacityEffect* flagVEffect = nullptr;
    QGraphicsOpacityEffect* flagQEffect = nullptr;

    bool detailedView = true;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
