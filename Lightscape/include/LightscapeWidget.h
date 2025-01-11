#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include "DeviceManager.h"
#include "SpatialGrid.h"
#include "effects/EffectEngine.h"

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
    void on_setUserPositionClicked();
    void on_clearUserPositionClicked();
    void on_userPositionRequired(const QString& warning);
    void on_userPositionChanged(const GridPosition& pos);
    void on_effectComboChanged(int index);
    void on_speedSliderChanged(int value);
    void on_intensitySliderChanged(int value);

private:
    Ui::LightscapeWidget* ui;
    DeviceManager device_manager;
    SpatialGrid spatial_grid;
    EffectEngine* effect_engine;
    QPushButton* set_user_position_button;
    QPushButton* clear_user_position_button;
    QLabel* warning_label;
    
    void setupConnections();
    void setupInitialState();
    void setupUserPositionControls();
    void setupEffectControls();
    void populateDeviceList();
    void updateSelectionCombo();
    void updateAssignmentsList();
    void refreshGridDisplay();
    void updateWarningLabel(const QString& warning);
    void updateDeviceColors();
};