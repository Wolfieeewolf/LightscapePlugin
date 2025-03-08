#pragma once

#include <QWidget>
#include <QJsonObject>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "effects/EffectInfo.h"

namespace Lightscape {

// Forward declare for use in BaseEffect class
struct DeviceInfo;

class BaseEffect : public QWidget
{
    Q_OBJECT

public:
    explicit BaseEffect(QWidget* parent = nullptr);
    virtual ~BaseEffect() = default;
    
    // Core effect methods
    virtual void initialize(DeviceManager* deviceManager, SpatialGrid* grid);
    virtual void update(float deltaTime);
    virtual void start();
    virtual void stop();
    
    // Spatial color calculation - this is key for grid integration
    virtual RGBColor getColorForPosition(const GridPosition& pos, float time) = 0;
    
    // Apply effect to devices
    virtual void applyToDevices(const QList<DeviceInfo>& devices);
    
    // Settings management
    virtual void loadSettings(const QJsonObject& json);
    virtual QJsonObject saveSettings() const;
    
    // State management
    void setEnabled(bool enabled) { isEnabled = enabled; }
    bool getEnabled() const { return isEnabled; }
    
    // Common effect properties
    void setSpeed(int value) { speed = value; }
    int getSpeed() const { return speed; }
    
    void setBrightness(int value) { brightness = value; }
    int getBrightness() const { return brightness; }
    
    void setReferencePoint(const GridPosition& pos) { referencePoint = pos; }
    GridPosition getReferencePoint() const { return referencePoint; }
    
    void setColors(const QList<RGBColor>& colors) { userColors = colors; }
    QList<RGBColor> getColors() const { return userColors; }
    
    void setRandomColors(bool value) { randomColorsEnabled = value; }
    bool getRandomColors() const { return randomColorsEnabled; }
    
    // Each effect must provide static info
    static EffectInfo GetStaticInfo() { return EffectInfo(); }
    
signals:
    void effectUpdated();
    void settingsChanged();
    
protected:
    DeviceManager* deviceManager = nullptr;
    SpatialGrid* spatialGrid = nullptr;
    bool isEnabled = false;
    float time = 0.0f;
    
    // Common effect properties
    int speed = 50;
    int brightness = 100;
    GridPosition referencePoint{0, 0, 0};
    QList<RGBColor> userColors;
    bool randomColorsEnabled = false;
    
    // Helper methods for derived classes
    float calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const;
    RGBColor applyBrightness(const RGBColor& color, float brightnessFactor) const;
};

// Definition of DeviceInfo structure
struct DeviceInfo {
    int index;
    DeviceType type;
    int zoneIndex = -1;
    int ledIndex = -1;
    GridPosition position;
};

} // namespace Lightscape