#pragma once

#include <QObject>
#include <QTimer>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"

namespace Lightscape {

/**
 * @brief Utility class for testing grid positions and patterns
 * 
 * This class provides testing functionality for the Setup & Test dialog,
 * handling pattern generation, color calculations, and device control.
 */
class SetupTestUtility : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Setup Test Utility
     * 
     * @param deviceManager Pointer to the device manager
     * @param grid Pointer to the spatial grid
     * @param parent Parent QObject
     */
    explicit SetupTestUtility(DeviceManager* deviceManager, SpatialGrid* grid, QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~SetupTestUtility() = default;
    
    /**
     * @brief Set the test color for testing operations
     * 
     * @param color Color to use for tests
     */
    void setTestColor(RGBColor color) { testColor = color; }
    
    /**
     * @brief Get the current test color
     * 
     * @return RGBColor Current test color
     */
    RGBColor getTestColor() const { return testColor; }
    
    /**
     * @brief Test a single position in the grid
     * 
     * @param pos Grid position to test
     */
    void testPosition(const GridPosition& pos);
    
    /**
     * @brief Test positions in a specific layer
     * 
     * @param layerIndex Layer to test
     * @param delay Delay between position tests in milliseconds
     * @return QList<GridPosition> List of positions being tested
     */
    QList<GridPosition> testLayer(int layerIndex, int delay);
    
    /**
     * @brief Test all assigned positions
     * 
     * @return QList<GridPosition> List of positions being tested
     */
    QList<GridPosition> testAllPositions();
    
    /**
     * @brief Test distance-based pattern
     * 
     * @param continuous Whether to continuously update the pattern
     */
    void testDistancePattern(bool continuous);
    
    /**
     * @brief Test wave pattern
     * 
     * @param continuous Whether to continuously update the pattern
     */
    void testWavePattern(bool continuous);
    
    /**
     * @brief Update the active pattern
     * 
     * @param speedFactor Speed factor for pattern updates
     */
    void updatePattern(float speedFactor);
    
    /**
     * @brief Stop active pattern
     */
    void stopPattern();
    
    /**
     * @brief Stop all active tests
     */
    void stopAllTests();
    
    /**
     * @brief Reset all devices to their default state
     */
    void resetAllDevices();
    
    /**
     * @brief Reset a specific position's device to its default state
     * 
     * @param pos Grid position containing the device to reset
     */
    void resetPositionDevice(const GridPosition& pos);
    
    /**
     * @brief Get a list of all assigned positions in the grid
     * 
     * @return QList<GridPosition> List of assigned positions
     */
    QList<GridPosition> getAssignedPositions() const;
    
    /**
     * @brief Get the active pattern type
     * 
     * @return int Pattern type (0 = none, 1 = distance, 2 = wave)
     */
    int getActivePattern() const { return activePattern; }
    
    /**
     * @brief Is a pattern currently active
     * 
     * @return bool True if pattern is active
     */
    bool isPatternActive() const { return patternTimer && patternTimer->isActive(); }
    
    /**
     * @brief Is any test currently in progress
     * 
     * @return bool True if a test is in progress
     */
    bool isTestInProgress() const { return testInProgress; }

signals:
    /**
     * @brief Signal emitted when a test starts
     */
    void testStarted();
    
    /**
     * @brief Signal emitted when a test completes
     */
    void testCompleted();
    
    /**
     * @brief Signal emitted when a position is tested
     */
    void positionTested(const GridPosition& pos);

private:
    /**
     * @brief Apply a test color to a specific position
     * 
     * @param pos Grid position
     * @param color Color to apply
     */
    void applyColorToPosition(const GridPosition& pos, const RGBColor& color);
    
    /**
     * @brief Calculate distance between two positions
     * 
     * @param pos1 First position
     * @param pos2 Second position
     * @return float Distance
     */
    float calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const;
    
    /**
     * @brief Create a color based on distance
     * 
     * @param distance Distance value
     * @param maxDistance Maximum distance for normalization
     * @return RGBColor Color based on distance
     */
    RGBColor createDistanceColor(float distance, float maxDistance) const;
    
    /**
     * @brief Create a wave pattern color
     * 
     * @param distance Distance from reference
     * @param time Current time value
     * @return RGBColor Color for the wave effect
     */
    RGBColor createWaveColor(float distance, float time) const;

    // Core resources
    DeviceManager* deviceManager;
    SpatialGrid* spatialGrid;
    
    // Pattern state
    QTimer* patternTimer;
    float currentTime;
    int activePattern; // 0 = none, 1 = distance, 2 = wave
    RGBColor testColor;
    
    // Test state
    bool testInProgress;
    QList<GridPosition> currentTestPositions;
};

} // namespace Lightscape