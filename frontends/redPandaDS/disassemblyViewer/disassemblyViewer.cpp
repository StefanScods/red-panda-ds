#include "disassemblyViewer.h"

#include "app/app.h"
#include "common.h"
#include "core/utils/utils.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
DisassemblyViewer::DisassemblyViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent)
    : QWidget(parent), app(app), ui(new Ui::DisassemblyViewer), core(core) {
    ui->setupUi(this);

    // Hook up callback functions.
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &DisassemblyViewer::update);
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
}
// ==================================================================================================
DisassemblyViewer::~DisassemblyViewer() {
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void DisassemblyViewer::update() {
    if (isHidden()) return;
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
