#pragma once

#include <QWidget>
#include <QJsonObject>
#include <vector>
#include "grid/SpatialGrid.h"
#include "effects/EffectInfo.h"
#include "core/Types.h"

// Forward declarations
class DeviceManager;

namespace Lightscape {

class ControllerZone; // For OpenRGBEffectsPlugin-style interface

class BaseEffect : public QWidget
{
    Q_OBJECT

public:
    explicit BaseEffect(QWidget* parent = nullptr);
    virtual ~BaseEffect() = default;
    
    // Core effect methods
    virtual void initialize(::DeviceManager* deviceManager, ::SpatialGrid* grid);
    virtual void update(float deltaTime);
    virtual void start();
    virtual void stop();
    
    // Spatial color calculation - this is key for grid integration
    virtual RGBColor getColorForPosition(const GridPosition& pos, float time) = 0;
    
    // Apply effect to devices
    virtual void applyToDevices(const QList<DeviceInfo>& devices);
    
    // OpenRGBEffectsPlugin-style interface methods
    virtual void StepEffect(std::vector<ControllerZone*> zones);
    virtual void OnControllerZonesListChanged(std::vector<ControllerZone*> zones);
    
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
    
    // Get the FPS setting
    void setFPS(unsigned int value) { fps = value; }
    unsigned int getFPS() const { return fps; }
    
    // Get the internal time for consistent animation timing
    float getInternalTime() const { return time; }
    
    // Each effect must provide static info
    static EffectInfo GetStaticInfo() { return EffectInfo(); }
    
signals:
    void effectUpdated();
    void settingsChanged();
    
protected:
    ::DeviceManager* deviceManager = nullptr;
    ::SpatialGrid* spatialGrid = nullptr;
    bool isEnabled = false;
    float time = 0.0f;
    
    // Common effect properties
    int speed = 50;
    int brightness = 100;
    GridPosition referencePoint{0, 0, 0};
    QList<RGBColor> userColors;
    bool randomColorsEnabled = false;
    
    // Helper methods for OpenRGBEffectsPlugin-style interface
    void processSpatialZones(std::vector<ControllerZone*> zones, float time);
    void processNonSpatialZones(std::vector<ControllerZone*> zones, float time);
    
    // FPS setting
    unsigned int fps = 60;
    
    // Helper methods for derived classes
    float calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const;
    RGBColor applyBrightness(const RGBColor& color, float brightnessFactor) const;
};

} // namespace Lightscape