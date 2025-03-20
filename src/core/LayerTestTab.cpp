#include "core/LayerTestTab.h"
#include "core/SetupTestDialog.h"
#include <QMessageBox>
#include <QColorDialog>

namespace Lightscape {

LayerTestTab::LayerTestTab(SetupTestDialog* parentDialog, DeviceManager* deviceManager, 
                          SpatialGrid* grid, QWidget* parent)
    : QWidget(parent)
    , parentDialog(parentDialog)
    , deviceManager(deviceManager)
    , spatialGrid(grid)
{
    // Get test utility from parent dialog
    testUtility = parentDialog->getTestUtility();
    
    // Create layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create split view
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    
    // --- Left side - visualizer ---
    QWidget* leftWidget = new QWidget(splitter);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    
    // Add info label
    QLabel* layerInfoLabel = new QLabel(
        "<b>Select a layer to test using the buttons below.</b><br>"
        "The test will light up all devices in the selected layer sequentially.",
        leftWidget);
    layerInfoLabel->setWordWrap(true);
    layerInfoLabel->setStyleSheet("padding: 5px; border-radius: 3px;");
    leftLayout->addWidget(layerInfoLabel);
    
    // Add grid visualizer - same settings as other tabs
    QGroupBox* visGroup = new QGroupBox("Layer Preview", leftWidget);
    QVBoxLayout* visLayout = new QVBoxLayout(visGroup);
    
    visualizer = new TestGridVisualizer(spatialGrid, visGroup);
    visualizer->setMinimumHeight(250);
    visualizer->toggleCoordinateDisplay(true);
    visLayout->addWidget(visualizer);
    
    visGroup->setLayout(visLayout);
    leftLayout->addWidget(visGroup);
    
    // Add layer selector buttons
    QGroupBox* layerButtonGroup = new QGroupBox("Select Layer", leftWidget);
    QHBoxLayout* layerButtonLayout = new QHBoxLayout(layerButtonGroup);
    
    // Get grid dimensions to know how many layers
    GridDimensions dims = spatialGrid->GetDimensions();
    
    for (int z = 0; z < dims.depth; z++) {
        QString layerName = spatialGrid->GetLayerLabel(z);
        if (layerName.isEmpty()) {
            layerName = QString("Layer %1").arg(z);
        }
        
        QPushButton* layerBtn = new QPushButton(layerName, layerButtonGroup);
        layerBtn->setCheckable(true);
        layerBtn->setProperty("layer", z);
        
        if (z == 0) {
            layerBtn->setChecked(true);
        }
        
        connect(layerBtn, &QPushButton::clicked, [this, layerBtn, z]() {
            // Uncheck other layer buttons
            QList<QPushButton*> buttons = layerBtn->parentWidget()->findChildren<QPushButton*>();
            for (auto btn : buttons) {
                if (btn->property("layer").isValid() && btn != layerBtn) {
                    btn->setChecked(false);
                }
            }
            
            // Set active layer in visualizer
            visualizer->setActiveLayer(z);
            
            // Store the selected layer for testing
            layerSelector->setCurrentIndex(layerSelector->findData(z));
        });
        
        layerButtonLayout->addWidget(layerBtn);
    }
    
    layerButtonLayout->addStretch();
    layerButtonGroup->setLayout(layerButtonLayout);
    leftLayout->addWidget(layerButtonGroup);
    
    leftLayout->addStretch();
    leftWidget->setLayout(leftLayout);
    
    // --- Right side - controls ---
    QWidget* rightWidget = new QWidget(splitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    
    // Add info text
    QGroupBox* infoGroup = new QGroupBox("Layer Test", rightWidget);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
    
    infoLabel = new QLabel(
        "<b>What this test does:</b><br>"
        "This test sequentially lights up all devices in the selected layer one by one. "
        "The devices will light up in order from left to right, top to bottom, allowing you to "
        "verify that your spatial assignments match your physical device layout.", infoGroup);
    infoLabel->setWordWrap(true);
    infoLayout->addWidget(infoLabel);
    
    infoGroup->setLayout(infoLayout);
    rightLayout->addWidget(infoGroup);
    
    // Keep a hidden layer selector for compatibility
    layerSelector = new QComboBox(rightWidget);
    layerSelector->setVisible(false);
    
    // Color selection - same controls as other tabs
    QGroupBox* colorGroup = new QGroupBox("Test Color", rightWidget);
    QVBoxLayout* colorLayout = new QVBoxLayout(colorGroup);
    
    QHBoxLayout* colorButtonLayout = new QHBoxLayout();
    QLabel* colorLabel = new QLabel("Choose color:", colorGroup);
    colorButtonLayout->addWidget(colorLabel);
    
    RGBColor testColor = parentDialog->getTestColor();
    QPushButton* colorBtn = new QPushButton(colorGroup);
    colorBtn->setFixedSize(60, 30);
    colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                              .arg(RGBGetRValue(testColor))
                              .arg(RGBGetGValue(testColor))
                              .arg(RGBGetBValue(testColor)));
    connect(colorBtn, &QPushButton::clicked, [this, parentDialog, colorBtn]() {
        // Get current color
        RGBColor currentColor = parentDialog->getTestColor();
        
        // Show color dialog
        QColor initialColor(RGBGetRValue(currentColor), RGBGetGValue(currentColor), RGBGetBValue(currentColor));
        QColor newColor = QColorDialog::getColor(initialColor, this, "Select Test Color");
        
        if (newColor.isValid()) {
            // Update the color in parent dialog
            RGBColor rgbColor = ToRGBColor(newColor.red(), newColor.green(), newColor.blue());
            parentDialog->setTestColor(rgbColor);
            
            // Update button color
            colorBtn->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                  .arg(newColor.red())
                                  .arg(newColor.green())
                                  .arg(newColor.blue()));
        }
    });
    colorButtonLayout->addWidget(colorBtn);
    
