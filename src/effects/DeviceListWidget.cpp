/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| DeviceListWidget.cpp                                      |
|                                                           |
| Widget for device selection                               |
\*---------------------------------------------------------*/

#include "effects/DeviceListWidget.h"
#include "ui_DeviceListWidget.h"
#include "core/HashFunctions.h"
#include <QMouseEvent>

namespace Lightscape {

DeviceListWidget::DeviceListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DeviceListWidget)
{
    ui->setupUi(this);
    
    // Connect signals for checkbox state changes
    connect(ui->deviceList, &QListWidget::itemChanged,
            this, &DeviceListWidget::onItemSelectionChanged);
            
    // Install event filter to handle mouse clicks directly
    ui->deviceList->installEventFilter(this);
    ui->deviceList->setFocusPolicy(Qt::NoFocus); // Prevent focus rectangle
}

DeviceListWidget::~DeviceListWidget()
{
    delete ui;
}

void DeviceListWidget::init(::DeviceManager* deviceManager, ::SpatialGrid* grid)
{
    _deviceManager = deviceManager;
    _spatialGrid = grid;
    
    // Refresh the device list
    refresh();
}

void DeviceListWidget::refresh()
{
    if (!_deviceManager || !_spatialGrid) return;
    
    // Remember selected items
    QList<DeviceInfo> selectedBefore = getSelectedDevices();
    
    // Clear the list
    ui->deviceList->clear();
    _itemToDevice.clear();
    
    // Get grid dimensions
    GridDimensions dims = _spatialGrid->GetDimensions();
    
    // Collect all devices assigned to grid positions
    QMap<QPair<int, DeviceType>, GridPosition> devicePositions;
    QSet<QPair<int, DeviceType>> uniqueDevices;
    
    // Scan all grid positions for assigned devices
    for (int z = 0; z < dims.depth; z++) {
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                
                // Get assignments for this position
                const auto& assignments = _spatialGrid->GetAssignments(pos);
                
                for (const auto& assignment : assignments) {
                    QPair<int, DeviceType> deviceKey(assignment.device_index, assignment.device_type);
                    devicePositions[deviceKey] = pos;
                    uniqueDevices.insert(deviceKey);
                }
            }
        }
    }
    
    // Add each unique device to the list
    for (const auto& deviceKey : uniqueDevices) {
        int deviceIndex = deviceKey.first;
        DeviceType deviceType = deviceKey.second;
        
        // Get device name
        QString name = _deviceManager->GetDeviceName(deviceIndex, deviceType);
        
        // Get device position
        GridPosition position = devicePositions[deviceKey];
        
        // Create list item
        QListWidgetItem* item = new QListWidgetItem(name);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        
        // Add position info to display
        item->setText(QString("%1 [%2,%3,%4]").arg(name)
                                             .arg(position.x)
                                             .arg(position.y)
                                             .arg(position.z));
        
        // Store device info
        DeviceListInfo info;
        info.index = deviceIndex;
        info.type = deviceType;
        info.position = position;
        info.hasPosition = true;
        
        _itemToDevice[item] = info;
        
        // Add to list
        ui->deviceList->addItem(item);
    }
    
    // Restore selection
    setSelectedDevices(selectedBefore);
}

QList<DeviceInfo> DeviceListWidget::getSelectedDevices() const
{
    QList<DeviceInfo> selected;
    
    // Go through all items and check which ones are checked
    for (int i = 0; i < ui->deviceList->count(); i++) {
        QListWidgetItem* item = ui->deviceList->item(i);
        
        if (item->checkState() == Qt::Checked && _itemToDevice.contains(item)) {
            const DeviceListInfo& info = _itemToDevice[item];
            
            DeviceInfo deviceInfo;
            deviceInfo.index = info.index;
            deviceInfo.type = info.type;
            deviceInfo.position = info.position;
            
            selected.append(deviceInfo);
        }
    }
    
    return selected;
}

void DeviceListWidget::setSelectedDevices(const QList<DeviceInfo>& devices)
{
    // Create a set of device identifiers for quick lookup
    QSet<QPair<int, DeviceType>> selectedSet;
    for (const DeviceInfo& device : devices) {
        selectedSet.insert(QPair<int, DeviceType>(device.index, device.type));
    }
    
    // Temporarily disconnect to avoid feedback loops
    disconnect(ui->deviceList, &QListWidget::itemChanged,
               this, &DeviceListWidget::onItemSelectionChanged);
               
    // Update list item checkboxes
    for (int i = 0; i < ui->deviceList->count(); i++) {
        QListWidgetItem* item = ui->deviceList->item(i);
        
        if (_itemToDevice.contains(item)) {
            const DeviceListInfo& info = _itemToDevice[item];
            
            QPair<int, DeviceType> deviceKey(info.index, info.type);
            bool selected = selectedSet.contains(deviceKey);
            
            item->setCheckState(selected ? Qt::Checked : Qt::Unchecked);
        }
    }
    
    // Reconnect signals
    connect(ui->deviceList, &QListWidget::itemChanged,
            this, &DeviceListWidget::onItemSelectionChanged);
}

void DeviceListWidget::onItemSelectionChanged()
{
    // Get currently selected devices based on checkbox state
    QList<DeviceInfo> selectedDevices = getSelectedDevices();
    
    // Emit selection changed signal with selected devices
    emit selectionChanged(selectedDevices);
}

void DeviceListWidget::selectAll()
{
    for (int i = 0; i < ui->deviceList->count(); i++) {
        QListWidgetItem* item = ui->deviceList->item(i);
        item->setCheckState(Qt::Checked);
    }
    
    emit selectionChanged(getSelectedDevices());
}

void DeviceListWidget::clearSelection()
{
    for (int i = 0; i < ui->deviceList->count(); i++) {
        QListWidgetItem* item = ui->deviceList->item(i);
        item->setCheckState(Qt::Unchecked);
    }
    
    emit selectionChanged(getSelectedDevices());
}

void DeviceListWidget::onSelectAllClicked()
{
    selectAll();
}

void DeviceListWidget::onClearAllClicked()
{
    clearSelection();
}

bool DeviceListWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->deviceList) {
        if (event->type() == QEvent::MouseButtonPress || 
            event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QListWidgetItem* item = ui->deviceList->itemAt(mouseEvent->pos());
            
            if (item) {
                // Toggle the checkbox state when clicking anywhere on the item
                Qt::CheckState newState = (item->checkState() == Qt::Checked) ? 
                                         Qt::Unchecked : Qt::Checked;
                item->setCheckState(newState);
                
                // Prevent the default handling that would select the item
                return true;
            }
        }
    }
    
    // Let other events pass through
    return QWidget::eventFilter(watched, event);
}

} // namespace Lightscape
