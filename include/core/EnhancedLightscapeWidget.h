/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EnhancedLightscapeWidget.h                                |
|                                                           |
| Enhanced main widget with updated effect system           |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QSplitter>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "grid/GridPanel.h"
#include "devices/DeviceControlWidget.h"
#include "assignments/AssignmentsWidget.h"
#include "effects/EnhancedEffectWidget.h"
#include "grid/NonRGBGridManager.h"

class EnhancedLightscapeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EnhancedLightscapeWidget(ResourceManager* resourceManager = nullptr, QWidget* parent = nullptr);
    ~EnhancedLightscapeWidget();

    // Core functionality
    void initializeGrid();
    SpatialGrid* getGrid() { return _spatialGrid; }

    // Basic UI functionality
    void handleGridVisibilityChanged(bool visible);
    void handleReset();

private slots:
    void onGridSettingsClicked();
    void onTabIndexChanged(int index);
    void onSetupTestClicked();

private:
    void setupWidgets();
    void setupConnections();

    ResourceManager* _resourceManager;
    DeviceManager* _deviceManager;
    SpatialGrid* _spatialGrid;

    // Non-RGB device management
    NonRGBDeviceManager* _nonRGBDeviceManager;
    NonRGBGridManager* _nonRGBGridManager;

    // UI Components
    QTabWidget* _tabWidget;
    GridPanel* _gridPanel;
    DeviceControlWidget* _deviceControlWidget;
    AssignmentsWidget* _assignmentsWidget;
    Lightscape::EnhancedEffectWidget* _effectWidget;

    // Main splitters
    QSplitter* _mainSplitter;
    QSplitter* _gridSplitter;
    QSplitter* _middleSplitter;
};
