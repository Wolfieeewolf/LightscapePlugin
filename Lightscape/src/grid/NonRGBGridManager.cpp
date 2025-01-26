#include "grid/NonRGBGridManager.h"

NonRGBGridManager::NonRGBGridManager(SpatialGrid* grid, QObject* parent)
    : QObject(parent)
    , grid(grid)
{
}

bool NonRGBGridManager::assignDeviceToGrid(NonRGBDevice* device)
{
    if (!device) {
        emit errorOccurred("Cannot assign null device to grid");
        return false;
    }

    QString deviceName = device->getName();
    if (deviceName.isEmpty()) {
        emit errorOccurred("Device must have a name to be assigned to grid");
        return false;
    }

    // Calculate all positions this device will occupy
    QVector<GridPosition> positions = calculateDevicePositions(device);
    
    // Validate positions
    if (positions.isEmpty()) {
        emit errorOccurred("Device positions could not be calculated");
        return false;
    }

    // Check if positions are available (except for positions already owned by this device)
    bool positionsAvailable = true;
    for (const GridPosition& pos : positions) {
        if (isPositionOccupied(pos) && positionToDevice[pos] != deviceName) {
            positionsAvailable = false;
            break;
        }
    }

    if (!positionsAvailable) {
        emit errorOccurred("One or more positions are already occupied by another device");
        return false;
    }

    // Remove old positions if device was previously assigned
    if (devicePositions.contains(deviceName)) {
        clearDevicePositions(deviceName);
    }

    // Assign new positions
    devicePositions[deviceName] = positions;
    for (const GridPosition& pos : positions) {
        positionToDevice[pos] = deviceName;
    }

    emit deviceAssigned(device);
    return true;
}

bool NonRGBGridManager::removeDeviceFromGrid(const QString& deviceName)
{
    if (!devicePositions.contains(deviceName)) {
        return false;
    }

    clearDevicePositions(deviceName);
    emit deviceUnassigned(deviceName);
    return true;
}

bool NonRGBGridManager::updateDeviceInGrid(NonRGBDevice* device)
{
    if (!device) return false;
    
    QString deviceName = device->getName();
    if (!devicePositions.contains(deviceName)) {
        return assignDeviceToGrid(device);
    }

    // Calculate new positions
    QVector<GridPosition> newPositions = calculateDevicePositions(device);
    
    // Check if new positions are available (except for current device's positions)
    for (const GridPosition& pos : newPositions) {
        if (isPositionOccupied(pos) && positionToDevice[pos] != deviceName) {
            emit errorOccurred("Cannot update device position: space is occupied");
            return false;
        }
    }

    // Update positions
    clearDevicePositions(deviceName);
    devicePositions[deviceName] = newPositions;
    for (const GridPosition& pos : newPositions) {
        positionToDevice[pos] = deviceName;
    }

    emit deviceUpdated(device);
    return true;
}

QVector<GridPosition> NonRGBGridManager::getDevicePositions(const QString& deviceName) const
{
    return devicePositions.value(deviceName);
}

bool NonRGBGridManager::isPositionOccupied(const GridPosition& pos) const
{
    return positionToDevice.contains(pos);
}

NonRGBDevice* NonRGBGridManager::getDeviceAtPosition(const GridPosition& pos) const
{
    QString deviceName = positionToDevice.value(pos);
    if (deviceName.isEmpty()) return nullptr;
    
    // Note: This requires access to NonRGBDeviceManager
    // You might want to pass it in constructor or use a different approach
    // For now, return nullptr
    return nullptr;
}

QStringList NonRGBGridManager::getDevicesInGrid() const
{
    return devicePositions.keys();
}

bool NonRGBGridManager::canFitDevice(NonRGBDevice* device, const GridPosition& pos) const
{
    if (!device) return false;

    QVector<GridPosition> requiredPositions;
    QSize dims = device->getDimensions();
    int depth = device->getDepth();

    for (int z = pos.z; z < pos.z + depth; z++) {
        for (int y = pos.y; y < pos.y + dims.height(); y++) {
            for (int x = pos.x; x < pos.x + dims.width(); x++) {
                GridPosition testPos(x, y, z);
                if (!isPositionWithinGrid(testPos) || 
                    (isPositionOccupied(testPos) && positionToDevice[testPos] != device->getName())) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool NonRGBGridManager::validateDevicePosition(NonRGBDevice* device) const
{
    if (!device) return false;
    return canFitDevice(device, device->getPosition());
}

void NonRGBGridManager::clearDevicePositions(const QString& deviceName)
{
    if (!devicePositions.contains(deviceName)) return;

    const QVector<GridPosition>& positions = devicePositions[deviceName];
    for (const GridPosition& pos : positions) {
        positionToDevice.remove(pos);
    }
    devicePositions.remove(deviceName);
}

QVector<GridPosition> NonRGBGridManager::calculateDevicePositions(NonRGBDevice* device) const
{
    if (!device) return QVector<GridPosition>();

    QVector<GridPosition> positions;
    GridPosition basePos = device->getPosition();
    QSize dims = device->getDimensions();
    int depth = device->getDepth();

    // Calculate all positions the device will occupy
    for (int z = basePos.z; z < basePos.z + depth; z++) {
        for (int y = basePos.y; y < basePos.y + dims.height(); y++) {
            for (int x = basePos.x; x < basePos.x + dims.width(); x++) {
                GridPosition pos(x, y, z);
                if (isPositionWithinGrid(pos)) {
                    positions.append(pos);
                }
            }
        }
    }

    return positions;
}

bool NonRGBGridManager::isPositionWithinGrid(const GridPosition& pos) const
{
    if (!grid) return false;
    
    GridDimensions dims = grid->GetDimensions();
    return pos.x >= 0 && pos.x < dims.width &&
           pos.y >= 0 && pos.y < dims.height &&
           pos.z >= 0 && pos.z < dims.depth;
}

bool NonRGBGridManager::arePositionsAvailable(const QVector<GridPosition>& positions) const
{
    for (const GridPosition& pos : positions) {
        if (!isPositionWithinGrid(pos) || isPositionOccupied(pos)) {
            return false;
        }
    }
    return true;
}