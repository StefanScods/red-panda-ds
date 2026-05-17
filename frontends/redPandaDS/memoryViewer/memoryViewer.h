#ifndef RED_PANDA_DS_UI_MEMORY_VIEWER_H
#define RED_PANDA_DS_UI_MEMORY_VIEWER_H

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
#include "memoryViewer/ui_memoryViewer.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

namespace MemoryMap {
enum MemoryMap : uint8_t { arm7, arm9 };
}
namespace MemoryFormat {
enum MemoryFormat : uint8_t { hex, decimal, ascii };
}

struct MemoryViewerConfiguration {
    MemoryMap::MemoryMap procType;
    MemoryFormat::MemoryFormat formatType;
};

class MemoryViewerContainer : public QWidget {
public:
    static const unsigned int ENTRY_LINE_PIXEL_HEIGHT = 28;
    static const unsigned int MAX_NUM_ENTRY_LINES = 100;

    MemoryViewerContainer(RedPandaDSApp* app, QWidget* parent = nullptr);
    ~MemoryViewerContainer();

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
    /**
     * @brief Helper util to get the min width of the widget.
     *
     * @return unsigned int
     */
    unsigned int getMinWidth();

    MemoryViewerConfiguration config;

private:
    RedPandaDSApp* app;

    uint32_t currentPosition = 0;

    QScrollBar* scrollBar = nullptr;
    QHBoxLayout* topHorizontalLayout = nullptr;
    QWidget* viewport = nullptr;
    QGridLayout* contentLayout = nullptr;
    QWidget* contentWidget = nullptr;

    std::vector<QLabel*> addressLabels;
    std::vector<QLabel*> valueLabels;
};

class MemoryViewer : public QWidget {
public:
    MemoryViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent = nullptr);
    ~MemoryViewer();

private:
    Ui::MemoryViewer* ui = nullptr;
    const RedPandaDSApp* app = nullptr;
    Core::DSEmuCore* core = nullptr;

    MemoryViewerContainer* viewer = nullptr;

    /**
     * @brief Update the radio buttons to reflect the state of the memory viewer configuration.
     */
    void setNewMemoryViewerConfiguration();

    /**
     * @brief Override the resize event hook.
     *
     * @param event The current resize event.
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief Callback function to refresh the widget with new content from the emulator core.
     */
    void update();

    /**
     * @brief Handles the text changed event. Moves the memory viewer container to the address
     * specified.
     */
    void onJumpToAddressTextBoxChanged();
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
