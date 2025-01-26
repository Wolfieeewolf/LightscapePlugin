#pragma once

#include <QString>
#include <QSize>
#include <QMap>
#include "grid/SpatialGrid.h"
#include "devices/NonRGBDeviceTypes.h"

class NonRGBDevice {
public:
    explicit NonRGBDevice(const QString& name = "", NonRGBDeviceType type = NonRGBDeviceType::Custom);
    
    // Basic properties
    QString getName() const { return name; }
    void setName(const QString& name);
    
    NonRGBDeviceType getType() const { return type; }
    void setType(NonRGBDeviceType type);
    
    // Position management
    GridPosition getPosition() const { return position; }
    void setPosition(const GridPosition& pos);
    
    // Dimensions
    QSize getDimensions() const { return dimensions; }
    void setDimensions(const QSize& size);
    int getDepth() const { return depth; }
    void setDepth(int d);
    
    // Reference points
    void addReferencePoint(const QString& name, const GridPosition& pos);
    void removeReferencePoint(const QString& name);
    GridPosition getReferencePoint(const QString& name) const;
    QList<QString> getReferencePointNames() const;
    const QMap<QString, GridPosition>& getAllReferencePoints() const { return referencePoints; }
    
    // Properties
    void setProperty(const QString& key, const QString& value);
    QString getProperty(const QString& key) const;
    bool hasProperty(const QString& key) const;
    void removeProperty(const QString& key);
    QList<QString> getPropertyKeys() const;

private:
    QString name;
    NonRGBDeviceType type;
    GridPosition position;
    QSize dimensions;
    int depth;
    QMap<QString, GridPosition> referencePoints;
    QMap<QString, QString> properties;
};