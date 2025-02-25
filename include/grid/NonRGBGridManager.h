#pragma once

#include <QObject>
#include <QMap>
#include "grid/SpatialGrid.h"
#include "devices/NonRGBDevice.h"

class NonRGBGridManager : public QObject {
    Q_OBJECT

public:
    explicit NonRGBGridManager(SpatialGrid* grid, QObject* parent = nullptr);
    
    // Grid assignment
    bool assignDeviceToGrid(NonRGBDevice* device);
    bool removeDeviceFromGrid(const QString& deviceName);
    bool updateDeviceInGrid(NonRGBDevice* device);
    
    // Query methods
    QVector<GridPosition> getDevicePositions(const QString& deviceName) const;
    bool isPositionOccupied(const GridPosition& pos) const;
    NonRGBDevice* getDeviceAtPosition(const GridPosition& pos) const;
    QStringList getDevicesInGrid() const;
    
    // Validation
    bool canFitDevice(NonRGBDevice* device, const GridPosition& pos) const;
    bool validateDevicePosition(NonRGBDevice* device) const;

signals:
    void deviceAssigned(NonRGBDevice* device);
    void deviceUnassigned(const QString& deviceName);
    void deviceUpdated(NonRGBDevice* device);
    void errorOccurred(const QString& error);

private:
    SpatialGrid* grid;
    QMap<QString, QVector<GridPosition>> devicePositions;  // Device name -> Positions it occupies
    QMap<GridPosition, QString> positionToDevice;          // Position -> Device name
    
    // Helper methods
    QVector<GridPosition> calculateDevicePositions(NonRGBDevice* device) const;
    void clearDevicePositions(const QString& deviceName);
    bool isPositionWithinGrid(const GridPosition& pos) const;
    bool arePositionsAvailable(const QVector<GridPosition>& positions) const;
};