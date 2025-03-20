#pragma once

#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "core/SetupTestUtility.h"
#include "core/GridValidator.h"
#include "core/TestGridVisualizer.h"

// Forward declarations for tab widgets
class PositionTestTab;
class LayerTestTab;
class PatternTestTab;
class ValidationTab;

namespace Lightscape {

/**
 * @brief Dialog for setting up and testing the spatial grid configuration
 * 
 * This dialog provides tools for validating grid positions, testing device assignments,
 * and verifying that the spatial effects system is working correctly.
 */
class SetupTestDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Setup Test Dialog
     * 
     * @param deviceManager Pointer to the device manager
     * @param grid Pointer to the spatial grid
     * @param parent Parent widget
     */
    explicit SetupTestDialog(DeviceManager* deviceManager, SpatialGrid* grid, QWidget* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~SetupTestDialog();
    
    /**
     * @brief Get the test utility instance
     * 
     * @return Pointer to the SetupTestUtility
     */
    SetupTestUtility* getTestUtility() const { return testUtility; }
    
    /**
     * @brief Get the test color
     * 
     * @return Current test color
     */
    RGBColor getTestColor() const { return testColor; }
    
    /**
     * @brief Set the test color
     * 
     * @param color New test color
     */
    void setTestColor(RGBColor color);

public slots:
    /**
     * @brief Stop all running tests
     */
    void stopAllTests();
    
    /**
     * @brief Reset all devices to their default state
     */
    void onResetAllDevices();
    
    /**
     * @brief Handle position tested event
     */
    void onPositionTested(const GridPosition& pos);

private slots:
    /**
     * @brief Handle tab changed event
     * 
     * @param index New tab index
     */
    void onTabChanged(int index);

private:
    /**
     * @brief Set up the UI components
     */
    void setupUI();
    
    /**
     * @brief Create the header with coordinate explanation
     * 
     * @return Widget containing the header
     */
    QWidget* createHeaderWidget();

    // UI Components
    QTabWidget* tabWidget;
    QPushButton* closeButton;
    
    // Tab widgets (now separate classes)
    QWidget* positionTestTab;
    QWidget* layerTestTab;
    QWidget* patternTestTab;
    QWidget* validationTab;
    
    // Utility classes
    SetupTestUtility* testUtility;
    GridValidator* gridValidator;
    
    // Common resources
    DeviceManager* deviceManager;
    SpatialGrid* spatialGrid;
    RGBColor testColor;
};

} // namespace Lightscape