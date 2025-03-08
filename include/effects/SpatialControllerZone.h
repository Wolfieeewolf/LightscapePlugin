#pragma once

#include <QJsonObject>
#include "grid/GridTypes.h"
#include "RGBController.h"
#include "core/Types.h"

// Forward declaration for DeviceManager
class DeviceManager;

namespace Lightscape {

/**
 * Abstract base class for controller zones, used by effects
 * This provides a common interface for both spatial and non-spatial zones
 */
class ControllerZone
{
public:
    virtual ~ControllerZone() = default;
    
    // Zone information
    virtual unsigned int getLEDCount() const = 0;
    
    // Color operations (using OpenRGB's RGBColor type)
    virtual void setLED(int led_idx, RGBColor color, int brightness = 100, int temperature = 0, int tint = 0) = 0;
    virtual void setAllLEDs(RGBColor color, int brightness = 100, int temperature = 0, int tint = 0) = 0;
    
    // Matrix support
    virtual bool isMatrix() const = 0;
    virtual unsigned int getMatrixWidth() const = 0;
    virtual unsigned int getMatrixHeight() const = 0;
    
    // Zone type enumeration
    enum class ZoneType {
        Single,    // Single LED
        Linear,    // Linear array of LEDs
        Matrix,    // 2D array of LEDs
        Custom     // Custom arrangement
    };
    
    // Zone type access
    virtual ZoneType getZoneType() const = 0;
};

/**
 * SpatialControllerZone adapter class
 * This bridges between OpenRGB controller zones and Lightscape's spatial grid
 */
class SpatialControllerZone : public ControllerZone
{
public:
    // Constructor
    SpatialControllerZone(int device_index, int zone_index, const GridPosition& position, ::DeviceManager* device_manager);
    ~SpatialControllerZone() override = default;
    
    // Core data
    int deviceIndex;
    int zoneIndex;
    GridPosition position;
    ::DeviceManager* deviceManager;
    
    // Spatial operations
    float getDistanceFrom(const GridPosition& reference) const;
    float getAngleFrom(const GridPosition& reference, int axis) const;
    
    // ControllerZone interface implementation
    unsigned int getLEDCount() const override;
    void setLED(int led_idx, RGBColor color, int brightness = 100, int temperature = 0, int tint = 0) override;
    void setAllLEDs(RGBColor color, int brightness = 100, int temperature = 0, int tint = 0) override;
    bool isMatrix() const override;
    unsigned int getMatrixWidth() const override;
    unsigned int getMatrixHeight() const override;
    ZoneType getZoneType() const override;
    
    // Serialization
    QJsonObject toJson() const;
    static SpatialControllerZone* fromJson(const QJsonObject& json, ::DeviceManager* device_manager);
    
    // Factory method to create from existing zones
    static SpatialControllerZone* fromDeviceInfo(const DeviceInfo& info, ::DeviceManager* device_manager);
};

} // namespace Lightscape