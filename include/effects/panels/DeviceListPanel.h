/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| DeviceListPanel.h                                         |
|                                                           |
| Panel for device selection                                |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "effects/DeviceListWidget.h"
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "core/Types.h"

namespace Ui {
class DeviceListPanel;
}

namespace Lightscape {

class DeviceListPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceListPanel(QWidget *parent = nullptr);
    ~DeviceListPanel();
    
    // Initialize
    void init(::DeviceManager* deviceManager, ::SpatialGrid* grid);
    
    // Device management
    void refresh();
    QList<DeviceInfo> getSelectedDevices() const;
    void setSelectedDevices(const QList<DeviceInfo>& devices);
    void selectAllDevices(); // Add new method to select all devices
    
    // Expose the widget
    DeviceListWidget* getDeviceListWidget() const;
    
signals:
    void selectionChanged(const QList<DeviceInfo>& devices);
    void selectAllClicked();
    void clearAllClicked();
    
private:
    Ui::DeviceListPanel *ui;
    DeviceListWidget* _deviceListWidget;
    
    void setupUI();
    void setupConnections();
};

} // namespace Lightscape