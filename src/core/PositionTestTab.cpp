#include "core/PositionTestTab.h"
#include "core/SetupTestDialog.h"
#include <QColorDialog>
#include <QCheckBox>

namespace Lightscape {

PositionTestTab::PositionTestTab(SetupTestDialog* parentDialog, DeviceManager* deviceManager, 
                                SpatialGrid* grid, QWidget* parent)
    : QWidget(parent)
    , parentDialog(parentDialog)
    , deviceManager(deviceManager)
    , spatialGrid(grid)
    , lastTestedPosition(GridPosition(-1, -1, -1))
    , multiSelectionMode(false)
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
    
    // Add info label about clicking grid
    QLabel* clickInfoLabel = new QLabel(
        "<b>Click on a position in the grid to select it, then press Test Position.</b><br>"
        "Only positions with device assignments (shown in orange) can be selected.",
        leftWidget);
    clickInfoLabel->setWordWrap(true);
    clickInfoLabel->setStyleSheet("padding: 5px; border-radius: 3px;");
    leftLayout->addWidget(clickInfoLabel);
    
    // Add grid visualizer
    QGroupBox* visGroup = new QGroupBox("Grid Preview", leftWidget);
    QVBoxLayout* visLayout = new QVBoxLayout(visGroup);
    
    visualizer = new TestGridVisualizer(spatialGrid, visGroup);
    visualizer->setMinimumHeight(250);
    visualizer->setInteractive(true); // Make it clickable
    connect(visualizer, &TestGridVisualizer::positionClicked, 
            this, &PositionTestTab::onGridPositionClicked);
    visLayout->addWidget(visualizer);
    
    // Add layer selector tabs
    QHBoxLayout* layerTabLayout = new QHBoxLayout();
    layerTabLayout->addWidget(new QLabel("Layers:"));
    
