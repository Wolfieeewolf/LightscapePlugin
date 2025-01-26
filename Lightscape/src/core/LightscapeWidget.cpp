#include "core/LightscapeWidget.h"
#include "ui_LightscapeWidget.h"
#include "grid/GridSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>

LightscapeWidget::LightscapeWidget(ResourceManager* resourceManager, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::LightscapeWidget),
    resourceManager(resourceManager),
    deviceManager(new DeviceManager(resourceManager)),
    spatialGrid(new SpatialGrid()),
    effectEngine(new EffectEngine(spatialGrid)),
    gameEffectManager(new GameEffectManager(spatialGrid)),
    nonRGBDeviceManager(new NonRGBDeviceManager(this)),
    nonRGBGridManager(new NonRGBGridManager(spatialGrid, this))
{
    setupWidgets();
    setupConnections();
}

LightscapeWidget::~LightscapeWidget()
{
    delete ui;
    delete deviceControlWidget;
    delete assignmentsWidget;
    delete effectsPanel;
    delete nonRGBDevicesPanel;
    delete gridPanel;
    delete deviceManager;
    delete spatialGrid;
    delete effectEngine;
    delete gameEffectManager;
    delete nonRGBDeviceManager;
    delete nonRGBGridManager;
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
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);

    // Create and setup grid panel (left)
    QWidget* gridContainer = new QWidget(mainSplitter);
    QVBoxLayout* gridLayout = new QVBoxLayout(gridContainer);
    gridPanel = new GridPanel(spatialGrid);
    gridLayout->addWidget(gridPanel);
    gridContainer->setLayout(gridLayout);

    // Create middle control panel with vertical splitter
    QSplitter* middleSplitter = new QSplitter(Qt::Vertical);
    
    // Device Control
    deviceControlWidget = new DeviceControlWidget(deviceManager);
    middleSplitter->addWidget(deviceControlWidget);
    
    // Non-RGB Devices Panel
    nonRGBDevicesPanel = new NonRGBDevicesPanel(nonRGBDeviceManager, nonRGBGridManager);
    middleSplitter->addWidget(nonRGBDevicesPanel);
    
    // Assignments Panel
    assignmentsWidget = new AssignmentsWidget(deviceManager, spatialGrid);
    assignmentsWidget->setDeviceControlWidget(deviceControlWidget);
    middleSplitter->addWidget(assignmentsWidget);

    // Set initial middle splitter sizes (1:1:1)
    QList<int> middleSizes;
    middleSizes << 200 << 200 << 200;
    middleSplitter->setSizes(middleSizes);

    // Create effects panel (right)
    effectsPanel = new EffectsPanel(effectEngine, gameEffectManager);

    // Add panels to main splitter
    mainSplitter->addWidget(gridContainer);     // Left
    mainSplitter->addWidget(middleSplitter);    // Middle
    mainSplitter->addWidget(effectsPanel);      // Right

    // Set initial main splitter sizes (1:1:1)
    QList<int> sizes;
    sizes << 300 << 300 << 300;
    mainSplitter->setSizes(sizes);

    // Set stretch factors for better resizing behavior
    mainSplitter->setStretchFactor(0, 1);  // Grid
    mainSplitter->setStretchFactor(1, 1);  // Middle panels
    mainSplitter->setStretchFactor(2, 1);  // Effects

    // Set splitter properties for better appearance
    mainSplitter->setHandleWidth(5);
    middleSplitter->setHandleWidth(5);
    
    mainSplitter->setChildrenCollapsible(false);
    middleSplitter->setChildrenCollapsible(false);
}

void LightscapeWidget::setupConnections()
{
    // Connect grid selections to assignments widget
    connect(spatialGrid, &SpatialGrid::positionSelected,
            assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);

    // Connect device control signals
    connect(deviceControlWidget, &DeviceControlWidget::deviceSelectionChanged,
            assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);

    // Add color change handling
    connect(assignmentsWidget, &AssignmentsWidget::colorChangeRequested,
            assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);

    // Connect effects to device updates
    connect(effectEngine, &EffectEngine::colorsUpdated,
            assignmentsWidget, &AssignmentsWidget::updateAssignmentsList);
}

void LightscapeWidget::onGridSettingsClicked()
{
    GridSettingsDialog* dialog = new GridSettingsDialog(spatialGrid, this);
    
    // Connect the dialog's gridUpdated signal to the panel's updateDisplay method
    connect(dialog, &GridSettingsDialog::gridUpdated, [this]() {
        gridPanel->updateDisplay();
    });

    // Show the dialog modally
    dialog->exec();
    
    // Clean up
    delete dialog;
}