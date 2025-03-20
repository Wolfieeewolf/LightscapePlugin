/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| DeviceListWidget.h                                        |
|                                                           |
| Widget for device selection                               |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QMap>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "core/Types.h"

namespace Ui {
class DeviceListWidget;
}

namespace Lightscape {

class DeviceListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceListWidget(QWidget *parent = nullptr);
    ~DeviceListWidget();
    
    // Device initialization
    void init(::DeviceManager* deviceManager, ::SpatialGrid* grid);
    void refresh();
    
    // Selection
    QList<DeviceInfo> getSelectedDevices() const;
    void setSelectedDevices(const QList<DeviceInfo>& devices);
    void selectAll();
    void clearSelection();
    
signals:
    void selectionChanged(const QList<DeviceInfo>& devices);
    
private slots:
    void onItemSelectionChanged();
    void onSelectAllClicked();
    void onClearAllClicked();
    bool eventFilter(QObject* watched, QEvent* event) override;
    
private:
    Ui::DeviceListWidget *ui;
    ::DeviceManager* _deviceManager = nullptr;
    ::SpatialGrid* _spatialGrid = nullptr;
    
    struct DeviceListInfo {
        int index;
        DeviceType type;
        GridPosition position;
        bool hasPosition;
    };
    
    QMap<QListWidgetItem*, DeviceListInfo> _itemToDevice;
};

} // namespace Lightscape
