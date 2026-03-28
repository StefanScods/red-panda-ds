#include "disassemblyViewer.h"

#include "app/app.h"
#include "common.h"
#include "core/armInterpreter/instructionDisassembler.h"
#include "core/utils/utils.h"
#include "disassemblyViewerContainer.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace UI {

// ==================================================================================================
DisassemblyViewerEntry::DisassemblyViewerEntry(RedPandaDSApp* app, QGridLayout* parentLayout,
                                               uint32_t lineId, QWidget* parent)
    : QWidget(parent), app(app) {
    QGraphicsOpacityEffect* addressLabelEffect = new QGraphicsOpacityEffect(this);
    addressLabelEffect->setOpacity(0.75);
    QGraphicsOpacityEffect* instructionBitsEffect = new QGraphicsOpacityEffect(this);
    instructionBitsEffect->setOpacity(0.75);

    addressLabel = new QLabel(parent);
    addressLabel->setProperty("disassemblyViewerEntryLabel", "true");
    addressLabel->setGraphicsEffect(addressLabelEffect);
    parentLayout->addWidget(addressLabel, lineId, 0);
    instructionBitsLabel = new QLabel(parent);
    instructionBitsLabel->setProperty("disassemblyViewerEntryLabel", "true");
    instructionBitsLabel->setGraphicsEffect(instructionBitsEffect);
    parentLayout->addWidget(instructionBitsLabel, lineId, 2);
    opcodeLabel = new QLabel(parent);
    opcodeLabel->setProperty("disassemblyViewerEntryLabel", "true");
    parentLayout->addWidget(opcodeLabel, lineId, 3);
    destinationLabel = new QLabel(parent);
    destinationLabel->setProperty("disassemblyViewerEntryLabel", "true");
    parentLayout->addWidget(destinationLabel, lineId, 4);
    operand1Label = new QLabel(parent);
    operand1Label->setProperty("disassemblyViewerEntryLabel", "true");
    parentLayout->addWidget(operand1Label, lineId, 5);
    operand2Label = new QLabel(parent);
    operand2Label->setProperty("disassemblyViewerEntryLabel", "true");
    parentLayout->addWidget(operand2Label, lineId, 6);
    commentLabel = new QLabel(parent);
    commentLabel->setProperty("disassemblyViewerEntryLabel", "true");
    parentLayout->addWidget(commentLabel, lineId, 7);
}
// ==================================================================================================
DisassemblyViewerEntry::~DisassemblyViewerEntry() {
}
// ==================================================================================================
void DisassemblyViewerEntry::setTargetInstruction(uint32_t newAddress, bool validInstruction,
                                                  uint32_t newInstruction,
                                                  uint32_t newInstructionSize, bool armMode) {
    targetAddress = newAddress;
    isValidInstruction = validInstruction;
    targetInstruction = newInstruction;
    targetInstructionSize = newInstructionSize;
    isARMModeInstruction = armMode;
}
// ==================================================================================================
void DisassemblyViewerEntry::update() {
    // Cache previous UI state.
    if (prevTargetAddress == targetAddress && prevTargetInstruction == targetInstruction &&
        prevIsValidInstruction == isValidInstruction &&
        prevTargetInstructionSize == targetInstructionSize &&
        prevIsARMModeInstruction == isARMModeInstruction) {
        // Label hasn't changed
        return;
    }
    prevTargetAddress = targetAddress;
    prevTargetInstruction = targetInstruction;
    prevIsValidInstruction = isValidInstruction;
    prevTargetInstructionSize = targetInstructionSize;
    prevIsARMModeInstruction = isARMModeInstruction;

    // Render new label.
    std::stringstream ss;
    std::string temp;

    // Address.
    ss << "0x" << std::hex << std::setfill('0') << std::setw(8) << targetAddress;
    addressLabel->setText(QString::fromStdString(ss.str()));

    // Clear the rest of the labels.
    instructionBitsLabel->setText("");
    opcodeLabel->setText("");
    destinationLabel->setText("");
    operand1Label->setText("");
    operand2Label->setText("");
    commentLabel->setText("");
    if (!isValidInstruction) {
        return;
    }

    // Disassemble the target instruction based on THUMB or ARM mode.
    Core::InstructionDisassembly disassembly =
        isARMModeInstruction ? Core::dissembleARMInstruction(targetInstruction, false)
                             : Core::dissembleTHUMBInstruction(targetInstruction, false);

    // Instruction Data.
    ss.str("");
    ss.clear();
    if (isARMModeInstruction) {
        ss << "0x" << std::hex << std::setfill('0') << std::setw(8) << targetInstruction;
    } else {
        if (targetInstructionSize == 4) {
            ss << "0x" << std::hex << std::setfill('0') << std::setw(4)
               << Core::readBits(targetInstruction, 16, 31);
            ss << " ";
            ss << "0x" << std::hex << std::setfill('0') << std::setw(4)
               << Core::readBits(targetInstruction, 0, 15);
        } else {
            ss << "0x" << std::hex << std::setfill('0') << std::setw(4) << targetInstruction;
        }
    }
    instructionBitsLabel->setText(QString::fromStdString(ss.str()));

    // Op code.
    temp = disassembly.opcode + disassembly.conditionCode;
    opcodeLabel->setText(QString::fromStdString(temp));

    // Destination
    if (!disassembly.destination.empty()) {
        temp = disassembly.destination;
        if (!disassembly.operand1.empty() || !disassembly.operand2.empty()) {
            temp += ",";
        }
        destinationLabel->setText(QString::fromStdString(temp));
    }

    // Operand 1.
    if (!disassembly.operand1.empty()) {
        temp = disassembly.operand1;
        if (!disassembly.operand2.empty()) {
            temp += ",";
        }
        operand1Label->setText(QString::fromStdString(temp));
    }

    // Operand 2.
    if (!disassembly.operand2.empty()) {
        temp = disassembly.operand2;
        operand2Label->setText(QString::fromStdString(temp));
    }

    // Comment.
    if (!disassembly.comment.empty()) {
        temp = "# " + disassembly.comment;
        commentLabel->setText(QString::fromStdString(temp));
    }
}
// ==================================================================================================
DisassemblyViewerContainer::DisassemblyViewerContainer(RedPandaDSApp* app, QWidget* parent)
    : QWidget(parent), app(app) {
    topHorizontalLayout = new QHBoxLayout(this);
    topHorizontalLayout->setContentsMargins(0, 0, 0, 0);
    topHorizontalLayout->setSpacing(0);

    viewport = new QWidget(this);
    contentWidget = new QWidget(viewport);
    contentLayout = new QGridLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    QFrame* vLine = new QFrame(this);
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFrameShadow(QFrame::Sunken);
    vLine->setFixedWidth(2);
    contentLayout->addWidget(vLine, 0, 1, -1, 1);

    contentLayout->setColumnStretch(0, 0);
    contentLayout->setColumnStretch(1, 0);
    contentLayout->setColumnStretch(2, 0);
    contentLayout->setColumnStretch(3, 1);
    contentLayout->setColumnStretch(4, 1);
    contentLayout->setColumnStretch(5, 1);
    contentLayout->setColumnStretch(6, 1);
    contentLayout->setColumnStretch(7, 5);

    scrollBar = new QScrollBar(Qt::Vertical, this);
    scrollBar->setRange(0, INT_MAX);

    topHorizontalLayout->addWidget(viewport);
    topHorizontalLayout->addWidget(scrollBar);

    for (int i = 0; i < MAX_NUM_ENTRY_LINES; ++i) {
        entries[i] = new DisassemblyViewerEntry(app, contentLayout, i, contentWidget);
        entries[i]->setFixedHeight(ENTRY_LINE_PIXEL_HEIGHT);
        contentLayout->addWidget(entries[i]);
    }
    setMaximumHeight(ENTRY_LINE_PIXEL_HEIGHT * MAX_NUM_ENTRY_LINES);

    // Hook up callback functions.
    connect(scrollBar, &QScrollBar::sliderMoved, this, &DisassemblyViewerContainer::onScroll);
    connect(scrollBar, &QScrollBar::actionTriggered, this,
            &DisassemblyViewerContainer::onScrollAction);
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &DisassemblyViewerContainer::update);
}
// ==================================================================================================
DisassemblyViewerContainer::~DisassemblyViewerContainer() {
}
// ==================================================================================================
void DisassemblyViewerContainer::update() {
    if (parentWidget()->isHidden()) return;
    contentWidget->hide();

    Core::Interconnect* interconnect = app->getEmuCore()->getInterconnect();

    int numLinesInView = height() / ENTRY_LINE_PIXEL_HEIGHT + 1;

    // From the config, determine whether we are tracing the arm7 or arm9 in ARM / THUMB mode.
    bool arm7 = config.procType == Processor::arm7;
    bool armMode = config.formatType == Format::arm;
    if (config.formatType == Format::autoDetect) {
        armMode = arm7 ? !app->getEmuCore()->getARM7Core()->getThumbMode()
                       : !app->getEmuCore()->getARM9Core()->getThumbMode();
    }

    uint32_t addrMask = armMode ? (~0b11) : (~0b1);

    uint32_t addr = currentPosition & addrMask;
    bool supportedAddress = 0;
    uint32_t addrSize = 0;
    uint32_t instruction = 0;
    // Loop over the lines in view and assign address / instructions to render on each line.
    for (int i = 0; i < numLinesInView; ++i) {
        // For ARM mode, instructions are always 4 bytes.
        if (armMode) {
            addrSize = 4;
            supportedAddress = arm7 ? interconnect->isAddressValidARM7(addr)
                                    : interconnect->isAddressValidARM9(addr);
            if (supportedAddress) {
                instruction =
                    arm7 ? interconnect->read32ARM7(addr) : interconnect->read32ARM9(addr);
            }
        }
        // For Thumb mode, instructions could be 2 or 4 bytes.
        else {
            bool valid = false;
            if (i == 0) {
                // Only for first line: check if previous halfword forms wide instruction.
                uint32_t testAddr = addr - 2;
                supportedAddress = arm7 ? interconnect->isAddressValidARM7(testAddr)
                                        : interconnect->isAddressValidARM9(testAddr);
                if (supportedAddress) {
                    instruction = arm7 ? interconnect->read16ARM7(testAddr)
                                       : interconnect->read16ARM9(testAddr);
                    bool wideEncoding = (Core::readBits(instruction, 13, 15) == 0b111) &&
                                        (Core::readBits(instruction, 11, 12) != 0b00);
                    if (wideEncoding) {
                        valid = true;
                        // Start the disassembly from this line.
                        addr = testAddr;
                        addrSize = 4;
                        instruction = instruction << 16;
                        instruction |= (arm7 ? interconnect->read16ARM7(testAddr + 2)
                                             : interconnect->read16ARM9(testAddr + 2));
                    }
                }
            }
            if (!valid) {
                addrSize = 2;
                supportedAddress = arm7 ? interconnect->isAddressValidARM7(addr)
                                        : interconnect->isAddressValidARM9(addr);
                if (supportedAddress) {
                    instruction =
                        arm7 ? interconnect->read16ARM7(addr) : interconnect->read16ARM9(addr);
                    bool wideEncoding = (Core::readBits(instruction, 13, 15) == 0b111) &&
                                        (Core::readBits(instruction, 11, 12) != 0b00);
                    if (wideEncoding) {
                        addrSize = 4;
                        instruction = instruction << 16;
                        instruction |= (arm7 ? interconnect->read16ARM7(addr + 2)
                                             : interconnect->read16ARM9(addr + 2));
                    }
                }
            }
        }

        // Set the target instruction and render it.
        entries[i]->setTargetInstruction(addr, supportedAddress, instruction, addrSize, armMode);
        entries[i]->update();
        addr += addrSize;
    }
    contentWidget->show();
}
// ==================================================================================================
void DisassemblyViewerContainer::setWidth(int width) {
    contentWidget->setMinimumWidth(width);
}
// ==================================================================================================
void DisassemblyViewerContainer::setPosition(uint32_t pos) {
    currentPosition = pos;

    // Set the scroll position.
    uint32_t scrollPos = pos / 2;
    scrollBar->setValue(std::min(scrollPos, (uint32_t)INT_MAX));
}
// ==================================================================================================
void DisassemblyViewerContainer::onScroll(int value) {
    uint32_t scrollPos = ((uint32_t)value) * 2;
    setPosition(scrollPos);
}
// ==================================================================================================
void DisassemblyViewerContainer::onScrollAction(int value) {
    bool arm7 = config.procType == Processor::arm7;
    bool armMode = config.formatType == Format::arm;
    if (config.formatType == Format::autoDetect) {
        armMode = arm7 ? !app->getEmuCore()->getARM7Core()->getThumbMode()
                       : !app->getEmuCore()->getARM9Core()->getThumbMode();
    }
    int scrollAmount = armMode ? 4 : 2;

    switch (value) {
        case QScrollBar::SliderAction::SliderPageStepAdd:
        case QScrollBar::SliderAction::SliderSingleStepAdd:
            setPosition(currentPosition + scrollAmount);
            break;
        case QScrollBar::SliderAction::SliderSingleStepSub:
        case QScrollBar::SliderAction::SliderPageStepSub:
            setPosition(currentPosition - scrollAmount);
            break;
        case QScrollBar::SliderAction::SliderToMaximum:
            setPosition(UINT32_MAX);
            break;
        case QScrollBar::SliderAction::SliderToMinimum:
            setPosition(0);
            break;
        default:
            break;
    }
}
// ==================================================================================================
void DisassemblyViewerContainer::wheelEvent(QWheelEvent* event) {
    bool arm7 = config.procType == Processor::arm7;
    bool armMode = config.formatType == Format::arm;
    if (config.formatType == Format::autoDetect) {
        armMode = arm7 ? !app->getEmuCore()->getARM7Core()->getThumbMode()
                       : !app->getEmuCore()->getARM9Core()->getThumbMode();
    }
    int scrollAmount = armMode ? 4 : 2;

    int delta = event->angleDelta().y();
    if (delta > 0) {
        setPosition(currentPosition - scrollAmount);
    } else {
        setPosition(currentPosition + scrollAmount);
    }
}
// ==================================================================================================
DisassemblyViewer::DisassemblyViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent)
    : QWidget(parent), app(app), ui(new Ui::DisassemblyViewer), core(core) {
    ui->setupUi(this);

    viewer = new DisassemblyViewerContainer(app, this);
    ui->verticalLayout->insertWidget(ui->verticalLayout->indexOf(ui->line), viewer);

    // Set default config.
    currentConfig.formatType = Format::autoDetect;
    currentConfig.procType = Processor::arm9;
    setNewDisassemblyConfiguration();

    // Hook up callback functions.
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
    // Radio buttons.
    connect(ui->radioButton_cpu_arm7, &QRadioButton::clicked, this, [this]() {
        currentConfig.procType = Processor::arm7;
        setNewDisassemblyConfiguration();
    });
    connect(ui->radioButton_cpu_arm9, &QRadioButton::clicked, this, [this]() {
        currentConfig.procType = Processor::arm9;
        setNewDisassemblyConfiguration();
    });
    connect(ui->radioButton_mode_auto, &QRadioButton::clicked, this, [this]() {
        currentConfig.formatType = Format::autoDetect;
        setNewDisassemblyConfiguration();
    });
    connect(ui->radioButton_mode_arm, &QRadioButton::clicked, this, [this]() {
        currentConfig.formatType = Format::arm;
        setNewDisassemblyConfiguration();
    });
    connect(ui->radioButton_mode_thumb, &QRadioButton::clicked, this, [this]() {
        currentConfig.formatType = Format::thumb;
        setNewDisassemblyConfiguration();
    });
}
// ==================================================================================================
DisassemblyViewer::~DisassemblyViewer() {
    DELETE_DYNAMIC_POINTER(ui);
}
// ==================================================================================================
void DisassemblyViewer::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    viewer->setWidth(width());
}
// ==================================================================================================
void DisassemblyViewer::setNewDisassemblyConfiguration() {
    // Tell the viewer the new configuration.
    viewer->setDisassemblyConfiguration(currentConfig);
    // Clear UI values.
    ui->radioButton_cpu_arm7->setChecked(false);
    ui->radioButton_cpu_arm9->setChecked(false);
    ui->radioButton_mode_auto->setChecked(false);
    ui->radioButton_mode_arm->setChecked(false);
    ui->radioButton_mode_thumb->setChecked(false);
    // Set UI values.
    switch (currentConfig.formatType) {
        case Format::autoDetect:
            ui->radioButton_mode_auto->setChecked(true);
            break;
        case Format::arm:
            ui->radioButton_mode_arm->setChecked(true);
            break;
        case Format::thumb:
            ui->radioButton_mode_thumb->setChecked(true);
            break;
        default:
            break;
    }
    switch (currentConfig.procType) {
        case Processor::arm7:
            ui->radioButton_cpu_arm7->setChecked(true);
            break;
        case Processor::arm9:
            ui->radioButton_cpu_arm9->setChecked(true);
            break;
        default:
            break;
    }
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
