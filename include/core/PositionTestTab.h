#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QCheckBox>

#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "core/TestGridVisualizer.h"
#include "core/SetupTestUtility.h"

namespace Lightscape {

class SetupTestDialog; // Forward declaration

/**
 * @brief Tab widget for testing individual grid positions
 */
class PositionTestTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Position Test Tab
     * 
     * @param parentDialog Parent setup test dialog
     * @param deviceManager Pointer to device manager
     * @param grid Pointer to spatial grid
     * @param parent Parent widget
     */
    explicit PositionTestTab(SetupTestDialog* parentDialog, DeviceManager* deviceManager, 
                            SpatialGrid* grid, QWidget* parent = nullptr);
    
    /**
     * @brief Get the position visualizer
     */
    TestGridVisualizer* getVisualizer() const { return visualizer; }
    
    /**
     * @brief Populate the position selector dropdown
     */
    void populateSelector();
    
    /**
     * @brief Update the color button appearance
     * 
     * @param color The new color
     */
    void updateColorButton(RGBColor color);
    
    /**
     * @brief Enable or disable test buttons
     * 
     * @param enabled Whether buttons should be enabled
     */
    void setButtonsEnabled(bool enabled);

signals:
    /**
     * @brief Signal emitted when a grid position is clicked
     * 
     * @param pos The clicked position
     */
    void gridPositionClicked(const GridPosition& pos);

public slots:
    /**
     * @brief Handle position selection change
     * 
     * @param index New selected index
     */
    void onPositionSelectionChanged(int index);
    
    /**
     * @brief Handle color button click
     */
    void onColorButtonClicked();
    
    /**
     * @brief Handle test position button click
     */
    void onTestPositionClicked();
    
    /**
     * @brief Handle position tested event
     * 
     * @param pos Position that was tested
     */
    void onPositionTested(const GridPosition& pos);
    
    /**
     * @brief Handle position clicked in grid
     * 
     * @param pos The clicked position
     */
    void onGridPositionClicked(const GridPosition& pos);

private:
    SetupTestDialog* parentDialog;
    DeviceManager* deviceManager;
    SpatialGrid* spatialGrid;
    SetupTestUtility* testUtility;
    
    // Previously tested position
    GridPosition lastTestedPosition;
    bool multiSelectionMode;
    
    // UI components
    TestGridVisualizer* visualizer;
    QComboBox* positionSelector;
    QPushButton* colorButton;
    QPushButton* testButton;
    QPushButton* resetButton;
    QLabel* infoLabel;
    QCheckBox* multiSelectCheckbox;
};

} // namespace Lightscape