    // Add quick color buttons
    QHBoxLayout* quickColorsLayout = new QHBoxLayout();
    quickColorsLayout->addWidget(new QLabel("Quick Colors:"));
    
    // Red button
    QPushButton* redBtn = new QPushButton(colorGroup);
    redBtn->setFixedSize(24, 24);
    redBtn->setStyleSheet("background-color: rgb(255, 0, 0); border-radius: 12px;");
    connect(redBtn, &QPushButton::clicked, [this, parentDialog, colorBtn]() {
        RGBColor color = ToRGBColor(255, 0, 0);
        parentDialog->setTestColor(color);
        colorBtn->setStyleSheet("background-color: rgb(255, 0, 0);");
    });
    quickColorsLayout->addWidget(redBtn);
    
    // Green button
    QPushButton* greenBtn = new QPushButton(colorGroup);
    greenBtn->setFixedSize(24, 24);
    greenBtn->setStyleSheet("background-color: rgb(0, 255, 0); border-radius: 12px;");
    connect(greenBtn, &QPushButton::clicked, [this, parentDialog, colorBtn]() {
        RGBColor color = ToRGBColor(0, 255, 0);
        parentDialog->setTestColor(color);
        colorBtn->setStyleSheet("background-color: rgb(0, 255, 0);");
    });
    quickColorsLayout->addWidget(greenBtn);
    
    // Blue button
    QPushButton* blueBtn = new QPushButton(colorGroup);
    blueBtn->setFixedSize(24, 24);
    blueBtn->setStyleSheet("background-color: rgb(0, 0, 255); border-radius: 12px;");
    connect(blueBtn, &QPushButton::clicked, [this, parentDialog, colorBtn]() {
        RGBColor color = ToRGBColor(0, 0, 255);
        parentDialog->setTestColor(color);
        colorBtn->setStyleSheet("background-color: rgb(0, 0, 255);");
    });
    quickColorsLayout->addWidget(blueBtn);
    
    // White button
    QPushButton* whiteBtn = new QPushButton(colorGroup);
    whiteBtn->setFixedSize(24, 24);
    whiteBtn->setStyleSheet("background-color: rgb(255, 255, 255); border-radius: 12px; border: 1px solid #aaa;");
    connect(whiteBtn, &QPushButton::clicked, [this, parentDialog, colorBtn]() {
        RGBColor color = ToRGBColor(255, 255, 255);
        parentDialog->setTestColor(color);
        colorBtn->setStyleSheet("background-color: rgb(255, 255, 255);");
    });
    quickColorsLayout->addWidget(whiteBtn);
    
    quickColorsLayout->addStretch();
    
    colorLayout->addLayout(colorButtonLayout);
    colorLayout->addLayout(quickColorsLayout);
    
    rightLayout->addWidget(colorGroup);
    
    // Test settings
    QGroupBox* delayGroup = new QGroupBox("Test Settings", rightWidget);
    QVBoxLayout* delayLayout = new QVBoxLayout(delayGroup);
    
    QLabel* delayLabel = new QLabel("Delay between positions (ms):", delayGroup);
    delayLayout->addWidget(delayLabel);
    
    QHBoxLayout* delaySliderLayout = new QHBoxLayout();
    delaySlider = new QSlider(Qt::Horizontal, delayGroup);
    delaySlider->setRange(100, 1000);
    delaySlider->setValue(300);
    delaySlider->setTickInterval(100);
    delaySlider->setTickPosition(QSlider::TicksBelow);
    connect(delaySlider, &QSlider::valueChanged, this, &LayerTestTab::onDelaySliderChanged);
    delaySliderLayout->addWidget(delaySlider);
    
