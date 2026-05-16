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

    // Set up the icon viewer.
    iconGraphicsScene = new QGraphicsScene(this);
    ui->iconValue->setScene(iconGraphicsScene);
    iconImage =
        QImage(NDS_CARTRIDGE_ICON_DIMENSION, NDS_CARTRIDGE_ICON_DIMENSION, QImage::Format_ARGB32);
    iconPixmapItem = iconGraphicsScene->addPixmap(QPixmap::fromImage(iconImage));
    ui->iconValue->fitInView(iconPixmapItem, Qt::KeepAspectRatio);
    ui->iconValue->setMinimumSize(NDS_CARTRIDGE_ICON_DIMENSION, NDS_CARTRIDGE_ICON_DIMENSION);

    // Hook up callback functions.
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &CartridgeInfo::update);
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
}
// ==================================================================================================
CartridgeInfo::~CartridgeInfo() {
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void CartridgeInfo::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);
    ui->iconValue->fitInView(iconPixmapItem, Qt::KeepAspectRatio);
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

    ui->gameTitleInternalValue->setText(cartHeader.getGameTitle().c_str());
    
    // TODO!!! Handle other prefixes.
    std::string fullGameCode = "NTR-" + cartHeader.getGameCode();
    ui->gameCodeValue->setText(fullGameCode.c_str());

    ui->gameTitleEnglishValue->setText(QString::fromLatin1(cart->getEngTitleString().c_str()));

    memcpy(iconImage.bits(), cart->getIconPixels().data(),
           NDS_CARTRIDGE_ICON_DIMENSION * NDS_CARTRIDGE_ICON_DIMENSION * 4);
    iconPixmapItem->setPixmap(QPixmap::fromImage(iconImage));
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
