#include "effects/SpatialControllerZone.h"
#include "devices/DeviceManager.h"
#include "core/Types.h"
#include <cmath>

namespace Lightscape {

SpatialControllerZone::SpatialControllerZone(int device_index, int zone_index, const GridPosition& pos, ::DeviceManager* device_manager)
    : deviceIndex(device_index)
    , zoneIndex(zone_index)
    , position(pos)
    , deviceManager(device_manager)
{
}

float SpatialControllerZone::getDistanceFrom(const GridPosition& reference) const
{
    float dx = position.x - reference.x;
    float dy = position.y - reference.y;
    float dz = position.z - reference.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

float SpatialControllerZone::getAngleFrom(const GridPosition& reference, int axis) const
{
    // Calculate the angle in radians based on axis (0 = XY, 1 = XZ, 2 = YZ)
    float dx = position.x - reference.x;
    float dy = position.y - reference.y;
    float dz = position.z - reference.z;
    
    switch (axis)
    {
        case 0: // XY plane
            return std::atan2(dy, dx);
        case 1: // XZ plane
            return std::atan2(dz, dx);
        case 2: // YZ plane
            return std::atan2(dz, dy);
        default:
            return 0.0f;
    }
}

unsigned int SpatialControllerZone::getLEDCount() const
{
    if (!deviceManager) return 0;
    return static_cast<unsigned int>(deviceManager->GetZoneCount(deviceIndex));
}

void SpatialControllerZone::setLED(int led_idx, RGBColor color, int brightness, int /*temperature*/, int /*tint*/)
{
    if (!deviceManager) return;
    
    // Apply brightness
    float brightnessF = brightness / 100.0f;
    int r = static_cast<int>(RGBGetRValue(color) * brightnessF);
    int g = static_cast<int>(RGBGetGValue(color) * brightnessF);
    int b = static_cast<int>(RGBGetBValue(color) * brightnessF);
    
    // Apply other adjustments if needed (temperature, tint)
    
    // Set the LED color
    RGBColor finalColor = ToRGBColor(r, g, b);
    deviceManager->SetLEDColor(deviceIndex, led_idx, finalColor);
}

void SpatialControllerZone::setAllLEDs(RGBColor color, int brightness, int /*temperature*/, int /*tint*/)
{
    if (!deviceManager) return;
    
    // Apply brightness
    float brightnessF = brightness / 100.0f;
    int r = static_cast<int>(RGBGetRValue(color) * brightnessF);
    int g = static_cast<int>(RGBGetGValue(color) * brightnessF);
    int b = static_cast<int>(RGBGetBValue(color) * brightnessF);
    
    // Set the zone color
    RGBColor finalColor = ToRGBColor(r, g, b);
    deviceManager->SetZoneColor(deviceIndex, zoneIndex, finalColor);
}

bool SpatialControllerZone::isMatrix() const
{
    if (!deviceManager) return false;
    
    // Determine if this is a matrix zone
    // Since we don't have direct access to zone type, assume it's not a matrix for now
    return false;
}

unsigned int SpatialControllerZone::getMatrixWidth() const
{
    if (!deviceManager || !isMatrix()) return 0;
    
    // Get matrix dimensions - since we don't have zone information, return 0
    return 0;
}

unsigned int SpatialControllerZone::getMatrixHeight() const
{
    if (!deviceManager || !isMatrix()) return 0;
    
    // Get matrix dimensions - since we don't have zone information, return 0
    return 0;
}

ControllerZone::ZoneType SpatialControllerZone::getZoneType() const
{
    if (!deviceManager) return ZoneType::Single;
    
    // Since we don't have zone type information, assume single
    return ZoneType::Single;
}

QJsonObject SpatialControllerZone::toJson() const
{
    QJsonObject json;
    json["device_index"] = deviceIndex;
    json["zone_index"] = zoneIndex;
    
    QJsonObject posJson;
    posJson["x"] = position.x;
    posJson["y"] = position.y;
    posJson["z"] = position.z;
    json["position"] = posJson;
    
    return json;
}

SpatialControllerZone* SpatialControllerZone::fromJson(const QJsonObject& json, ::DeviceManager* device_manager)
{
    if (!json.contains("device_index") || !json.contains("zone_index") || !json.contains("position"))
        return nullptr;
    
    int deviceIdx = json["device_index"].toInt();
    int zoneIdx = json["zone_index"].toInt();
    
    QJsonObject posJson = json["position"].toObject();
    GridPosition pos(
        posJson["x"].toDouble(),
        posJson["y"].toDouble(),
        posJson["z"].toDouble()
    );
    
    return new SpatialControllerZone(deviceIdx, zoneIdx, pos, device_manager);
}

SpatialControllerZone* SpatialControllerZone::fromDeviceInfo(const DeviceInfo& info, ::DeviceManager* device_manager)
{
    // Convert a DeviceInfo structure to a SpatialControllerZone
    if (info.zoneIndex < 0) return nullptr; // Must have a valid zone
    
    return new SpatialControllerZone(info.index, info.zoneIndex, info.position, device_manager);
}

} // namespace Lightscape