    delayValueLabel = new QLabel("300", delayGroup);
    delayValueLabel->setMinimumWidth(40);
    delayValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    delaySliderLayout->addWidget(delayValueLabel);
    
    delayLayout->addLayout(delaySliderLayout);
    
    delayGroup->setLayout(delayLayout);
    rightLayout->addWidget(delayGroup);
    
    // Actions
    QGroupBox* actionsGroup = new QGroupBox("Actions", rightWidget);
    QVBoxLayout* actionsLayout = new QVBoxLayout(actionsGroup);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    testLayerButton = new QPushButton("Test Layer", actionsGroup);
    testLayerButton->setToolTip("Test all devices in this layer sequentially");
    connect(testLayerButton, &QPushButton::clicked, this, &LayerTestTab::onTestLayerClicked);
    buttonLayout->addWidget(testLayerButton);
    
    stopTestButton = new QPushButton("Stop Test", actionsGroup);
    stopTestButton->setToolTip("Stop the current test");
    connect(stopTestButton, &QPushButton::clicked, this, &LayerTestTab::onStopTestClicked);
    buttonLayout->addWidget(stopTestButton);
    
    actionsLayout->addLayout(buttonLayout);
    
    resetButton = new QPushButton("Reset All Devices", actionsGroup);
    resetButton->setToolTip("Turn off all devices");
    connect(resetButton, &QPushButton::clicked, this, &LayerTestTab::onResetDevicesClicked);
    actionsLayout->addWidget(resetButton);
    
    actionsGroup->setLayout(actionsLayout);
    rightLayout->addWidget(actionsGroup);
    rightLayout->addStretch();
    
    rightWidget->setLayout(rightLayout);
    
    // Add both sides to the splitter
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    
    // Set splitter proportions (same as other tabs)
    QList<int> splitSizes;
    splitSizes << 400 << 300;
    splitter->setSizes(splitSizes);
    
    // Set initial layer to visualize
    visualizer->setActiveLayer(0);
    
    mainLayout->addWidget(splitter);
    
    // Populate selector with available layers
    populateSelector();
}

void LayerTestTab::populateSelector()
{
    // Block signals to prevent triggering callbacks during setup
    layerSelector->blockSignals(true);
    
    // Clear existing items
    layerSelector->clear();
    
    // Get grid dimensions
    GridDimensions dims = spatialGrid->GetDimensions();
    
    // Populate layer selector
    for (int z = 0; z < dims.depth; z++) {
        QString layerName = spatialGrid->GetLayerLabel(z);
        if (layerName.isEmpty()) {
            layerName = QString("Layer %1").arg(z);
        }
        
        // Add layer position for clarity
        QString layerDisplay = QString("%1 (Z=%2)").arg(layerName).arg(z);
        layerSelector->addItem(layerDisplay, z);
    }
    
    // Restore signal handling
    layerSelector->blockSignals(false);
    
    // Set initial layer if available
    if (layerSelector->count() > 0) {
        onLayerSelectionChanged(0);
    }
    
    // Set button enabled state
    bool hasLayers = dims.depth > 0;
    testLayerButton->setEnabled(hasLayers);
    
    // Force update of visualizer
    visualizer->update();
}

void LayerTestTab::setButtonsEnabled(bool enabled)
{
    testLayerButton->setEnabled(enabled && layerSelector->count() > 0);
    stopTestButton->setEnabled(enabled);
    resetButton->setEnabled(enabled);
}

void LayerTestTab::onLayerSelectionChanged(int /*index*/)
{
    // Update visualizer for the selected layer
    int layerIndex = layerSelector->currentData().toInt();
    visualizer->setActiveLayer(layerIndex);
    visualizer->update();
}

void LayerTestTab::onDelaySliderChanged(int value)
{
    delayValueLabel->setText(QString::number(value));
}

void LayerTestTab::onTestLayerClicked()
{
    int layerIndex = layerSelector->currentData().toInt();
    int delay = delaySlider->value();
    
    // Clear visualizer
    visualizer->clearColors();
    
    // Test the layer
    QList<GridPosition> positions = testUtility->testLayer(layerIndex, delay);
    
    if (positions.isEmpty()) {
        QMessageBox::information(this, "No Assignments", 
                               "There are no device assignments in this layer.");
    }
}

void LayerTestTab::onStopTestClicked()
{
    // Stop any running tests
    parentDialog->stopAllTests();
}

void LayerTestTab::onResetDevicesClicked()
{
    // Reset all devices
    parentDialog->onResetAllDevices();
}

void LayerTestTab::onPositionTested(const GridPosition& pos)
{
    // Update the visualizer with the test color
    visualizer->setPositionColor(pos, parentDialog->getTestColor());
    visualizer->update();
}

} // namespace Lightscape