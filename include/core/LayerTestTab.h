#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSlider>

#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "core/TestGridVisualizer.h"
#include "core/SetupTestUtility.h"

namespace Lightscape {

class SetupTestDialog; // Forward declaration

/**
 * @brief Tab widget for testing entire grid layers
 */
class LayerTestTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Layer Test Tab
     * 
     * @param parentDialog Parent setup test dialog
     * @param deviceManager Pointer to device manager
     * @param grid Pointer to spatial grid
     * @param parent Parent widget
     */
    explicit LayerTestTab(SetupTestDialog* parentDialog, DeviceManager* deviceManager, 
                         SpatialGrid* grid, QWidget* parent = nullptr);
    
    /**
     * @brief Get the layer visualizer
     */
    TestGridVisualizer* getVisualizer() const { return visualizer; }
    
    /**
     * @brief Get the layer selector combobox
     */
    QComboBox* getLayerSelector() const { return layerSelector; }
    
    /**
     * @brief Populate the layer selector dropdown
     */
    void populateSelector();
    
    /**
     * @brief Enable or disable test buttons
     * 
     * @param enabled Whether buttons should be enabled
     */
    void setButtonsEnabled(bool enabled);

public slots:
    /**
     * @brief Handle layer selection change
     * 
     * @param index New selected index
     */
    void onLayerSelectionChanged(int index);
    
    /**
     * @brief Handle delay slider change
     * 
     * @param value New slider value
     */
    void onDelaySliderChanged(int value);
    
    /**
     * @brief Handle test layer button click
     */
    void onTestLayerClicked();
    
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
    QComboBox* layerSelector;
    QSlider* delaySlider;
    QLabel* delayValueLabel;
    QPushButton* testLayerButton;
    QPushButton* stopTestButton;
    QPushButton* resetButton;
    QLabel* infoLabel;
};

} // namespace Lightscape