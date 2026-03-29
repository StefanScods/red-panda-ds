#ifndef RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_H
#define RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_H

#include <QScrollArea>

#include "core/core.h"
#include "disassemblyViewer/disassemblyViewerContainer.h"
#include "disassemblyViewer/ui_disassemblyViewer.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class DisassemblyViewer : public QWidget {
public:
    DisassemblyViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent = nullptr);
    ~DisassemblyViewer();

    /**
     * @brief Override the resize event hook.
     *
     * @param event The current resize event.
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief Moves the disassembler viewer container to the active CPU's PC.
     */
    void goToPC();

    /**
     * @brief Handles the text changed event. Moves the disassembler viewer container to the address
     * specified.
     */
    void onJumpToAddressTextBoxChanged();

private:
    DisassemblyConfiguration currentConfig;
    void setNewDisassemblyConfiguration();

    Ui::DisassemblyViewer* ui = nullptr;
    DisassemblyViewerContainer* viewer = nullptr;

    RedPandaDSApp* app = nullptr;
    Core::DSEmuCore* core = nullptr;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
