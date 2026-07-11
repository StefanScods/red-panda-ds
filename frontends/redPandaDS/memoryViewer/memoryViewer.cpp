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
MemoryViewerContainer::MemoryViewerContainer(RedPandaDSApp* app, QWidget* parent)
    : QWidget(parent), app(app) {
    topHorizontalLayout = new QHBoxLayout(this);
    topHorizontalLayout->setContentsMargins(0, 0, 0, 0);
    topHorizontalLayout->setSpacing(0);

    viewport = new QWidget(this);
    contentWidget = new QWidget(viewport);
    contentLayout = new QGridLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    config.formatType = MemoryFormat::hex;
    config.procType = MemoryMap::arm9;

    for (unsigned int i = 0; i <= 0xF; i++) {
        QLabel* byteLabel = new QLabel(contentWidget);
        byteLabel->setProperty("boldLabel", true);
        byteLabel->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                           Qt::TextSelectableByKeyboard);
        std::stringstream ss;
        ss << PrintHex(i);
        byteLabel->setText(ss.str().c_str());
        contentLayout->addWidget(byteLabel, 0, 2 + i);
    }

    addressLabels.resize(MAX_NUM_ENTRY_LINES);
    for (unsigned int i = 0; i < MAX_NUM_ENTRY_LINES; i++) {
        addressLabels[i] = new QLabel(contentWidget);
        addressLabels[i]->setProperty("boldLabel", true);
        addressLabels[i]->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                                  Qt::TextSelectableByKeyboard);
        addressLabels[i]->setText("0x00000000");
        contentLayout->addWidget(addressLabels[i], 2 + i, 0);
    }

    const uint32_t numElements = MAX_NUM_ENTRY_LINES * 16;
    valueLabels.resize(numElements);
    for (unsigned int i = 0; i < numElements; i++) {
        valueLabels[i] = new QLabel(contentWidget);
        valueLabels[i]->setText("0x00");
        valueLabels[i]->setTextInteractionFlags(Qt::TextSelectableByMouse |
                                                Qt::TextSelectableByKeyboard);
        valueLabels[i]->adjustSize();
        valueLabels[i]->setFixedSize(valueLabels[i]->size());
        valueLabels[i]->setText("");
        valueLabels[i]->setAlignment(Qt::AlignLeft);
        unsigned int x = i % 16;
        unsigned int y = i / 16;
        contentLayout->addWidget(valueLabels[i], 2 + y, 2 + x);
    }

    QFrame* vLine = new QFrame(contentWidget);
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFrameShadow(QFrame::Sunken);
    vLine->setFixedWidth(2);
    contentLayout->addWidget(vLine, 0, 1, -1, 1);

    QFrame* hLine = new QFrame(contentWidget);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    hLine->setFixedHeight(2);
    contentLayout->addWidget(hLine, 1, 0, 1, -1);

    scrollBar = new QScrollBar(Qt::Vertical, this);
    scrollBar->setRange(0, INT_MAX);

    topHorizontalLayout->addWidget(viewport);
    topHorizontalLayout->addWidget(scrollBar);

    // Hook up callback functions.
    connect(scrollBar, &QScrollBar::sliderMoved, this, &MemoryViewerContainer::onScroll);
    connect(scrollBar, &QScrollBar::actionTriggered, this, &MemoryViewerContainer::onScrollAction);
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &MemoryViewerContainer::update);
}
// ==================================================================================================
MemoryViewerContainer::~MemoryViewerContainer() {
}
// ==================================================================================================
void MemoryViewerContainer::update() {
    if (isHidden()) return;

    Core::DSEmuCore* core = app->getEmuCore();
    Core::Interconnect* interconnect = core->getInterconnect();

    uint32_t topAddress = currentPosition & 0xFFFFFFF0;

    unsigned int numLinesInView = height() / ENTRY_LINE_PIXEL_HEIGHT;

    for (unsigned int lineNum = 0; lineNum < numLinesInView; lineNum++) {
        uint32_t lineAddress = topAddress + (lineNum * 0x10);
        std::stringstream lineAddressSS;
        lineAddressSS << PrintHexPadded(lineAddress, 8);
        addressLabels[lineNum]->setText(lineAddressSS.str().c_str());

        for (unsigned int entryNum = 0; entryNum <= 0xF; entryNum++) {
            uint32_t address = lineAddress + entryNum;
            uint32_t labelIndex = lineNum * 0x10 + entryNum;

            // Check if the address is valid.
            bool valid = config.procType == MemoryMap::arm7
                             ? interconnect->isAddressValidARM7(address)
                             : interconnect->isAddressValidARM9(address);
            if (!valid) {
                // If not, show an empty entry.
                valueLabels[labelIndex]->setText("");
                continue;
            }
            // Entry is valid, get the value.
            uint8_t value = config.procType == MemoryMap::arm7 ? interconnect->read8ARM7(address)
                                                               : interconnect->read8ARM9(address);
            // Format the value and render it.
            std::stringstream entryValueSS;
            switch (config.formatType) {
                case MemoryFormat::hex:
                    entryValueSS << PrintHexPadded(value, 2);
                    break;
                case MemoryFormat::decimal:
                    entryValueSS << static_cast<uint32_t>(value);
                    break;
                case MemoryFormat::ascii:
                    if (value == 0) {
                        // Special case if zero.
                        entryValueSS << "·";
                        break;
                    }
                    entryValueSS << value;
                    break;
                default:
                    break;
            }
            valueLabels[labelIndex]->setText(entryValueSS.str().c_str());
        }
    }
}
// ==================================================================================================
unsigned int MemoryViewerContainer::getMinWidth() {
    return contentWidget->sizeHint().width() + scrollBar->sizeHint().width() + 30;
}
// ==================================================================================================
void MemoryViewerContainer::setPosition(uint32_t pos) {
    currentPosition = pos;

    // Set the scroll position.
    uint32_t scrollPos = pos / 2;
    scrollBar->setValue(std::min(scrollPos, (uint32_t)INT_MAX));
}
// ==================================================================================================
void MemoryViewerContainer::onScroll(int value) {
    uint32_t scrollPos = ((uint32_t)value) * 2;
    setPosition(scrollPos);
}
// ==================================================================================================
void MemoryViewerContainer::onScrollAction(int value) {
    int scrollAmount = 0x10;
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
void MemoryViewerContainer::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y();
    int scrollAmount = 0x10;
    if (delta > 0) {
        setPosition(currentPosition - scrollAmount);
    } else {
        setPosition(currentPosition + scrollAmount);
    }
}
// ==================================================================================================
MemoryViewer::MemoryViewer(RedPandaDSApp* app, Core::DSEmuCore* core, QWidget* parent)
    : QWidget(parent), app(app), ui(new Ui::MemoryViewer), core(core) {
    ui->setupUi(this);

    viewer = new MemoryViewerContainer(app, this);
    ui->verticalLayout->insertWidget(ui->verticalLayout->indexOf(ui->line), viewer);

    setFixedWidth(viewer->getMinWidth());

    setNewMemoryViewerConfiguration();

    // Hook up callback functions.
    connect(app->getRefreshTimer(), &QTimer::timeout, this, &MemoryViewer::update);
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->addressInput, &QLineEdit::textChanged, this,
            &MemoryViewer::onJumpToAddressTextBoxChanged);
    // Radio buttons.
    connect(ui->radioButton_cpu_arm7, &QRadioButton::clicked, this, [this]() {
        viewer->config.procType = MemoryMap::arm7;
        setNewMemoryViewerConfiguration();
    });
    connect(ui->radioButton_cpu_arm9, &QRadioButton::clicked, this, [this]() {
        viewer->config.procType = MemoryMap::arm9;
        setNewMemoryViewerConfiguration();
    });
    connect(ui->radioButton_mode_hex, &QRadioButton::clicked, this, [this]() {
        viewer->config.formatType = MemoryFormat::hex;
        setNewMemoryViewerConfiguration();
    });
    connect(ui->radioButton_mode_decimal, &QRadioButton::clicked, this, [this]() {
        viewer->config.formatType = MemoryFormat::decimal;
        setNewMemoryViewerConfiguration();
    });
    connect(ui->radioButton_mode_ascii, &QRadioButton::clicked, this, [this]() {
        viewer->config.formatType = MemoryFormat::ascii;
        setNewMemoryViewerConfiguration();
    });
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
void MemoryViewer::setNewMemoryViewerConfiguration() {
    // Clear UI values.
    ui->radioButton_cpu_arm7->setChecked(false);
    ui->radioButton_cpu_arm9->setChecked(false);
    ui->radioButton_mode_hex->setChecked(false);
    ui->radioButton_mode_decimal->setChecked(false);
    ui->radioButton_mode_ascii->setChecked(false);
    // Set UI values.
    switch (viewer->config.formatType) {
        case MemoryFormat::hex:
            ui->radioButton_mode_hex->setChecked(true);
            break;
        case MemoryFormat::decimal:
            ui->radioButton_mode_decimal->setChecked(true);
            break;
        case MemoryFormat::ascii:
            ui->radioButton_mode_ascii->setChecked(true);
            break;
        default:
            break;
    }
    switch (viewer->config.procType) {
        case MemoryMap::arm7:
            ui->radioButton_cpu_arm7->setChecked(true);
            break;
        case MemoryMap::arm9:
            ui->radioButton_cpu_arm9->setChecked(true);
            break;
        default:
            break;
    }
}
// ==================================================================================================
void MemoryViewer::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}
// ==================================================================================================
void MemoryViewer::onJumpToAddressTextBoxChanged() {
    QString text = ui->addressInput->text().trimmed().toLower();
    if (!text.startsWith("0x")) {
        text.prepend("0x");
    }
    bool ok;
    uint32_t address = text.toUInt(&ok, 16);
    if (!ok) return;
    viewer->setPosition(address);
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
