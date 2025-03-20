/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EnhancedLightscapeWidget.cpp                              |
|                                                           |
| Enhanced main widget with updated effect system           |
\*---------------------------------------------------------*/

#include "core/EnhancedLightscapeWidget.h"
#include "grid/GridSettingsDialog.h"
#include "core/SettingsManager.h"
#include "core/SetupTestDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QDebug>

EnhancedLightscapeWidget::EnhancedLightscapeWidget(ResourceManager* resourceManager, QWidget* parent)
    : QWidget(parent)
    , _resourceManager(resourceManager)
    , _deviceManager(new DeviceManager(resourceManager))
    , _spatialGrid(new SpatialGrid())
    , _nonRGBDeviceManager(new NonRGBDeviceManager(this))
    , _nonRGBGridManager(new NonRGBGridManager(_spatialGrid, this))
    , _tabWidget(nullptr)
    , _gridPanel(nullptr)
    , _deviceControlWidget(nullptr)
    , _assignmentsWidget(nullptr)
    , _effectWidget(nullptr)
    , _mainSplitter(nullptr)
    , _gridSplitter(nullptr)
    , _middleSplitter(nullptr)
{
    setupWidgets();
    setupConnections();

    // Initialize settings manager
    SettingsManager::getInstance().initialize(_spatialGrid, _nonRGBDeviceManager, _deviceManager);
}

EnhancedLightscapeWidget::~EnhancedLightscapeWidget()
{
    delete _deviceManager;
    delete _spatialGrid;
}

void EnhancedLightscapeWidget::initializeGrid()
{
    if (_gridPanel) {
        _gridPanel->updateDisplay();
    }
}

void EnhancedLightscapeWidget::handleGridVisibilityChanged(bool visible)
{
    if (_gridPanel) {
        _gridPanel->setVisible(visible);
    }
}

void EnhancedLightscapeWidget::handleReset()
{
    // Clear grid selections
    if (_spatialGrid) {
        _spatialGrid->ClearSelection();
    }

    // Reset grid display
    if (_gridPanel) {
        _gridPanel->updateDisplay();
    }

    // Reset assignments
    if (_assignmentsWidget) {
        _assignmentsWidget->clearAssignments();
    }
}

void EnhancedLightscapeWidget::setupWidgets()
{
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // Create toolbar layout
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    mainLayout->addLayout(toolbarLayout);

    // Add grid settings button
    QPushButton* gridSettingsButton = new QPushButton("Grid Settings", this);
    toolbarLayout->addWidget(gridSettingsButton);
    connect(gridSettingsButton, &QPushButton::clicked, this, &EnhancedLightscapeWidget::onGridSettingsClicked);
    
    // Add setup & test button
    QPushButton* setupTestButton = new QPushButton("Setup & Test", this);
    toolbarLayout->addWidget(setupTestButton);
    connect(setupTestButton, &QPushButton::clicked, this, &EnhancedLightscapeWidget::onSetupTestClicked);

    // Add spacer to push buttons to the left
    toolbarLayout->addStretch();

    // Create tab widget for main sections
    _tabWidget = new QTabWidget(this);
    mainLayout->addWidget(_tabWidget);

    // Create Setup tab
    QWidget* setupTab = new QWidget(_tabWidget);
    QVBoxLayout* setupLayout = new QVBoxLayout(setupTab);
    setupTab->setLayout(setupLayout);

    // Create main horizontal splitter for setup tab
    _mainSplitter = new QSplitter(Qt::Horizontal, setupTab);
    setupLayout->addWidget(_mainSplitter);

    // Create and setup grid panel (left)
    QWidget* gridContainer = new QWidget(_mainSplitter);
    QVBoxLayout* gridLayout = new QVBoxLayout(gridContainer);
    _gridPanel = new GridPanel(_spatialGrid);
    gridLayout->addWidget(_gridPanel);
    gridContainer->setLayout(gridLayout);

    // Create middle control panel with vertical splitter
    _middleSplitter = new QSplitter(Qt::Vertical);
    
    // Device Control
    _deviceControlWidget = new DeviceControlWidget(_deviceManager);
    _middleSplitter->addWidget(_deviceControlWidget);
    
    // Assignments Panel
    _assignmentsWidget = new AssignmentsWidget(_deviceManager, _spatialGrid);
    _assignmentsWidget->setDeviceControlWidget(_deviceControlWidget);
    _middleSplitter->addWidget(_assignmentsWidget);

    // Add panels to main splitter
    _mainSplitter->addWidget(gridContainer);     // Left
    _mainSplitter->addWidget(_middleSplitter);   // Middle

    // Set initial splitter sizes (1:1)
    QList<int> sizes;
    sizes << 400 << 400;
    _mainSplitter->setSizes(sizes);

    // Set initial middle splitter sizes (1:1)
    QList<int> middleSizes;
    middleSizes << 200 << 200;
    _middleSplitter->setSizes(middleSizes);

    // Create Effects tab
    QWidget* effectsTab = new QWidget(_tabWidget);
    QVBoxLayout* effectsLayout = new QVBoxLayout(effectsTab);
    _effectWidget = new Lightscape::EnhancedEffectWidget(_deviceManager, _spatialGrid);
    effectsLayout->addWidget(_effectWidget);
    effectsTab->setLayout(effectsLayout);

    // Add tabs to the tab widget
    _tabWidget->addTab(setupTab, "Setup");
    _tabWidget->addTab(effectsTab, "Effects");

    // Set splitter properties for better appearance
    _mainSplitter->setHandleWidth(5);
    _middleSplitter->setHandleWidth(5);
    
    _mainSplitter->setChildrenCollapsible(false);
    _middleSplitter->setChildrenCollapsible(false);
}

void EnhancedLightscapeWidget::setupConnections()
{
    // Grid to assignments connections
    connect(_spatialGrid, &SpatialGrid::positionSelected,
            _assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);

    // Device control to assignments connections
    connect(_deviceControlWidget, &DeviceControlWidget::deviceSelectionChanged,
            _assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);
            
    // Tab widget connections
    connect(_tabWidget, &QTabWidget::currentChanged,
            this, &EnhancedLightscapeWidget::onTabIndexChanged);
}

void EnhancedLightscapeWidget::onGridSettingsClicked()
{
    GridSettingsDialog* dialog = new GridSettingsDialog(_spatialGrid, this);
    
    connect(dialog, &GridSettingsDialog::gridUpdated, [this]() {
        _gridPanel->updateDisplay();
        
        // Also update effect widget preview if it exists
        if (_effectWidget) {
            _effectWidget->refresh();
        }
    });

    dialog->exec();
    delete dialog;
}

void EnhancedLightscapeWidget::onTabIndexChanged(int index)
{
    // When switching to Effects tab, refresh the effect widget
    if (index == 1 && _effectWidget) {
        _effectWidget->refresh();
    }
}

void EnhancedLightscapeWidget::onSetupTestClicked()
{
    // Create and show the setup test dialog
    Lightscape::SetupTestDialog* dialog = new Lightscape::SetupTestDialog(_deviceManager, _spatialGrid, this);
    dialog->exec();
    delete dialog;
}
