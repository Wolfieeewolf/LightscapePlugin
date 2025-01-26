#include "devices/NonRGBDeviceWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QResizeEvent>

NonRGBDeviceWidget::NonRGBDeviceWidget(NonRGBDevice* device, NonRGBGridManager* gridManager, QWidget* parent)
    : QFrame(parent)
    , device(device)
    , gridManager(gridManager)
{
    setupUI();
    setupConnections();
    loadDeviceData();

    // Set size policy
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

QSize NonRGBDeviceWidget::sizeHint() const
{
    return calculateNeededSize();
}

QSize NonRGBDeviceWidget::minimumSizeHint() const
{
    return calculateNeededSize();
}

QSize NonRGBDeviceWidget::calculateNeededSize() const
{
    // Calculate based on content
    int minWidth = 250;  // Base minimum width
    
    // Get width from the form layout
    if (layout()) {
        minWidth = std::max(minWidth, layout()->minimumSize().width());
    }
    
    // Calculate height based on all widgets plus spacing
    int totalHeight = 0;
    if (layout()) {
        totalHeight = layout()->minimumSize().height();
    }
    
    // Add some padding
    totalHeight += 2 * frameWidth();
    minWidth += 2 * frameWidth();
    
    return QSize(minWidth, totalHeight);
}

void NonRGBDeviceWidget::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);
    
    // If new size is smaller than needed, resize back to minimum
    QSize needed = calculateNeededSize();
    if (event->size().width() < needed.width() || 
        event->size().height() < needed.height()) {
        resize(
            std::max(event->size().width(), needed.width()),
            std::max(event->size().height(), needed.height())
        );
    }
}

void NonRGBDeviceWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);  // Important for size handling
    
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSizeConstraint(QLayout::SetMinimumSize);  // Important for size handling

    // Name input
    nameEdit = new QLineEdit(this);
    formLayout->addRow("Name:", nameEdit);

    // Type selection
    typeCombo = new QComboBox(this);
    typeCombo->addItem("Monitor", static_cast<int>(NonRGBDeviceType::Monitor));
    typeCombo->addItem("PC Case", static_cast<int>(NonRGBDeviceType::PCCase));
    typeCombo->addItem("Speaker", static_cast<int>(NonRGBDeviceType::Speaker));
    typeCombo->addItem("Desk", static_cast<int>(NonRGBDeviceType::Desk));
    typeCombo->addItem("Custom", static_cast<int>(NonRGBDeviceType::Custom));
    formLayout->addRow("Type:", typeCombo);

    // Dimensions
    QHBoxLayout* dimLayout = new QHBoxLayout();
    widthSpin = new QSpinBox(this);
    heightSpin = new QSpinBox(this);
    depthSpin = new QSpinBox(this);
    
    widthSpin->setRange(1, 10);
    heightSpin->setRange(1, 10);
    depthSpin->setRange(1, 10);
    
    dimLayout->addWidget(new QLabel("W:", this));
    dimLayout->addWidget(widthSpin);
    dimLayout->addWidget(new QLabel("H:", this));
    dimLayout->addWidget(heightSpin);
    dimLayout->addWidget(new QLabel("D:", this));
    dimLayout->addWidget(depthSpin);
    formLayout->addRow("Dimensions:", dimLayout);

    // Position
    QHBoxLayout* posLayout = new QHBoxLayout();
    posXSpin = new QSpinBox(this);
    posYSpin = new QSpinBox(this);
    posZSpin = new QSpinBox(this);
    
    posXSpin->setRange(0, 9);
    posYSpin->setRange(0, 9);
    posZSpin->setRange(0, 9);
    
    posLayout->addWidget(new QLabel("X:", this));
    posLayout->addWidget(posXSpin);
    posLayout->addWidget(new QLabel("Y:", this));
    posLayout->addWidget(posYSpin);
    posLayout->addWidget(new QLabel("Z:", this));
    posLayout->addWidget(posZSpin);
    formLayout->addRow("Position:", posLayout);

    // Remove button
    QPushButton* removeButton = new QPushButton("Remove Device", this);
    
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(removeButton);

    // Add frame around the widget
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(1);

    connect(removeButton, &QPushButton::clicked, this, &NonRGBDeviceWidget::onRemoveClicked);
}

void NonRGBDeviceWidget::setupConnections()
{
    connect(nameEdit, &QLineEdit::textChanged,
            this, &NonRGBDeviceWidget::onNameChanged);
            
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NonRGBDeviceWidget::onTypeChanged);
            
    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NonRGBDeviceWidget::onDimensionsChanged);
    connect(heightSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NonRGBDeviceWidget::onDimensionsChanged);
    connect(depthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NonRGBDeviceWidget::onDimensionsChanged);
            
    connect(posXSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NonRGBDeviceWidget::onPositionChanged);
    connect(posYSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NonRGBDeviceWidget::onPositionChanged);
    connect(posZSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NonRGBDeviceWidget::onPositionChanged);
}

void NonRGBDeviceWidget::loadDeviceData()
{
    if (!device) return;

    nameEdit->setText(device->getName());
    typeCombo->setCurrentIndex(static_cast<int>(device->getType()));
    
    QSize dims = device->getDimensions();
    widthSpin->setValue(dims.width());
    heightSpin->setValue(dims.height());
    depthSpin->setValue(device->getDepth());
    
    GridPosition pos = device->getPosition();
    posXSpin->setValue(pos.x);
    posYSpin->setValue(pos.y);
    posZSpin->setValue(pos.z);
}

bool NonRGBDeviceWidget::validateInput()
{
    if (nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Device name cannot be empty");
        return false;
    }

    GridPosition newPos(posXSpin->value(), posYSpin->value(), posZSpin->value());
    QSize newDims(widthSpin->value(), heightSpin->value());
    
    // Temporarily update device with new values for validation
    device->setPosition(newPos);
    device->setDimensions(newDims);
    device->setDepth(depthSpin->value());
    
    if (!gridManager->validateDevicePosition(device)) {
        QMessageBox::warning(this, "Validation Error", "Device position is invalid or space is already occupied");
        return false;
    }

    return true;
}

void NonRGBDeviceWidget::onNameChanged(const QString& name)
{
    if (!device) return;
    device->setName(name);
    emit deviceModified(device);
}

void NonRGBDeviceWidget::onTypeChanged(int index)
{
    if (!device) return;
    device->setType(static_cast<NonRGBDeviceType>(index));
    emit deviceModified(device);
}

void NonRGBDeviceWidget::onDimensionsChanged()
{
    if (!device || !validateInput()) return;
    
    device->setDimensions(QSize(widthSpin->value(), heightSpin->value()));
    device->setDepth(depthSpin->value());
    emit deviceModified(device);
}

void NonRGBDeviceWidget::onPositionChanged()
{
    if (!device || !validateInput()) return;
    
    device->setPosition(GridPosition(
        posXSpin->value(),
        posYSpin->value(),
        posZSpin->value()
    ));
    emit deviceModified(device);
}

void NonRGBDeviceWidget::onRemoveClicked()
{
    if (!device) return;
    
    if (QMessageBox::question(this, "Remove Device",
                            "Are you sure you want to remove this device?",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        emit removeRequested(device->getName());
    }
}