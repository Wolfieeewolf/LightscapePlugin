/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| SpatialControllerZone.h                                   |
|                                                           |
| Adapter between OpenRGB zones and spatial positions       |
\*---------------------------------------------------------*/

#pragma once

#include <QJsonObject>
#include "ControllerZone.h"
#include "grid/GridTypes.h"
#include "core/DeviceManager.h"

namespace Lightscape {

class SpatialControllerZone
{
public:
    SpatialControllerZone(ControllerZone* zone, const GridPosition& position);
    ~SpatialControllerZone() = default;
    
    // Core data
    ControllerZone* zone;
    GridPosition position;
    
    // Spatial operations
    float getDistanceFrom(const GridPosition& reference) const;
    float getAngleFrom(const GridPosition& reference, int axis) const; // axis: 0=X, 1=Y, 2=Z
    
    // Color operations
    void setLED(int led_idx, const RGBColor& color, int brightness = 100, int temperature = 0, int tint = 0);
    void setAllLEDs(const RGBColor& color, int brightness = 100, int temperature = 0, int tint = 0);
    
    // Zone information
    zone_type getZoneType() const;
    unsigned int getLEDCount() const;
    
    // Matrix operations (if applicable)
    bool isMatrix() const;
    unsigned int getMatrixWidth() const;
    unsigned int getMatrixHeight() const;
    
    // Serialization
    QJsonObject toJson() const;
    static SpatialControllerZone* fromJson(const QJsonObject& json, DeviceManager* device_manager);
    
    // Factory method to convert from existing zones
    static SpatialControllerZone* fromControllerZone(ControllerZone* zone, const GridPosition& position);
    
private:
    // Helper methods
    RGBColor applyColorAdjustments(const RGBColor& color, int brightness, int temperature, int tint) const;
};

} // namespace Lightscape