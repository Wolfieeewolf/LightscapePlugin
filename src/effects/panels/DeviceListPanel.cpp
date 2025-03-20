/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| DeviceListPanel.cpp                                       |
|                                                           |
| Panel for device selection                                |
\*---------------------------------------------------------*/

#include "effects/panels/DeviceListPanel.h"
#include "ui_DeviceListPanel.h"

namespace Lightscape {

DeviceListPanel::DeviceListPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DeviceListPanel)
    , _deviceListWidget(nullptr)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

DeviceListPanel::~DeviceListPanel()
{
    delete ui;
}

void DeviceListPanel::setupUI()
{
    // Create the device list widget
    _deviceListWidget = new DeviceListWidget(this);
    if (ui->deviceListContainerLayout) {
        ui->deviceListContainerLayout->addWidget(_deviceListWidget);
    }
}

void DeviceListPanel::setupConnections()
{
    if (_deviceListWidget) {
        connect(_deviceListWidget, &DeviceListWidget::selectionChanged,
                this, &DeviceListPanel::selectionChanged);
    }
    
    // Connect Select All button
    if (ui->selectAllButton) {
        connect(ui->selectAllButton, &QPushButton::clicked, this, [this]() {
            if (_deviceListWidget) {
                _deviceListWidget->selectAll();
            }
            emit selectAllClicked();
        });
    }
    
    // Connect Clear All button
    if (ui->clearAllButton) {
        connect(ui->clearAllButton, &QPushButton::clicked, this, [this]() {
            if (_deviceListWidget) {
                _deviceListWidget->clearSelection();
            }
            emit clearAllClicked();
        });
    }
}

void DeviceListPanel::init(::DeviceManager* deviceManager, ::SpatialGrid* grid)
{
    if (_deviceListWidget) {
        _deviceListWidget->init(deviceManager, grid);
    }
}

void DeviceListPanel::refresh()
{
    if (_deviceListWidget) {
        _deviceListWidget->refresh();
    }
}

QList<DeviceInfo> DeviceListPanel::getSelectedDevices() const
{
    if (_deviceListWidget) {
        return _deviceListWidget->getSelectedDevices();
    }
    return QList<DeviceInfo>();
}

void DeviceListPanel::setSelectedDevices(const QList<DeviceInfo>& devices)
{
    if (_deviceListWidget) {
        _deviceListWidget->setSelectedDevices(devices);
    }
}

DeviceListWidget* DeviceListPanel::getDeviceListWidget() const
{
    return _deviceListWidget;
}

void DeviceListPanel::selectAllDevices()
{
    if (_deviceListWidget) {
        printf("[Lightscape][DeviceListPanel] Selecting all devices\n");
        _deviceListWidget->selectAll();
    }
}

} // namespace Lightscape