#include "memoryViewer.h"

#include "app/app.h"
#include "common.h"
#include "core/utils/utils.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
MemoryViewer::MemoryViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent)
    : QWidget(parent), app(app), ui(new Ui::MemoryViewer), core(core) {
    ui->setupUi(this);

    // Hook up callback functions.
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &MemoryViewer::update);
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
}
// ==================================================================================================
MemoryViewer::~MemoryViewer() {
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void MemoryViewer::update() {
    if (isHidden()) return;
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
