#ifndef RED_PANDA_DS_UI_MEMORY_VIEWER_H
#define RED_PANDA_DS_UI_MEMORY_VIEWER_H

#include <QGraphicsOpacityEffect>

#include "core/core.h"
#include "memoryViewer/ui_memoryViewer.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class MemoryViewer : public QWidget {
public:
    MemoryViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent = nullptr);
    ~MemoryViewer();

private:
    Ui::MemoryViewer* ui = nullptr;
    const RedPandaDSApp* app = nullptr;
    Core::DSEmuCore* core = nullptr;

    /**
     * @brief Callback function to refresh the widget with new content from the emulator core.
     */
    void update();
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