    // Get grid dimensions to know how many layers
    GridDimensions dims = spatialGrid->GetDimensions();
    for (int z = 0; z < dims.depth; z++) {
        QString layerName = spatialGrid->GetLayerLabel(z);
        if (layerName.isEmpty()) {
            layerName = QString("Layer %1").arg(z);
        }
        
        QPushButton* layerBtn = new QPushButton(layerName, visGroup);
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
        });
        
        layerTabLayout->addWidget(layerBtn);
    }
    
    layerTabLayout->addStretch();
    visLayout->addLayout(layerTabLayout);
    
    visGroup->setLayout(visLayout);
    leftLayout->addWidget(visGroup);
    leftLayout->addStretch();
    leftWidget->setLayout(leftLayout);
    
    // --- Right side - controls ---
    QWidget* rightWidget = new QWidget(splitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    
    // Add info text
    QGroupBox* infoGroup = new QGroupBox("Position Test", rightWidget);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
    
    infoLabel = new QLabel(
        "<b>What this test does:</b><br>"
        "This test allows you to verify that a specific position in your grid has devices "
        "correctly assigned to it. When you run the test, the device at the selected position "
        "will light up with the test color.", infoGroup);
    infoLabel->setWordWrap(true);
    infoLayout->addWidget(infoLabel);
    
    infoGroup->setLayout(infoLayout);
    rightLayout->addWidget(infoGroup);
    
    // Position selection info
    QGroupBox* posSelectGroup = new QGroupBox("Selected Position", rightWidget);
    QVBoxLayout* posSelectLayout = new QVBoxLayout(posSelectGroup);
    
    positionSelector = new QComboBox(posSelectGroup);
    positionSelector->setVisible(false); // Hide the selector but keep for compatibility
    
    QLabel* posLabel = new QLabel("No position selected", posSelectGroup);
    posLabel->setAlignment(Qt::AlignCenter);
    posLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    posSelectLayout->addWidget(posLabel);
    
    connect(this, &PositionTestTab::gridPositionClicked, [posLabel, this](const GridPosition& pos) {
        // Update label with selected position info
        QString layerName = spatialGrid->GetLayerLabel(pos.z);
        if (layerName.isEmpty()) {
            layerName = QString("Layer %1").arg(pos.z);
        }
        
        QString posName = spatialGrid->GetPositionLabel(pos);
        if (posName.isEmpty()) {
            posName = QString("Position (%1,%2,%3)").arg(pos.x).arg(pos.y).arg(pos.z);
        } else {
            posName = QString("%1 at (%2,%3,%4)").arg(posName).arg(pos.x).arg(pos.y).arg(pos.z);
        }
        
        posLabel->setText(QString("<b>%1</b><br>%2").arg(posName).arg(layerName));
    });
    
    posSelectLayout->addStretch();
    rightLayout->addWidget(posSelectGroup);
    
    // Color selection
    QGroupBox* colorGroup = new QGroupBox("Test Color", rightWidget);
    QVBoxLayout* colorLayout = new QVBoxLayout(colorGroup);
    
    QHBoxLayout* colorButtonLayout = new QHBoxLayout();
    QLabel* colorLabel = new QLabel("Choose color:", colorGroup);
    colorButtonLayout->addWidget(colorLabel);
    
    RGBColor testColor = parentDialog->getTestColor();
    colorButton = new QPushButton(colorGroup);
    colorButton->setFixedSize(60, 30);
    colorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                              .arg(RGBGetRValue(testColor))
                              .arg(RGBGetGValue(testColor))
                              .arg(RGBGetBValue(testColor)));
    connect(colorButton, &QPushButton::clicked, this, &PositionTestTab::onColorButtonClicked);
    colorButtonLayout->addWidget(colorButton);
    
    // Add quick color buttons
    QHBoxLayout* quickColorsLayout = new QHBoxLayout();
    quickColorsLayout->addWidget(new QLabel("Quick Colors:"));
    
    // Red button
    QPushButton* redBtn = new QPushButton(colorGroup);
    redBtn->setFixedSize(24, 24);
    redBtn->setStyleSheet("background-color: rgb(255, 0, 0); border-radius: 12px;");
    connect(redBtn, &QPushButton::clicked, [=]() {
        RGBColor color = ToRGBColor(255, 0, 0);
        parentDialog->setTestColor(color);
    });
    quickColorsLayout->addWidget(redBtn);
    
    // Green button
    QPushButton* greenBtn = new QPushButton(colorGroup);
    greenBtn->setFixedSize(24, 24);
    greenBtn->setStyleSheet("background-color: rgb(0, 255, 0); border-radius: 12px;");
    connect(greenBtn, &QPushButton::clicked, [=]() {
        RGBColor color = ToRGBColor(0, 255, 0);
        parentDialog->setTestColor(color);
    });
    quickColorsLayout->addWidget(greenBtn);
    
    // Blue button
    QPushButton* blueBtn = new QPushButton(colorGroup);
    blueBtn->setFixedSize(24, 24);
    blueBtn->setStyleSheet("background-color: rgb(0, 0, 255); border-radius: 12px;");
    connect(blueBtn, &QPushButton::clicked, [=]() {
        RGBColor color = ToRGBColor(0, 0, 255);
        parentDialog->setTestColor(color);
    });
    quickColorsLayout->addWidget(blueBtn);
    
    // White button
    QPushButton* whiteBtn = new QPushButton(colorGroup);
    whiteBtn->setFixedSize(24, 24);
    whiteBtn->setStyleSheet("background-color: rgb(255, 255, 255); border-radius: 12px; border: 1px solid #aaa;");
    connect(whiteBtn, &QPushButton::clicked, [=]() {
        RGBColor color = ToRGBColor(255, 255, 255);
        parentDialog->setTestColor(color);
    });
    quickColorsLayout->addWidget(whiteBtn);
    
    quickColorsLayout->addStretch();
    
    colorLayout->addLayout(colorButtonLayout);
    colorLayout->addLayout(quickColorsLayout);
    
    rightLayout->addWidget(colorGroup);
    
    // Actions
    QGroupBox* actionsGroup = new QGroupBox("Actions", rightWidget);
    QVBoxLayout* actionsLayout = new QVBoxLayout(actionsGroup);
    
    // Add multi-select checkbox
    multiSelectCheckbox = new QCheckBox("Multi-select mode", actionsGroup);
    multiSelectCheckbox->setToolTip("When checked, selecting new positions will not reset previous selections\nYou can also hold Ctrl key while clicking for temporary multi-select");
    connect(multiSelectCheckbox, &QCheckBox::toggled, [this](bool checked) {
        multiSelectionMode = checked;
    });
    actionsLayout->addWidget(multiSelectCheckbox);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    testButton = new QPushButton("Test Position", actionsGroup);
    testButton->setToolTip("Light up the selected position with the test color");
    testButton->setEnabled(false); // Disabled until a position is selected
    connect(testButton, &QPushButton::clicked, this, &PositionTestTab::onTestPositionClicked);
    buttonLayout->addWidget(testButton);
    
    resetButton = new QPushButton("Reset All Devices", actionsGroup);
    resetButton->setToolTip("Turn off all devices");
    connect(resetButton, &QPushButton::clicked, parentDialog, &SetupTestDialog::onResetAllDevices);
    buttonLayout->addWidget(resetButton);
    
    actionsLayout->addLayout(buttonLayout);
    
    rightLayout->addWidget(actionsGroup);
    rightLayout->addStretch();
    
    rightWidget->setLayout(rightLayout);
    
    // Add both sides to the splitter
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    
    // Set splitter proportions
    QList<int> splitSizes;
    splitSizes << 400 << 300;
    splitter->setSizes(splitSizes);
    
    // Initialize multi-select mode
    multiSelectCheckbox->setChecked(false);
    multiSelectionMode = false;
    
    // Reset the lastTestedPosition
    lastTestedPosition = GridPosition(-1, -1, -1);
    
    mainLayout->addWidget(splitter);
}

