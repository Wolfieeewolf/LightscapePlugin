#include "core/SetupTestDialog.h"
#include "core/PositionTestTab.h"
#include "core/LayerTestTab.h"
#include "core/PatternTestTab.h"
#include "core/ValidationTab.h"

#include <QColorDialog>

using namespace Lightscape;

SetupTestDialog::SetupTestDialog(DeviceManager* deviceManager, SpatialGrid* grid, QWidget* parent)
    : QDialog(parent)
    , deviceManager(deviceManager)
    , spatialGrid(grid)
    , testColor(ToRGBColor(255, 0, 0)) // Default to red
{
    // Create utility classes
    testUtility = new SetupTestUtility(deviceManager, grid, this);
    gridValidator = new GridValidator(grid, this);
    
    // Connect to common events
    connect(testUtility, &SetupTestUtility::positionTested, this, &SetupTestDialog::onPositionTested);
    
    setWindowTitle("Spatial Grid Setup & Test");
    setMinimumSize(700, 600);
    
    // No global style - let Qt handle default styling
    
    setupUI();
}

SetupTestDialog::~SetupTestDialog()
{
    // Clean up any running patterns
    if (testUtility) {
        testUtility->stopPattern();
    }
}

void SetupTestDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Add header with explanation
    mainLayout->addWidget(createHeaderWidget());
    
    // Create tab widget
    tabWidget = new QTabWidget(this);
    
    // Create tabs
    positionTestTab = new PositionTestTab(this, deviceManager, spatialGrid, tabWidget);
    layerTestTab = new LayerTestTab(this, deviceManager, spatialGrid, tabWidget);
    patternTestTab = new PatternTestTab(this, deviceManager, spatialGrid, tabWidget);
    validationTab = new ValidationTab(this, spatialGrid, tabWidget);
    
    // Add tabs to widget
    tabWidget->addTab(positionTestTab, "Position Test");
    tabWidget->addTab(layerTestTab, "Layer Test");
    tabWidget->addTab(patternTestTab, "Pattern Test");
    tabWidget->addTab(validationTab, "Validation");
    
    // Connect tab change signal
    connect(tabWidget, &QTabWidget::currentChanged, this, &SetupTestDialog::onTabChanged);
    
    // Add the tab widget to the main layout
    mainLayout->addWidget(tabWidget);
    
    // Add close button
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

QWidget* SetupTestDialog::createHeaderWidget()
{
    QWidget* headerWidget = new QWidget(this);
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    
    QLabel* titleLabel = new QLabel("Spatial Grid Setup & Test Utility", headerWidget);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    headerLayout->addWidget(titleLabel);
    
    QLabel* descriptionLabel = new QLabel(
        "This utility helps you verify your spatial grid configuration and device assignments. "
        "Use the tabs below to test individual positions, entire layers, or spatial patterns to "
        "ensure your setup works correctly for effects.", headerWidget);
    descriptionLabel->setWordWrap(true);
    headerLayout->addWidget(descriptionLabel);
    
    // Add coordinate system explanation
    QLabel* coordExplainLabel = new QLabel(
        "<b>Understanding Grid Coordinates:</b><br>"
        "• <b>X</b> = Horizontal position (left to right)<br>"
        "• <b>Y</b> = Vertical position (top to bottom)<br>"
        "• <b>Z</b> = Layer (Z=0 is bottom layer, higher numbers are higher layers)", 
        headerWidget);
    coordExplainLabel->setStyleSheet("padding: 5px; border: 1px solid #ccc;");
    coordExplainLabel->setWordWrap(true);
    headerLayout->addWidget(coordExplainLabel);
    
    headerWidget->setLayout(headerLayout);
    return headerWidget;
}

void SetupTestDialog::setTestColor(RGBColor color)
{
    testColor = color;
    testUtility->setTestColor(color);
    
    // Inform the position test tab about the change
    QColor newColor(RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color));
    PositionTestTab* posTab = qobject_cast<PositionTestTab*>(positionTestTab);
    if (posTab) {
        posTab->updateColorButton(color);
    }
}

void SetupTestDialog::stopAllTests()
{
    // Stop all running tests in the utility
    testUtility->stopAllTests();
}

void SetupTestDialog::onResetAllDevices()
{
    // Reset all devices to their default state
    testUtility->resetAllDevices();
    
    // Clear visualizers in all tabs
    PositionTestTab* posTab = qobject_cast<PositionTestTab*>(positionTestTab);
    if (posTab) {
        posTab->getVisualizer()->clearColors();
    }
    
    LayerTestTab* layerTab = qobject_cast<LayerTestTab*>(layerTestTab);
    if (layerTab) {
        layerTab->getVisualizer()->clearColors();
    }
    
    PatternTestTab* patternTab = qobject_cast<PatternTestTab*>(patternTestTab);
    if (patternTab) {
        patternTab->getVisualizer()->clearColors();
    }
}

void SetupTestDialog::onPositionTested(const GridPosition& pos)
{
    int currentTabIndex = tabWidget->currentIndex();
    
    // Forward position tested event to the appropriate tab
    switch (currentTabIndex) {
        case 0: // Position Test tab
            if (qobject_cast<PositionTestTab*>(positionTestTab)) {
                qobject_cast<PositionTestTab*>(positionTestTab)->onPositionTested(pos);
            }
            break;
            
        case 1: // Layer Test tab
            if (qobject_cast<LayerTestTab*>(layerTestTab)) {
                qobject_cast<LayerTestTab*>(layerTestTab)->onPositionTested(pos);
            }
            break;
            
        case 2: // Pattern Test tab
            if (qobject_cast<PatternTestTab*>(patternTestTab)) {
                qobject_cast<PatternTestTab*>(patternTestTab)->onPositionTested(pos);
            }
            break;
    }
}

void SetupTestDialog::onTabChanged(int index)
{
    // When switching to the Layer Test tab, force a refresh of the visualizer
    if (index == 1) { // Layer Test tab
        LayerTestTab* layerTab = qobject_cast<LayerTestTab*>(layerTestTab);
        if (layerTab) {
            // Force visualizer to update
            layerTab->populateSelector();
            
            // Make sure the visualizer is updated
            int layerIndex = 0; // Default to first layer
            if (layerTab->getLayerSelector() && layerTab->getLayerSelector()->count() > 0) {
                layerIndex = layerTab->getLayerSelector()->currentData().toInt();
            }
            
            if (layerTab->getVisualizer()) {
                layerTab->getVisualizer()->setActiveLayer(layerIndex);
                layerTab->getVisualizer()->update();
            }
        }
    }
}