#include "devices/NonRGBDevice.h"

NonRGBDevice::NonRGBDevice(const QString& name, NonRGBDeviceType type)
    : name(name)
    , type(type)
    , position(0, 0, 0)
    , dimensions(1, 1)
    , depth(1)
{
}

void NonRGBDevice::setName(const QString& name)
{
    this->name = name;
}

void NonRGBDevice::setType(NonRGBDeviceType type)
{
    this->type = type;
}

void NonRGBDevice::setPosition(const GridPosition& pos)
{
    position = pos;
}

void NonRGBDevice::setDimensions(const QSize& size)
{
    dimensions = size;
}

void NonRGBDevice::setDepth(int d)
{
    depth = d > 0 ? d : 1;  // Ensure depth is at least 1
}

void NonRGBDevice::addReferencePoint(const QString& name, const GridPosition& pos)
{
    referencePoints[name] = pos;
}

void NonRGBDevice::removeReferencePoint(const QString& name)
{
    referencePoints.remove(name);
}

GridPosition NonRGBDevice::getReferencePoint(const QString& name) const
{
    return referencePoints.value(name, position);  // Default to main position if reference not found
}

QList<QString> NonRGBDevice::getReferencePointNames() const
{
    return referencePoints.keys();
}

void NonRGBDevice::setProperty(const QString& key, const QString& value)
{
    properties[key] = value;
}

QString NonRGBDevice::getProperty(const QString& key) const
{
    return properties.value(key);
}

bool NonRGBDevice::hasProperty(const QString& key) const
{
    return properties.contains(key);
}

void NonRGBDevice::removeProperty(const QString& key)
{
    properties.remove(key);
}

QList<QString> NonRGBDevice::getPropertyKeys() const
{
    return properties.keys();
}