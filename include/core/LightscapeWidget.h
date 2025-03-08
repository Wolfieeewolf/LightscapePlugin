#pragma once

#include <QWidget>
#include <QSplitter>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "grid/GridPanel.h"
#include "devices/DeviceControlWidget.h"
#include "assignments/AssignmentsWidget.h"
#include "effects/EffectWidget.h"
#include "grid/NonRGBGridManager.h"

class LightscapeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LightscapeWidget(ResourceManager* resourceManager = nullptr, QWidget* parent = nullptr);
    ~LightscapeWidget();

    // Core functionality
    void initializeGrid();
    SpatialGrid* getGrid() { return spatialGrid; }

    // Basic UI functionality
    void handleGridVisibilityChanged(bool visible);
    void handleReset();

private slots:
    void onGridSettingsClicked();

private:
    void setupWidgets();
    void setupConnections();

    ResourceManager* resourceManager;
    DeviceManager* deviceManager;
    SpatialGrid* spatialGrid;

    // Non-RGB device management
    NonRGBDeviceManager* nonRGBDeviceManager;
    NonRGBGridManager* nonRGBGridManager;

    // UI Components
    GridPanel* gridPanel;
    DeviceControlWidget* deviceControlWidget;
    AssignmentsWidget* assignmentsWidget;
    EffectWidget* effectWidget;

    // Main splitters
    QSplitter* mainSplitter;
    QSplitter* middleSplitter;
};