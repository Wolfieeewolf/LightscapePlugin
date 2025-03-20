#include "core/SetupTestUtility.h"
#include <QMessageBox>
#include <cmath>

using namespace Lightscape;

SetupTestUtility::SetupTestUtility(DeviceManager* deviceManager, SpatialGrid* grid, QObject* parent)
    : QObject(parent)
    , deviceManager(deviceManager)
    , spatialGrid(grid)
    , currentTime(0.0f)
    , activePattern(0)
    , testColor(ToRGBColor(255, 0, 0)) // Default to red
    , testInProgress(false)
{
    // Initialize the pattern timer
    patternTimer = new QTimer(this);
    connect(patternTimer, &QTimer::timeout, [this]() {
        this->updatePattern(1.0f);
    });
    patternTimer->setInterval(33); // ~30fps
}

void SetupTestUtility::testPosition(const GridPosition& pos)
{
    if (testInProgress) {
        stopAllTests();
    }
    
    testInProgress = true;
    emit testStarted();
    
    applyColorToPosition(pos, testColor);
    emit positionTested(pos);
    
    testInProgress = false;
    emit testCompleted();
}

QList<GridPosition> SetupTestUtility::testLayer(int layerIndex, int delay)
{
    if (testInProgress) {
        stopAllTests();
    }
    
    GridDimensions dims = spatialGrid->GetDimensions();
    
    // Get positions in this layer that have assignments
    QList<GridPosition> layerPositions;
    for (int y = 0; y < dims.height; y++) {
        for (int x = 0; x < dims.width; x++) {
            GridPosition pos(x, y, layerIndex);
            if (spatialGrid->HasAssignments(pos)) {
                layerPositions.append(pos);
            }
        }
    }
    
    // Start the test sequence with a timer
    if (!layerPositions.isEmpty()) {
        testInProgress = true;
        emit testStarted();
        
        // Save the positions for potential cancellation
        currentTestPositions = layerPositions;
        
        // Schedule each position test with a delay
        for (int i = 0; i < layerPositions.size(); i++) {
            QTimer::singleShot(i * delay, this, [this, i]() {
                // Check if test was cancelled
                if (!testInProgress) {
                    return;
                }
                
                if (i < currentTestPositions.size()) {
                    // Apply color to this position
                    applyColorToPosition(currentTestPositions[i], testColor);
                    emit positionTested(currentTestPositions[i]);
                    
                    // If this is the last position, complete the test
                    if (i == currentTestPositions.size() - 1) {
                        QTimer::singleShot(500, this, [this]() {
                            testInProgress = false;
                            emit testCompleted();
                        });
                    }
                }
            });
        }
    }
    
    return layerPositions;
}

QList<GridPosition> SetupTestUtility::testAllPositions()
{
    // Stop any ongoing pattern test
    stopAllTests();
    
    QList<GridPosition> positions = getAssignedPositions();
    
    // Start a new test
    testInProgress = true;
    emit testStarted();
    
    // Apply test color to all positions
    for (const GridPosition& pos : positions) {
        applyColorToPosition(pos, testColor);
        emit positionTested(pos);
    }
    
    testInProgress = false;
    emit testCompleted();
    
    return positions;
}

void SetupTestUtility::testDistancePattern(bool continuous)
{
    // Stop any ongoing pattern
    stopAllTests();
    
    // Check if grid has a user position
    if (!spatialGrid->HasUserPosition()) {
        QMessageBox::warning(nullptr, "No Reference Point", 
                           "Please set a user position in the grid first.");
        return;
    }
    
    // Start a new test
    testInProgress = true;
    emit testStarted();
    
    // If continuous update is enabled, start the timer
    if (continuous) {
        // Set the current pattern and start the timer
        activePattern = 1; // Distance pattern
        currentTime = 0.0f;
        patternTimer->start();
    } else {
        // Just do a one-time update
        QList<GridPosition> positions = getAssignedPositions();
        GridPosition refPoint = spatialGrid->GetUserPosition().value();
        
        // Calculate max distance for normalization
        float maxDistance = 0.0f;
        for (const GridPosition& pos : positions) {
            float distance = calculateDistance(pos, refPoint);
            maxDistance = std::max(maxDistance, distance);
        }
        
        for (const GridPosition& pos : positions) {
            float distance = calculateDistance(pos, refPoint);
            RGBColor color = createDistanceColor(distance, maxDistance);
            applyColorToPosition(pos, color);
        }
        
        testInProgress = false;
        emit testCompleted();
    }
}

