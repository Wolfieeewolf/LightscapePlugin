#include "core/PatternTestTab.h"
#include "core/SetupTestDialog.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QButtonGroup>
#include <QComboBox>

namespace Lightscape {

PatternTestTab::PatternTestTab(SetupTestDialog* parentDialog, DeviceManager* deviceManager, 
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
    QLabel* patternInfoLabel = new QLabel(
        "<b>The patterns below simulate spatial effects that will flow through your devices.</b><br>"
        "These tests require a reference point (user position) to be set in the grid.",
        leftWidget);
    patternInfoLabel->setWordWrap(true);
    patternInfoLabel->setStyleSheet("padding: 5px; border-radius: 3px;");
    leftLayout->addWidget(patternInfoLabel);
    
    // Add grid visualizer - consistent with other tabs
    QGroupBox* visGroup = new QGroupBox("Pattern Preview", leftWidget);
    QVBoxLayout* visLayout = new QVBoxLayout(visGroup);
    
    visualizer = new TestGridVisualizer(spatialGrid, visGroup);
    visualizer->setMinimumHeight(250);
    visualizer->toggleCoordinateDisplay(true);
    visLayout->addWidget(visualizer);
    
    // Add layer selection
    QGroupBox* layerGroup = new QGroupBox("Layer Selection", visGroup);
    QHBoxLayout* layerButtonLayout = new QHBoxLayout(layerGroup);
    layerButtonLayout->addWidget(new QLabel("View Layer:"));
    
    // Get grid dimensions to know how many layers
    GridDimensions dims = spatialGrid->GetDimensions();
    
    // Create a button group for the layer buttons
    QButtonGroup* layerBtnGroup = new QButtonGroup(this);
    
    for (int z = 0; z < dims.depth; z++) {
        QString layerName = spatialGrid->GetLayerLabel(z);
        if (layerName.isEmpty()) {
            layerName = QString("Layer %1").arg(z);
        }
        
        QPushButton* layerBtn = new QPushButton(layerName, layerGroup);
        layerBtn->setCheckable(true);
        layerBtn->setProperty("layer", z);
        layerBtnGroup->addButton(layerBtn);
        
        if (z == 0) {
            layerBtn->setChecked(true);
        }
        
        connect(layerBtn, &QPushButton::clicked, [this, layerBtnGroup, z]() {
            // Update all buttons in the group
            QList<QAbstractButton*> buttons = layerBtnGroup->buttons();
            for (auto btn : buttons) {
                if (btn->property("layer").toInt() == z) {
                    btn->setChecked(true);
                } else {
                    btn->setChecked(false);
                }
            }
            
            // Set active layer in visualizer
            visualizer->setActiveLayer(z);
            visualizer->update();
        });
        
        layerButtonLayout->addWidget(layerBtn);
    }
    
    layerButtonLayout->addStretch();
    layerGroup->setLayout(layerButtonLayout);
    visLayout->addWidget(layerGroup);
    
    visGroup->setLayout(visLayout);
    leftLayout->addWidget(visGroup);
    
    // --- Reference point display ---
    QGroupBox* refPointGroup = new QGroupBox("Reference Point", leftWidget);
    QVBoxLayout* refPointLayout = new QVBoxLayout(refPointGroup);
    
    QLabel* referencePointStatus = new QLabel(refPointGroup);
    
    // Update label based on current reference point status
    if (spatialGrid->HasUserPosition()) {
        GridPosition pos = spatialGrid->GetUserPosition().value();
        QString layerName = spatialGrid->GetLayerLabel(pos.z);
        if (layerName.isEmpty()) {
            layerName = QString("Layer %1").arg(pos.z);
        }
        
        QString posLabel = spatialGrid->GetPositionLabel(pos);
        if (posLabel.isEmpty()) {
            posLabel = QString("Position (%1,%2,%3)").arg(pos.x).arg(pos.y).arg(pos.z);
        } else {
            posLabel = QString("%1 at (%2,%3,%4)").arg(posLabel).arg(pos.x).arg(pos.y).arg(pos.z);
        }
        
        referencePointStatus->setText(QString(
            "<b>Reference point is set to:</b><br>%1 in %2<br>"
            "<span style='color: green;'>✓ Ready for pattern tests</span>"
        ).arg(posLabel).arg(layerName));
    } else {
        referencePointStatus->setText(
            "<b>No reference point is set!</b><br>"
            "<span style='color: red;'>⚠ Set a user position in the grid panel first</span>"
        );
    }
    
    refPointLayout->addWidget(referencePointStatus);
    refPointGroup->setLayout(refPointLayout);
    leftLayout->addWidget(refPointGroup);
    
    leftLayout->addStretch();
    leftWidget->setLayout(leftLayout);
    
    // --- Right side - controls ---
    QWidget* rightWidget = new QWidget(splitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    
    // Basic pattern controls
    QGroupBox* basicPatternGroup = new QGroupBox("Basic Patterns", rightWidget);
    QVBoxLayout* basicLayout = new QVBoxLayout(basicPatternGroup);
    
    // Basic patterns explanation
    QLabel* basicPatternsInfo = new QLabel(
        "<b>Available Patterns:</b><br>"
        "• Test All Positions: Lights up all devices at once<br>"
        "• Test Distance Pattern: Brightness decreases with distance from reference<br>"
        "• Test Wave Pattern: Creates waves that ripple out from the reference point",
        basicPatternGroup);
    basicPatternsInfo->setWordWrap(true);
    basicLayout->addWidget(basicPatternsInfo);
    
    // Color selection - same as other tabs
    QGroupBox* colorGroup = new QGroupBox("Test Color", basicPatternGroup);
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
        // Show color dialog
        RGBColor currentColor = parentDialog->getTestColor();
        QColor initialColor(RGBGetRValue(currentColor), RGBGetGValue(currentColor), RGBGetBValue(currentColor));
        QColor newColor = QColorDialog::getColor(initialColor, this, "Select Test Color");
        
        if (newColor.isValid()) {
            // Update the color
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
    
    colorGroup->setLayout(colorLayout);
    basicLayout->addWidget(colorGroup);
    
    // Pattern settings
    QGroupBox* settingsGroup = new QGroupBox("Pattern Settings", basicPatternGroup);
    QVBoxLayout* settingsLayout = new QVBoxLayout(settingsGroup);
    
    QLabel* speedLabel = new QLabel("Effect Speed:", settingsGroup);
    settingsLayout->addWidget(speedLabel);
    
    QHBoxLayout* speedSliderLayout = new QHBoxLayout();
    speedSlider = new QSlider(Qt::Horizontal, settingsGroup);
    speedSlider->setRange(1, 100);
    speedSlider->setValue(50);
    speedSlider->setTickInterval(10);
    speedSlider->setTickPosition(QSlider::TicksBelow);
    connect(speedSlider, &QSlider::valueChanged, this, &PatternTestTab::onSpeedSliderChanged);
    speedSliderLayout->addWidget(speedSlider);
    
    speedValueLabel = new QLabel("50", settingsGroup);
    speedValueLabel->setMinimumWidth(40);
    speedValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    speedSliderLayout->addWidget(speedValueLabel);
    
    settingsLayout->addLayout(speedSliderLayout);
    
    // Continuous update checkbox
    continuousUpdateCheck = new QCheckBox("Continuous Update", settingsGroup);
    continuousUpdateCheck->setToolTip("When checked, the pattern will continuously update");
    continuousUpdateCheck->setChecked(true);
    settingsLayout->addWidget(continuousUpdateCheck);
    
    settingsGroup->setLayout(settingsLayout);
    basicLayout->addWidget(settingsGroup);
    
    // Actions
    QGroupBox* actionsGroup = new QGroupBox("Actions", basicPatternGroup);
    QVBoxLayout* actionsLayout = new QVBoxLayout(actionsGroup);
    
    QHBoxLayout* buttonLayout1 = new QHBoxLayout();
    testAllButton = new QPushButton("Test All Positions", actionsGroup);
    connect(testAllButton, &QPushButton::clicked, this, &PatternTestTab::onTestAllPositionsClicked);
    buttonLayout1->addWidget(testAllButton);
    
    stopTestButton = new QPushButton("Stop Test", actionsGroup);
    connect(stopTestButton, &QPushButton::clicked, this, &PatternTestTab::onStopTestClicked);
    buttonLayout1->addWidget(stopTestButton);
    actionsLayout->addLayout(buttonLayout1);
    
    QHBoxLayout* buttonLayout2 = new QHBoxLayout();
    testDistanceButton = new QPushButton("Test Distance Pattern", actionsGroup);
    testDistanceButton->setToolTip("Try a distance-based brightness effect");
    connect(testDistanceButton, &QPushButton::clicked, this, &PatternTestTab::onTestDistancePatternClicked);
    buttonLayout2->addWidget(testDistanceButton);
    
    testWaveButton = new QPushButton("Test Wave Pattern", actionsGroup);
    testWaveButton->setToolTip("Try a wave effect that ripples from the reference point");
    connect(testWaveButton, &QPushButton::clicked, this, &PatternTestTab::onTestWavePatternClicked);
    buttonLayout2->addWidget(testWaveButton);
    actionsLayout->addLayout(buttonLayout2);
    
    resetButton = new QPushButton("Reset All Devices", actionsGroup);
    resetButton->setToolTip("Turn off all devices");
    connect(resetButton, &QPushButton::clicked, this, &PatternTestTab::onResetDevicesClicked);
    actionsLayout->addWidget(resetButton);
    
    actionsGroup->setLayout(actionsLayout);
    basicLayout->addWidget(actionsGroup);
    
    basicPatternGroup->setLayout(basicLayout);
    rightLayout->addWidget(basicPatternGroup);
    
    rightWidget->setLayout(rightLayout);
    
    // Add both sides to the splitter
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    
    // Set splitter proportions (same as other tabs)
    QList<int> splitSizes;
    splitSizes << 400 << 300;
    splitter->setSizes(splitSizes);
    
    mainLayout->addWidget(splitter);
    
    // Set initial state
    setButtonsEnabled(true);
    
    // Set active layer to be the one with the user position if available
    if (spatialGrid->HasUserPosition()) {
        auto userPos = spatialGrid->GetUserPosition();
        if (userPos.has_value()) {
            visualizer->setActiveLayer(userPos.value().z);
        }
    } else {
        visualizer->setActiveLayer(0);
    }
    
    // Force update of visualization
    visualizer->update();
}

void PatternTestTab::setButtonsEnabled(bool enabled)
{
    // Get number of positions with assignments
    QList<GridPosition> positions = testUtility->getAssignedPositions();
    bool hasPositions = !positions.isEmpty();
    
    testAllButton->setEnabled(enabled && hasPositions);
    testDistanceButton->setEnabled(enabled && hasPositions && spatialGrid->HasUserPosition());
    testWaveButton->setEnabled(enabled && hasPositions && spatialGrid->HasUserPosition());
    stopTestButton->setEnabled(enabled);
    resetButton->setEnabled(enabled);
}

void PatternTestTab::onSpeedSliderChanged(int value)
{
    speedValueLabel->setText(QString::number(value));
}

void PatternTestTab::onTestAllPositionsClicked()
{
    // Clear visualizer
    visualizer->clearColors();
    
    // Test all positions
    QList<GridPosition> positions = testUtility->testAllPositions();
    
    if (positions.isEmpty()) {
        QMessageBox::information(this, "No Assignments", 
                               "There are no device assignments in the grid.");
    }
}

void PatternTestTab::onTestDistancePatternClicked()
{
    // Check if the grid has a user position defined
    if (!spatialGrid->HasUserPosition()) {
        QMessageBox::warning(this, "No Reference Point", 
                           "Please set a user position in the grid first.");
        return;
    }
    
    // Clear visualizer
    visualizer->clearColors();
    
    // Set the speed factor from slider
    float speedFactor = speedSlider->value() / 50.0f;  // 50 = 1.0 (normal speed)
    testUtility->updatePattern(speedFactor);
    
    // Start distance pattern test
    testUtility->testDistancePattern(continuousUpdateCheck->isChecked());
}

void PatternTestTab::onTestWavePatternClicked()
{
    // Check if the grid has a user position defined
    if (!spatialGrid->HasUserPosition()) {
        QMessageBox::warning(this, "No Reference Point", 
                           "Please set a user position in the grid first.");
        return;
    }
    
    // Clear visualizer
    visualizer->clearColors();
    
    // Set the speed factor from slider
    float speedFactor = speedSlider->value() / 50.0f;  // 50 = 1.0 (normal speed)
    testUtility->updatePattern(speedFactor);
    
    // Start wave pattern test
    testUtility->testWavePattern(continuousUpdateCheck->isChecked());
}

void PatternTestTab::onStopTestClicked()
{
    // Stop any running tests
    parentDialog->stopAllTests();
}

void PatternTestTab::onResetDevicesClicked()
{
    // Reset all devices
    parentDialog->onResetAllDevices();
}

void PatternTestTab::onPositionTested(const GridPosition& pos)
{
    // For pattern tests, get the color directly from the assignment
    if (spatialGrid->HasAssignments(pos)) {
        QList<DeviceAssignment> assignments = spatialGrid->GetAssignments(pos);
        if (!assignments.isEmpty()) {
            // Use the color from the first assignment (all assignments at a position
            // should have the same color after a test)
            RGBColor color = assignments.first().color;
            visualizer->setPositionColor(pos, color);
        }
    } else {
        // If there's no assignment, use the test color
        visualizer->setPositionColor(pos, parentDialog->getTestColor());
    }
    
    // Ensure visualizer updates
    visualizer->update();
}

} // namespace Lightscape