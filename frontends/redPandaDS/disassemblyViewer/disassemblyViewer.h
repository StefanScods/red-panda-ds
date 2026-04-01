#ifndef RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_H
#define RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_H

#include <QScrollArea>
#include <QCloseEvent>

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
     * @brief Override the close event hook.
     *
     * @param event The current close event.
     */
    void closeEvent(QCloseEvent* event) override;

    /**
     * @brief Callback function to refresh the widget with new content from the emulator core.
     */
    void update();

    /**
     * @brief Moves the disassembler viewer container to the active CPU's PC.
     */
    void goToPC();

    /**
     * @brief Toggles emulator core's execution status. 
     */
    void togglePaused();
    
    /**
     * @brief Callback function for stepping the CPU. 
     */
    void stepCPU();

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
