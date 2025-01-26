#include "devices/NonRGBDevicesPanel.h"
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>

NonRGBDevicesPanel::NonRGBDevicesPanel(NonRGBDeviceManager* deviceManager, 
                                     NonRGBGridManager* gridManager,
                                     QWidget* parent)
    : QWidget(parent)
    , deviceManager(deviceManager)
    , gridManager(gridManager)
{
    setupUI();
    updateDevicesList();
}

void NonRGBDevicesPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add Device button at top
    QPushButton* addButton = new QPushButton("Add Device", this);
    connect(addButton, &QPushButton::clicked, this, &NonRGBDevicesPanel::onAddDeviceClicked);
    mainLayout->addWidget(addButton);

    // Scroll area for devices
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Scroll content
    scrollContent = new QWidget(scrollArea);
    devicesLayout = new QVBoxLayout(scrollContent);
    devicesLayout->setAlignment(Qt::AlignTop);
    scrollContent->setLayout(devicesLayout);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // Style and size
    setMinimumWidth(300);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

void NonRGBDevicesPanel::onAddDeviceClicked()
{
    QString name = generateUniqueName("NewDevice");
    NonRGBDevice* device = new NonRGBDevice(name);
    
    if (deviceManager->addDevice(device)) {
        gridManager->assignDeviceToGrid(device);
        addDeviceWidget(device);
    }
}

QString NonRGBDevicesPanel::generateUniqueName(const QString& baseName) const
{
    QString name = baseName;
    int counter = 1;
    
    while (deviceManager->hasDevice(name)) {
        name = QString("%1_%2").arg(baseName).arg(counter++);
    }
    
    return name;
}

void NonRGBDevicesPanel::onDeviceModified(NonRGBDevice* device)
{
    if (!device) return;
    
    // Update the group box title if name changed
    QWidget* widget = qobject_cast<NonRGBDeviceWidget*>(sender())->parentWidget();
    if (QGroupBox* groupBox = qobject_cast<QGroupBox*>(widget)) {
        groupBox->setTitle(device->getName());
    }
    
    gridManager->updateDeviceInGrid(device);
}

void NonRGBDevicesPanel::onDeviceRemoveRequested(const QString& deviceName)
{
    gridManager->removeDeviceFromGrid(deviceName);
    deviceManager->removeDevice(deviceName);
    updateDevicesList();
}

void NonRGBDevicesPanel::updateDevicesList()
{
    // Clear existing widgets
    QLayoutItem* item;
    while ((item = devicesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add widgets for all devices
    for (NonRGBDevice* device : deviceManager->getAllDevices()) {
        addDeviceWidget(device);
    }
}

void NonRGBDevicesPanel::addDeviceWidget(NonRGBDevice* device)
{
    if (!device) return;

    // Create group box for the device
    QGroupBox* deviceGroup = new QGroupBox(device->getName(), scrollContent);
    QVBoxLayout* groupLayout = new QVBoxLayout(deviceGroup);

    // Create and add device widget
    NonRGBDeviceWidget* widget = new NonRGBDeviceWidget(device, gridManager, deviceGroup);
    groupLayout->addWidget(widget);

    // Connect signals
    connect(widget, &NonRGBDeviceWidget::deviceModified,
            this, &NonRGBDevicesPanel::onDeviceModified);
    connect(widget, &NonRGBDeviceWidget::removeRequested,
            this, &NonRGBDevicesPanel::onDeviceRemoveRequested);

    // Add to panel
    devicesLayout->addWidget(deviceGroup);
}

void NonRGBDevicesPanel::onDeviceListChanged()
{
    updateDevicesList();
}