void PositionTestTab::populateSelector()
{
    // This is now implemented visually through the interactive grid
    // but we keep the method for compatibility
}

void PositionTestTab::updateColorButton(RGBColor color)
{
    // Update the color button appearance
    colorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                              .arg(RGBGetRValue(color))
                              .arg(RGBGetGValue(color))
                              .arg(RGBGetBValue(color)));
}

void PositionTestTab::setButtonsEnabled(bool enabled)
{
    // Only enable test button if a position is selected
    if (testButton) {
        testButton->setEnabled(enabled && visualizer && visualizer->getSelectedPosition().x >= 0);
    }
    if (resetButton) {
        resetButton->setEnabled(enabled);
    }
}

void PositionTestTab::onPositionSelectionChanged(int index)
{
    // No longer needed as we're using the grid directly
    Q_UNUSED(index);
}

void PositionTestTab::onGridPositionClicked(const GridPosition& pos)
{
    // Update UI state when a position is clicked in the grid
    
    // Enable the test button
    if (testButton) {
        testButton->setEnabled(true);
    }
    
    // Make sure the right layer is shown
    if (visualizer) {
        visualizer->setActiveLayer(pos.z);
    }
    
    // Multi-select mode check
    bool inMultiSelectMode = multiSelectionMode || (visualizer && visualizer->getMultiSelectMode());
    
    // Reset the previous position if not in multi-select mode and clicking a different position
    if (!inMultiSelectMode && lastTestedPosition.x >= 0 && !(pos == lastTestedPosition)) {
        // Turn off the device at the previous position
        if (spatialGrid->HasAssignments(lastTestedPosition)) {
            testUtility->resetPositionDevice(lastTestedPosition);
        }
    }
    
    // Let other parts know about the selection
    emit gridPositionClicked(pos);
}

void PositionTestTab::onColorButtonClicked()
{
    // Get current color
    RGBColor currentColor = parentDialog->getTestColor();
    
    // Show color dialog
    QColor initialColor(RGBGetRValue(currentColor), RGBGetGValue(currentColor), RGBGetBValue(currentColor));
    QColor newColor = QColorDialog::getColor(initialColor, this, "Select Test Color");
    
    if (newColor.isValid()) {
        // Update the color in parent dialog
        RGBColor rgbColor = ToRGBColor(newColor.red(), newColor.green(), newColor.blue());
        parentDialog->setTestColor(rgbColor);
    }
}

void PositionTestTab::onTestPositionClicked()
{
    // Get the selected position from the visualizer
    if (!visualizer) return;
    
    // Get all selected positions for multi-select mode
    QList<GridPosition> positionsToTest;
    bool inMultiSelectMode = multiSelectionMode || (visualizer && visualizer->getMultiSelectMode());
    
    if (inMultiSelectMode) {
        // Get all selected positions
        positionsToTest = visualizer->getSelectedPositions();
    } else {
        // Get just the current position
        GridPosition selectedPos = visualizer->getSelectedPosition();
        if (selectedPos.x >= 0 && selectedPos.y >= 0 && selectedPos.z >= 0) {
            positionsToTest.append(selectedPos);
        }
    }
    
    // If no positions to test, return
    if (positionsToTest.isEmpty()) return;
    
    // Clear visualizer if we're not in multi-select mode
    if (!inMultiSelectMode) {
        visualizer->clearColors();
    }
    
    // Store last tested position
    lastTestedPosition = positionsToTest.last();
    
    // Test all positions
    for (const GridPosition& pos : positionsToTest) {
        testUtility->testPosition(pos);
    }
}

void PositionTestTab::onPositionTested(const GridPosition& pos)
{
    // Update the visualizer with the test color
    if (visualizer) {
        visualizer->setPositionColor(pos, parentDialog->getTestColor());
    }
}

} // namespace Lightscape