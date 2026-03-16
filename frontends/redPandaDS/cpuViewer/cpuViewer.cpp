#include "cpuViewer.h"

#include "app/app.h"
#include "common.h"
#include "core/utils/utils.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
CPUViewer::CPUViewer(RedPandaDSApp* app, Core::ARM* cpu, QWidget* parent)
    : QWidget(parent), app(app), ui(new Ui::CPUViewer), cpu(cpu) {
    ui->setupUi(this);

    std::string windowTitle = "CPU Viewer - " + cpu->getCPUName();
    setWindowTitle(windowTitle.c_str());

    // Start with the detailed view collapsed.
    detailedView = true;
    toggleDetailedView();

    // Hook up callback functions.
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &CPUViewer::update);
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->toggleDetailedDisplayButton, &QPushButton::clicked, this,
            &CPUViewer::toggleDetailedView);

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    // Create effects for the flag labels.
    flagNEffect = new QGraphicsOpacityEffect(this);
    ui->flagNValue->setGraphicsEffect(flagNEffect);
    flagZEffect = new QGraphicsOpacityEffect(this);
    ui->flagZValue->setGraphicsEffect(flagZEffect);
    flagCEffect = new QGraphicsOpacityEffect(this);
    ui->flagCValue->setGraphicsEffect(flagCEffect);
    flagVEffect = new QGraphicsOpacityEffect(this);
    ui->flagVValue->setGraphicsEffect(flagVEffect);
    flagQEffect = new QGraphicsOpacityEffect(this);
    ui->flagQValue->setGraphicsEffect(flagQEffect);

    // Hide the Q flag when not viewing an ARM9 core.
    if (!cpu->isARM9()) {
        ui->flagQValue->hide();
    }
}
// ==================================================================================================
CPUViewer::~CPUViewer() {
    DELETE_DYNAMIC_POINTER(flagNEffect);
    DELETE_DYNAMIC_POINTER(flagZEffect);
    DELETE_DYNAMIC_POINTER(flagCEffect);
    DELETE_DYNAMIC_POINTER(flagVEffect);
    DELETE_DYNAMIC_POINTER(flagQEffect);
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void CPUViewer::update() {
    if (isHidden()) return;

    ui->r0Val->setText(Core::hexString(cpu->readReg(0)).c_str());
    ui->r1Val->setText(Core::hexString(cpu->readReg(1)).c_str());
    ui->r2Val->setText(Core::hexString(cpu->readReg(2)).c_str());
    ui->r3Val->setText(Core::hexString(cpu->readReg(3)).c_str());
    ui->r4Val->setText(Core::hexString(cpu->readReg(4)).c_str());
    ui->r5Val->setText(Core::hexString(cpu->readReg(5)).c_str());
    ui->r6Val->setText(Core::hexString(cpu->readReg(6)).c_str());
    ui->r7Val->setText(Core::hexString(cpu->readReg(7)).c_str());
    ui->r8Val->setText(Core::hexString(cpu->readReg(8)).c_str());
    ui->r9Val->setText(Core::hexString(cpu->readReg(9)).c_str());
    ui->r10Val->setText(Core::hexString(cpu->readReg(10)).c_str());
    ui->r11Val->setText(Core::hexString(cpu->readReg(11)).c_str());
    ui->r12Val->setText(Core::hexString(cpu->readReg(12)).c_str());
    ui->r13Val->setText(Core::hexString(cpu->readReg(13)).c_str());
    ui->r14Val->setText(Core::hexString(cpu->readReg(14)).c_str());
    ui->r15Val->setText(Core::hexString(cpu->readReg(15)).c_str());

    ui->cpsrVal->setText(Core::hexString(cpu->readCPSR()).c_str());

    cpu->readFlag(N_FLAG) ? flagNEffect->setOpacity(1) : flagNEffect->setOpacity(0.5);
    cpu->readFlag(Z_FLAG) ? flagZEffect->setOpacity(1) : flagZEffect->setOpacity(0.5);
    cpu->readFlag(C_FLAG) ? flagCEffect->setOpacity(1) : flagCEffect->setOpacity(0.5);
    cpu->readFlag(V_FLAG) ? flagVEffect->setOpacity(1) : flagVEffect->setOpacity(0.5);
    cpu->readFlag(Q_BIT) ? flagQEffect->setOpacity(1) : flagQEffect->setOpacity(0.5);

    ui->modeVal->setText(Core::ProcessorModes::toString(cpu->getProcessorMode()));
    ui->stateVal->setText(cpu->getThumbMode() ? "Thumb" : "ARM");

    ui->IRQValue->setText(cpu->readFlag(I_BIT) ? "Disabled" : "Enabled");
    ui->FIQVal->setText(cpu->readFlag(F_BIT) ? "Disabled" : "Enabled");

    // Detailed view.
    if (!detailedView) return;

    ui->r0UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 0)).c_str());
    ui->r1UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 1)).c_str());
    ui->r2UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 2)).c_str());
    ui->r3UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 3)).c_str());
    ui->r4UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 4)).c_str());
    ui->r5UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 5)).c_str());
    ui->r6UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 6)).c_str());
    ui->r7UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 7)).c_str());
    ui->r8UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 8)).c_str());
    ui->r9UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 9)).c_str());
    ui->r10UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 10)).c_str());
    ui->r11UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 11)).c_str());
    ui->r12UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 12)).c_str());
    ui->r13UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 13)).c_str());
    ui->r14UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 14)).c_str());
    ui->r15UserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 15)).c_str());
    ui->cpsrUserVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::User, 16)).c_str());

    ui->r8FIQVal->setText(Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 8)).c_str());
    ui->r9FIQVal->setText(Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 9)).c_str());
    ui->r10FIQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 10)).c_str());
    ui->r11FIQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 11)).c_str());
    ui->r12FIQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 12)).c_str());
    ui->r13FIQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 13)).c_str());
    ui->r14FIQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 14)).c_str());
    ui->spsrFIQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::FIQ, 17)).c_str());

    ui->r13SupVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Supervisor, 13)).c_str());
    ui->r14SupVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Supervisor, 14)).c_str());
    ui->spsrSupVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Supervisor, 17)).c_str());

    ui->r13IRQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::IRQ, 13)).c_str());
    ui->r14IRQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::IRQ, 14)).c_str());
    ui->spsrIRQVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::IRQ, 17)).c_str());

    ui->r13AbortVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Abort, 13)).c_str());
    ui->r14AbortVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Abort, 14)).c_str());
    ui->spsrAbortVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Abort, 17)).c_str());

    ui->r13UndVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Undefined, 13)).c_str());
    ui->r14UndVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Undefined, 14)).c_str());
    ui->spsrUndVal->setText(
        Core::hexString(cpu->readModeReg(Core::ProcessorModes::Undefined, 17)).c_str());
}
// ==================================================================================================
void CPUViewer::toggleDetailedView() {
    detailedView = !detailedView;
    if (detailedView) {
        ui->toggleDetailedDisplayButton->setText("-");
        for (QWidget* elem : findChildren<QWidget*>()) {
            if (elem->property("detailedViewElement").toBool()) {
                elem->show();
            }
        }
    } else {
        ui->toggleDetailedDisplayButton->setText("+");
        for (QWidget* elem : findChildren<QWidget*>()) {
            if (elem->property("detailedViewElement").toBool()) {
                elem->hide();
            }
        }
    }
    this->adjustSize();
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