void SetupTestUtility::testWavePattern(bool continuous)
{
    // Stop any ongoing pattern
    stopAllTests();
    
    // Check if grid has a user position
    if (!spatialGrid->HasUserPosition()) {
        QMessageBox::warning(nullptr, "No Reference Point", 
                           "Please set a user position in the grid first.");
        return;
    }
    
    // Start a new test
    testInProgress = true;
    emit testStarted();
    
    // If continuous update is enabled, start the timer
    if (continuous) {
        // Set the current pattern and start the timer
        activePattern = 2; // Wave pattern
        currentTime = 0.0f;
        patternTimer->start();
    } else {
        // Just do a one-time update
        QList<GridPosition> positions = getAssignedPositions();
        GridPosition refPoint = spatialGrid->GetUserPosition().value();
        
        for (const GridPosition& pos : positions) {
            float distance = calculateDistance(pos, refPoint);
            RGBColor color = createWaveColor(distance, currentTime);
            applyColorToPosition(pos, color);
        }
        
        testInProgress = false;
        emit testCompleted();
    }
}

void SetupTestUtility::updatePattern(float speedFactor)
{
    // Increment time
    currentTime += 0.033f * speedFactor; // ~30fps with speed adjustment
    
    // Get reference point
    if (!spatialGrid->HasUserPosition()) {
        // Stop timer if user position is no longer valid
        stopPattern();
        return;
    }
    
    GridPosition refPoint = spatialGrid->GetUserPosition().value();
    
    // Get all positions with assignments
    QList<GridPosition> positions = getAssignedPositions();
    if (positions.isEmpty()) {
        // Stop timer if no positions are assigned
        stopPattern();
        return;
    }
    
    // Apply appropriate pattern
    switch (activePattern) {
        case 1: { // Distance pattern
            // Calculate max distance for normalization
            float maxDistance = 0.0f;
            for (const GridPosition& pos : positions) {
                float distance = calculateDistance(pos, refPoint);
                maxDistance = std::max(maxDistance, distance);
            }
            
            // Apply colors based on distance
            for (const GridPosition& pos : positions) {
                float distance = calculateDistance(pos, refPoint);
                RGBColor color = createDistanceColor(distance, maxDistance);
                applyColorToPosition(pos, color);
            }
            break;
        }
        
        case 2: { // Wave pattern
            for (const GridPosition& pos : positions) {
                float distance = calculateDistance(pos, refPoint);
                RGBColor color = createWaveColor(distance, currentTime);
                applyColorToPosition(pos, color);
            }
            break;
        }
    }
}

void SetupTestUtility::stopPattern()
{
    if (patternTimer->isActive()) {
        patternTimer->stop();
        activePattern = 0;
        testInProgress = false;
        emit testCompleted();
    }
}

void SetupTestUtility::stopAllTests()
{
    // Stop any active pattern test
    stopPattern();
    
    // Cancel any in-progress single tests
    testInProgress = false;
    currentTestPositions.clear();
    emit testCompleted();
}

void SetupTestUtility::resetAllDevices()
{
    // Stop any active tests
    stopAllTests();
    
    // Reset all devices to their default state
    if (deviceManager) {
        // Reset all RGB devices to black
        for (unsigned int i = 0; i < deviceManager->GetRGBDeviceCount(); i++) {
            deviceManager->SetDeviceColor(i, ToRGBColor(0, 0, 0));
            deviceManager->UpdateDevice(i);
        }
    }
}

