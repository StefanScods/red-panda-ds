#ifndef RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_H
#define RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_H

#include <QGraphicsOpacityEffect>

#include "core/core.h"
#include "disassemblyViewer/ui_disassemblyViewer.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class DisassemblyViewer : public QWidget {
public:
    DisassemblyViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent = nullptr);
    ~DisassemblyViewer();

private:
    Ui::DisassemblyViewer* ui = nullptr;
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
