#include "effects/BaseEffect.h"
#include <cmath>

namespace Lightscape {

BaseEffect::BaseEffect(QWidget* parent)
    : QWidget(parent)
{
    // Set default properties for the widget
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    // Initialize with a default color if none provided
    userColors.append(ToRGBColor(255, 0, 0)); // Default red
}

void BaseEffect::initialize(DeviceManager* manager, SpatialGrid* grid)
{
    deviceManager = manager;
    spatialGrid = grid;
    time = 0.0f;
    
    // If reference point is required, get user position from grid as default
    if (spatialGrid && spatialGrid->HasUserPosition() && 
        GetStaticInfo().requiresReferencePoint) {
        auto userPos = spatialGrid->GetUserPosition();
        if (userPos.has_value()) {
            referencePoint = userPos.value();
        }
    }
}

void BaseEffect::update(float deltaTime)
{
    // Update time - actual effects will override this with more specific behavior
    if (isEnabled) {
        time += deltaTime * (speed / 50.0f); // Normalize speed
    }
}

void BaseEffect::start()
{
    isEnabled = true;
}

void BaseEffect::stop()
{
    isEnabled = false;
}

void BaseEffect::applyToDevices(const QList<DeviceInfo>& devices)
{
    if (!deviceManager || !isEnabled) return;
    
    // Apply the effect to each device based on its position
    for (const DeviceInfo& device : devices) {
        RGBColor color = getColorForPosition(device.position, time);
        
        // Apply brightness
        float brightnessValue = brightness / 100.0f;
        color = applyBrightness(color, brightnessValue);
        
        // Apply the color based on device specifics
        if (device.type == DeviceType::RGB) {
            if (device.ledIndex >= 0) {
                // Set specific LED
                deviceManager->SetLEDColor(device.index, device.ledIndex, color);
            }
            else if (device.zoneIndex >= 0) {
                // Set specific zone
                deviceManager->SetZoneColor(device.index, device.zoneIndex, color);
            }
            else {
                // Set whole device
                deviceManager->SetDeviceColor(device.index, color);
            }
        }
        // Could add support for non-RGB devices in the future
    }
}

void BaseEffect::loadSettings(const QJsonObject& json)
{
    // Load basic effect settings
    if (json.contains("speed")) {
        speed = json["speed"].toInt();
    }
    
    if (json.contains("brightness")) {
        brightness = json["brightness"].toInt();
    }
    
    if (json.contains("randomColors")) {
        randomColorsEnabled = json["randomColors"].toBool();
    }
    
    if (json.contains("referencePoint")) {
        QJsonObject refPoint = json["referencePoint"].toObject();
        if (refPoint.contains("x") && refPoint.contains("y") && refPoint.contains("z")) {
            referencePoint.x = refPoint["x"].toInt();
            referencePoint.y = refPoint["y"].toInt();
            referencePoint.z = refPoint["z"].toInt();
        }
    }
    
    if (json.contains("colors")) {
        QJsonArray colorsArray = json["colors"].toArray();
        userColors.clear();
        
        for (const QJsonValue& val : colorsArray) {
            QJsonObject colorObj = val.toObject();
            if (colorObj.contains("r") && colorObj.contains("g") && colorObj.contains("b")) {
                RGBColor color = ToRGBColor(
                    colorObj["r"].toInt(),
                    colorObj["g"].toInt(),
                    colorObj["b"].toInt()
                );
                userColors.append(color);
            }
        }
    }
    
    emit settingsChanged();
}

QJsonObject BaseEffect::saveSettings() const
{
    QJsonObject json;
    
    // Save basic effect settings
    json["speed"] = speed;
    json["brightness"] = brightness;
    json["randomColors"] = randomColorsEnabled;
    
    // Save reference point
    QJsonObject refPoint;
    refPoint["x"] = referencePoint.x;
    refPoint["y"] = referencePoint.y;
    refPoint["z"] = referencePoint.z;
    json["referencePoint"] = refPoint;
    
    // Save colors
    QJsonArray colorsArray;
    for (const RGBColor& color : userColors) {
        QJsonObject colorObj;
        colorObj["r"] = static_cast<int>(RGBGetRValue(color));
        colorObj["g"] = static_cast<int>(RGBGetGValue(color));
        colorObj["b"] = static_cast<int>(RGBGetBValue(color));
        colorsArray.append(colorObj);
    }
    json["colors"] = colorsArray;
    
    return json;
}

float BaseEffect::calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const
{
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float dz = pos1.z - pos2.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

RGBColor BaseEffect::applyBrightness(const RGBColor& color, float brightnessFactor) const
{
    // Clamp brightness to reasonable values
    float clampedBrightness = std::max(0.0f, std::min(1.0f, brightnessFactor));
    
    // Apply brightness to each component
    int r = static_cast<int>(RGBGetRValue(color) * clampedBrightness);
    int g = static_cast<int>(RGBGetGValue(color) * clampedBrightness);
    int b = static_cast<int>(RGBGetBValue(color) * clampedBrightness);
    
    return ToRGBColor(r, g, b);
}

} // namespace Lightscape