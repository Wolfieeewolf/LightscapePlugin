#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSlider>
#include <QCheckBox>

#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "core/TestGridVisualizer.h"
#include "core/SetupTestUtility.h"

namespace Lightscape {

class SetupTestDialog; // Forward declaration

/**
 * @brief Tab widget for testing pattern effects
 */
class PatternTestTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Pattern Test Tab
     * 
     * @param parentDialog Parent setup test dialog
     * @param deviceManager Pointer to device manager
     * @param grid Pointer to spatial grid
     * @param parent Parent widget
     */
    explicit PatternTestTab(SetupTestDialog* parentDialog, DeviceManager* deviceManager, 
                           SpatialGrid* grid, QWidget* parent = nullptr);
    
    /**
     * @brief Get the pattern visualizer
     */
    TestGridVisualizer* getVisualizer() const { return visualizer; }
    
    /**
     * @brief Enable or disable test buttons
     * 
     * @param enabled Whether buttons should be enabled
     */
    void setButtonsEnabled(bool enabled);

public slots:
    /**
     * @brief Handle speed slider change
     * 
     * @param value New slider value
     */
    void onSpeedSliderChanged(int value);
    
    /**
     * @brief Handle test all positions button click
     */
    void onTestAllPositionsClicked();
    
    /**
     * @brief Handle test distance pattern button click
     */
    void onTestDistancePatternClicked();
    
    /**
     * @brief Handle test wave pattern button click
     */
    void onTestWavePatternClicked();
    
    /**
     * @brief Handle stop test button click
     */
    void onStopTestClicked();
    
    /**
     * @brief Handle reset devices button click
     */
    void onResetDevicesClicked();
    
    /**
     * @brief Handle position tested event
     * 
     * @param pos Position that was tested
     */
    void onPositionTested(const GridPosition& pos);

private:
    SetupTestDialog* parentDialog;
    DeviceManager* deviceManager;
    SpatialGrid* spatialGrid;
    SetupTestUtility* testUtility;
    
    // UI components
    TestGridVisualizer* visualizer;
    QSlider* speedSlider;
    QLabel* speedValueLabel;
    QCheckBox* continuousUpdateCheck;
    QPushButton* testAllButton;
    QPushButton* testDistanceButton;
    QPushButton* testWaveButton;
    QPushButton* stopTestButton;
    QPushButton* resetButton;
    QLabel* infoLabel;
};

} // namespace Lightscape