void SetupTestUtility::resetPositionDevice(const GridPosition& pos)
{
    if (!spatialGrid || !deviceManager) {
        return;
    }
    
    if (!spatialGrid->HasAssignments(pos)) {
        return;
    }
    
    QList<DeviceAssignment> assignments = spatialGrid->GetAssignments(pos);
    
    for (const DeviceAssignment& assignment : assignments) {
        // Reset the assignment's color to black (off)
        spatialGrid->UpdateAssignmentColor(pos, assignments.indexOf(assignment), ToRGBColor(0, 0, 0));
        
        // Apply the color to the actual device
        if (deviceManager) {
            // Apply color based on assignment type
            if (assignment.zone_index >= 0) {
                deviceManager->SetZoneColor(assignment.device_index, assignment.zone_index, ToRGBColor(0, 0, 0));
                deviceManager->UpdateDevice(assignment.device_index);
            } else if (assignment.led_index >= 0) {
                deviceManager->SetLEDColor(assignment.device_index, assignment.led_index, ToRGBColor(0, 0, 0));
                deviceManager->UpdateDevice(assignment.device_index);
            } else {
                deviceManager->SetDeviceColor(assignment.device_index, ToRGBColor(0, 0, 0));
                deviceManager->UpdateDevice(assignment.device_index);
            }
        }
    }
}

QList<GridPosition> SetupTestUtility::getAssignedPositions() const
{
    QList<GridPosition> result;
    GridDimensions dims = spatialGrid->GetDimensions();
    
    // Iterate through all positions in the grid
    for (int z = 0; z < dims.depth; z++) {
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                if (spatialGrid->HasAssignments(pos)) {
                    result.append(pos);
                }
            }
        }
    }
    
    return result;
}

void SetupTestUtility::applyColorToPosition(const GridPosition& pos, const RGBColor& color)
{
    if (!spatialGrid->HasAssignments(pos)) {
        return;
    }
    
    QList<DeviceAssignment> assignments = spatialGrid->GetAssignments(pos);
    
    for (const DeviceAssignment& assignment : assignments) {
        // Update the assignment's color (for visual feedback in the UI)
        spatialGrid->UpdateAssignmentColor(pos, assignments.indexOf(assignment), color);
        
        // Apply the color to the actual device
        if (deviceManager) {
            // Apply color based on assignment type
            if (assignment.zone_index >= 0) {
                deviceManager->SetZoneColor(assignment.device_index, assignment.zone_index, color);
                deviceManager->UpdateDevice(assignment.device_index);
            } else if (assignment.led_index >= 0) {
                deviceManager->SetLEDColor(assignment.device_index, assignment.led_index, color);
                deviceManager->UpdateDevice(assignment.device_index);
            } else {
                deviceManager->SetDeviceColor(assignment.device_index, color);
                deviceManager->UpdateDevice(assignment.device_index);
            }
        }
    }
    
    emit positionTested(pos);
}

float SetupTestUtility::calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const
{
    // Calculate Euclidean distance between two grid positions
    float dx = static_cast<float>(pos1.x - pos2.x);
    float dy = static_cast<float>(pos1.y - pos2.y);
    float dz = static_cast<float>(pos1.z - pos2.z);
    
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

RGBColor SetupTestUtility::createDistanceColor(float distance, float maxDistance) const
{
    // Map distance to a brightness value (closer = brighter)
    float brightness = 1.0f - (distance / maxDistance);
    brightness = qBound(0.0f, brightness, 1.0f);
    
    // Get RGB components from test color
    int r = static_cast<int>(RGBGetRValue(testColor) * brightness);
    int g = static_cast<int>(RGBGetGValue(testColor) * brightness);
    int b = static_cast<int>(RGBGetBValue(testColor) * brightness);
    
    // Ensure values are within valid range
    r = qBound(0, r, 255);
    g = qBound(0, g, 255);
    b = qBound(0, b, 255);
    
    return ToRGBColor(r, g, b);
}

RGBColor SetupTestUtility::createWaveColor(float distance, float time) const
{
    // Create a wave effect based on distance and time
    float wave = sin(time * 5.0f - distance * 0.5f);
    float intensity = (wave * 0.5f + 0.5f);
    
    // Get RGB components from test color
    int r = static_cast<int>(RGBGetRValue(testColor) * intensity);
    int g = static_cast<int>(RGBGetGValue(testColor) * intensity);
    int b = static_cast<int>(RGBGetBValue(testColor) * intensity);
    
    // Ensure values are within valid range
    r = qBound(0, r, 255);
    g = qBound(0, g, 255);
    b = qBound(0, b, 255);
    
    return ToRGBColor(r, g, b);
}