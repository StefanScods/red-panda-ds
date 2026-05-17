#ifndef RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_CONTAINER_H
#define RED_PANDA_DS_UI_DISASSEMBLY_VIEWER_CONTAINER_H

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <functional>

#include "core/core.h"

namespace RedPandaDS {
namespace UI {

namespace Processor {
enum Processor : uint8_t { arm7, arm9 };
}
namespace Format {
enum Format : uint8_t { autoDetect, arm, thumb };
}
namespace BreakpointState {
enum BreakpointState : uint8_t { clear, enabled, disabled };
}

struct DisassemblyConfiguration {
    Processor::Processor procType;
    Format::Format formatType;
};

// Forward delcare.
class RedPandaDSApp;

class DisassemblyViewerEntry : public QWidget {
public:
    DisassemblyViewerEntry(RedPandaDSApp* app, QGridLayout* parentLayout, uint32_t lineId,
                           std::function<void(uint32_t)> toggleBreakpointCallbackFunc,
                           QWidget* parent = nullptr);
    ~DisassemblyViewerEntry();

    /**
     * @brief Set the target instruction to render.
     *
     * @param newAddress The address to assign
     * @param validInstruction Whether the instruction is valid.
     * @param newInstruction The new instruction to assign
     * @param newInstructionSize The size of the instruction (2 or 4 bytes).
     * @param armMode True if the instruction is an ARM instruction.
     */
    void setTargetInstruction(uint32_t newAddress, bool validInstruction, uint32_t newInstruction,
                              uint32_t newInstructionSize, bool armMode);
    /**
     * @brief Callback function to refresh the widget with new content from the emulator core.
     */
    void update();

    void setBreakpointState(BreakpointState::BreakpointState state);

    void breakpointButtonToggle();

private:
    bool isValidInstruction = 0;
    uint32_t targetAddress = 0;
    uint32_t targetInstruction = 0;
    uint32_t targetInstructionSize = 0;
    bool isARMModeInstruction = 0;

    bool prevIsValidInstruction = 0;
    uint32_t prevTargetAddress = 0;
    uint32_t prevTargetInstruction = 0;
    uint32_t prevTargetInstructionSize = 0;
    bool prevIsARMModeInstruction = 0;

    QPushButton* breakpointButton = nullptr;
    QLabel* addressLabel = nullptr;
    QLabel* instructionBitsLabel = nullptr;
    QLabel* opcodeLabel = nullptr;
    QLabel* destinationLabel = nullptr;
    QLabel* operand1Label = nullptr;
    QLabel* operand2Label = nullptr;
    QLabel* commentLabel = nullptr;

    RedPandaDSApp* app = nullptr;
    std::function<void(uint32_t)> toggleBreakpointCallback;
};

class DisassemblyViewerContainer : public QWidget {
public:
    static const unsigned int ENTRY_LINE_PIXEL_HEIGHT = 20;
    static const unsigned int MAX_NUM_ENTRY_LINES = 100;

    DisassemblyViewerContainer(RedPandaDSApp* app, QWidget* parent = nullptr);
    ~DisassemblyViewerContainer();

    /**
     * @brief Set the disassembly configuration.
     *
     * @param newConfig Settings to assign.
     */
    void setDisassemblyConfiguration(DisassemblyConfiguration newConfig) { config = newConfig; }
    /**
     * @brief Set the width of the widget.
     *
     * @param width Width to set.
     */
    void setWidth(int width);
    /**
     * @brief Event callback for a mouse wheel event.
     *
     * @param event Event to process.
     */
    void wheelEvent(QWheelEvent* event) override;
    /**
     * @brief Sets the position of the viewer.
     *
     * @param pos Position to set.
     */
    void setPosition(uint32_t pos);
    /**
     * @brief Event callback for scroll bar on scroll event.
     *
     * @param value Event value to process.
     */
    void onScroll(int value);
    /**
     * @brief Event callback for scroll bar on scroll action event.
     *
     * @param value Event value to process.
     */
    void onScrollAction(int value);
    /**
     * @brief Callback function to refresh the widget with new content from the emulator core.
     */
    void update();

private:
    uint32_t currentPosition = 0;
    DisassemblyConfiguration config;

    QScrollBar* scrollBar = nullptr;
    QHBoxLayout* topHorizontalLayout = nullptr;
    QWidget* viewport = nullptr;
    QVBoxLayout* viewportLayout = nullptr;
    QGridLayout* contentLayout = nullptr;
    QWidget* contentWidget = nullptr;
    DisassemblyViewerEntry* entries[MAX_NUM_ENTRY_LINES] = {nullptr};
    QFrame* currentPCIndicator = nullptr;

    RedPandaDSApp* app = nullptr;
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
