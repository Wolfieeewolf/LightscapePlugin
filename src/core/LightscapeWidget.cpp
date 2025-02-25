#include "core/LightscapeWidget.h"
#include "grid/GridSettingsDialog.h"
#include "core/SettingsManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

LightscapeWidget::LightscapeWidget(ResourceManager* resourceManager, QWidget* parent)
    : QWidget(parent)
    , resourceManager(resourceManager)
    , deviceManager(new DeviceManager(resourceManager))
    , spatialGrid(new SpatialGrid())
    , nonRGBDeviceManager(new NonRGBDeviceManager(this))
    , nonRGBGridManager(new NonRGBGridManager(spatialGrid, this))
    , gridPanel(nullptr)
    , deviceControlWidget(nullptr)
    , assignmentsWidget(nullptr)
    , effectWidget(nullptr)
    , mainSplitter(nullptr)
    , middleSplitter(nullptr)
{
    setupWidgets();
    setupConnections();

    // Initialize settings manager
    SettingsManager::getInstance().initialize(spatialGrid, nonRGBDeviceManager, deviceManager);
}

LightscapeWidget::~LightscapeWidget()
{
    delete deviceManager;
    delete spatialGrid;
}

void LightscapeWidget::initializeGrid()
{
    if (gridPanel) {
        gridPanel->updateDisplay();
    }
}

void LightscapeWidget::handleGridVisibilityChanged(bool visible)
{
    if (gridPanel) {
        gridPanel->setVisible(visible);
    }
}

void LightscapeWidget::handleReset()
{
    // Clear grid selections
    if (spatialGrid) {
        spatialGrid->ClearSelection();
    }

    // Reset grid display
    if (gridPanel) {
        gridPanel->updateDisplay();
    }

    // Reset assignments
    if (assignmentsWidget) {
        assignmentsWidget->clearAssignments();
    }
}

void LightscapeWidget::setupWidgets()
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
    connect(gridSettingsButton, &QPushButton::clicked, this, &LightscapeWidget::onGridSettingsClicked);

    // Add spacer to push button to the left
    toolbarLayout->addStretch();

    // Create main horizontal splitter
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);

    // Create and setup grid panel (left)
    QWidget* gridContainer = new QWidget(mainSplitter);
    QVBoxLayout* gridLayout = new QVBoxLayout(gridContainer);
    gridPanel = new GridPanel(spatialGrid);
    gridLayout->addWidget(gridPanel);
    gridContainer->setLayout(gridLayout);

    // Create middle control panel with vertical splitter
    middleSplitter = new QSplitter(Qt::Vertical);
    
    // Device Control
    deviceControlWidget = new DeviceControlWidget(deviceManager);
    middleSplitter->addWidget(deviceControlWidget);
    
    // Assignments Panel
    assignmentsWidget = new AssignmentsWidget(deviceManager, spatialGrid);
    assignmentsWidget->setDeviceControlWidget(deviceControlWidget);
    middleSplitter->addWidget(assignmentsWidget);

    // Create effects panel (right side)
    effectWidget = new EffectWidget(deviceManager, spatialGrid);
    effectWidget->setMinimumWidth(200);
    
    // Add panels to main splitter
    mainSplitter->addWidget(gridContainer);     // Left
    mainSplitter->addWidget(middleSplitter);    // Middle
    mainSplitter->addWidget(effectWidget); // Right

    // Set initial splitter sizes (1:1:1)
    QList<int> sizes;
    sizes << 300 << 300 << 300;
    mainSplitter->setSizes(sizes);

    // Set initial middle splitter sizes (1:1)
    QList<int> middleSizes;
    middleSizes << 200 << 200;
    middleSplitter->setSizes(middleSizes);

    // Set stretch factors for better resizing behavior
    mainSplitter->setStretchFactor(0, 1);  // Grid
    mainSplitter->setStretchFactor(1, 1);  // Middle panels
    mainSplitter->setStretchFactor(2, 1);  // Effects placeholder

    // Set splitter properties for better appearance
    mainSplitter->setHandleWidth(5);
    middleSplitter->setHandleWidth(5);
    
    mainSplitter->setChildrenCollapsible(false);
    middleSplitter->setChildrenCollapsible(false);
}

void LightscapeWidget::setupConnections()
{
    // Grid to assignments connections
    connect(spatialGrid, &SpatialGrid::positionSelected,
            assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);

    // Device control to assignments connections
    connect(deviceControlWidget, &DeviceControlWidget::deviceSelectionChanged,
            assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);
}

void LightscapeWidget::onGridSettingsClicked()
{
    GridSettingsDialog* dialog = new GridSettingsDialog(spatialGrid, this);
    
    connect(dialog, &GridSettingsDialog::gridUpdated, [this]() {
        gridPanel->updateDisplay();
    });

    dialog->exec();
    delete dialog;
}