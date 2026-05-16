#ifndef RED_PANDA_DS_UI_CARTRIDGE_INFO_H
#define RED_PANDA_DS_UI_CARTRIDGE_INFO_H

#include <QGraphicsOpacityEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>

#include "cartridgeInfo/ui_cartridgeInfo.h"
#include "core/core.h"

namespace RedPandaDS {
namespace UI {

// Forward delcare.
class RedPandaDSApp;

class CartridgeInfo : public QWidget {
public:
    CartridgeInfo(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent = nullptr);
    ~CartridgeInfo();

private:
    Ui::CartridgeInfo* ui = nullptr;
    const RedPandaDSApp* app = nullptr;
    Core::DSEmuCore* core = nullptr;

    QImage iconImage;
    QGraphicsScene* iconGraphicsScene = nullptr;
    QGraphicsPixmapItem* iconPixmapItem = nullptr;

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
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
