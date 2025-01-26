#pragma once

#include <QWidget>
#include "core/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "effects/base/EffectEngine.h"
#include "effects/game/GameEffectManager.h"
#include "grid/GridPanel.h"
#include "devices/DeviceControlWidget.h"
#include "assignments/AssignmentsWidget.h"
#include "effects/ui/EffectsPanel.h"
#include "devices/NonRGBDeviceManager.h"
#include "grid/NonRGBGridManager.h"
#include "devices/NonRGBDevicesPanel.h"

namespace Ui {
class LightscapeWidget;
}

class LightscapeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LightscapeWidget(ResourceManager* resourceManager = nullptr, QWidget* parent = nullptr);
    ~LightscapeWidget();

private slots:
    void onGridSettingsClicked();

private:
    void setupWidgets();
    void setupConnections();

    Ui::LightscapeWidget *ui;
    ResourceManager *resourceManager;
    DeviceManager *deviceManager;
    SpatialGrid *spatialGrid;
    EffectEngine *effectEngine;
    GameEffectManager *gameEffectManager;

    // Non-RGB device management
    NonRGBDeviceManager *nonRGBDeviceManager;
    NonRGBGridManager *nonRGBGridManager;

    // UI Components
    GridPanel *gridPanel;
    DeviceControlWidget *deviceControlWidget;
    AssignmentsWidget *assignmentsWidget;
    EffectsPanel *effectsPanel;
    NonRGBDevicesPanel *nonRGBDevicesPanel;
};