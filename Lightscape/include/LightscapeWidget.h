#pragma once

#include <QWidget>
#include <QTimer>
#include "DeviceManager.h"
#include "SpatialGrid.h"

namespace Ui {
class LightscapeWidget;
}

class LightscapeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LightscapeWidget(ResourceManager* resource_manager = nullptr, QWidget* parent = nullptr);
    ~LightscapeWidget();

private slots:
    void on_configureButton_clicked();
    void on_deviceComboChanged(int index);
    void on_assignmentTypeChanged();
    void on_assignButtonClicked();
    void on_removeButtonClicked();
    void on_clearButtonClicked();
    void on_colorButtonClicked();
    void on_gridPositionSelected(const GridPosition& pos);
    void on_updateColors();

private:
    Ui::LightscapeWidget* ui;
    DeviceManager device_manager;
    SpatialGrid spatial_grid;
    QTimer update_timer;
    
    void setupConnections();
    void setupInitialState();
    void populateDeviceList();
    void updateSelectionCombo();
    void updateAssignmentsList();
    void refreshGridDisplay();
};