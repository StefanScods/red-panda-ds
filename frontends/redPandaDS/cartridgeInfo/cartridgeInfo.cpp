#include "cartridgeInfo.h"

#include "app/app.h"
#include "common.h"
#include "core/utils/utils.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
CartridgeInfo::CartridgeInfo(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent)
    : QWidget(parent), app(app), ui(new Ui::CartridgeInfo), core(core) {
    ui->setupUi(this);

    // Hook up callback functions.
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &CartridgeInfo::update);
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
}
// ==================================================================================================
CartridgeInfo::~CartridgeInfo() {
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void CartridgeInfo::update() {
    if (isHidden()) return;
    Core::NDS_Cartridge* cart = core->getNDS_Cartridge();
    // Close the widget if the cartridge is closed.
    if (!cart->isOpen()) {
        close();
        return;
    }
    const Core::CartridgeHeader& cartHeader = cart->getHeader();

    ui->gameTitleValue->setText(cartHeader.getGameTitle().c_str());
    std::string fullGameCode = "NTR-" + cartHeader.getGameCode();
    ui->gameCodeValue->setText(fullGameCode.c_str());
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
