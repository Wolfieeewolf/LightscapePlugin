#pragma once

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include <QMap>
#include <mutex>
#include <thread>
#include <vector>
#include "effects/BaseEffect.h"
#include "effects/SpatialControllerZone.h"

// Forward declarations
class DeviceManager;
class SpatialGrid;

namespace Lightscape {

class EffectManager : public QObject
{
    Q_OBJECT

public:
    static EffectManager& getInstance();
    
    void initialize(::DeviceManager* deviceManager, ::SpatialGrid* grid);
    
    // Effect control
    bool startEffect(const QString& effectId);
    void stopEffect();
    bool isEffectRunning() const { return activeEffect != nullptr && isRunning; }
    QString getCurrentEffectId() const { return currentEffectId; }
    BaseEffect* getCurrentEffect() const { return activeEffect; }
    
    // Device selection
    void setActiveDevices(const QList<DeviceInfo>& devices);
    QList<DeviceInfo> getActiveDevices() const { return activeDevices; }
    
    // Zone management (OpenRGBEffectsPlugin-style)
    void setActiveZones(const std::vector<ControllerZone*>& zones);
    std::vector<ControllerZone*> getActiveZones() const;
    
    // Spatial zone management
    void setSpatialZones(const std::vector<SpatialControllerZone*>& zones);
    std::vector<SpatialControllerZone*> getSpatialZones() const;
    
    // Convert device info to zones
    void updateZonesFromDevices();
    
    // Preview
    void setPreviewEnabled(bool enabled);
    bool isPreviewEnabled() const { return previewEnabled; }
    void setReducedFps(bool reduced);
    
    // Preview zone management
    void addPreview(BaseEffect* effect, ControllerZone* preview);
    void removePreview(BaseEffect* effect);

signals:
    void effectStarted(const QString& effectId);
    void effectStopped();
    void previewUpdated();

private slots:
    void updateEffect();

private:
    EffectManager();
    ~EffectManager();
    
    // Don't allow copying
    EffectManager(const EffectManager&) = delete;
    EffectManager& operator=(const EffectManager&) = delete;
    
    // Private methods
    void effectThreadFunction(BaseEffect* effect);
    void updateDevicePositions();
    
    QTimer* updateTimer;
    ::DeviceManager* deviceManager = nullptr;
    ::SpatialGrid* spatialGrid = nullptr;
    BaseEffect* activeEffect = nullptr;
    QString currentEffectId;
    QList<DeviceInfo> activeDevices;
    bool isRunning = false;
    bool previewEnabled = true;
    int updateInterval = 33; // ~30 FPS
    QElapsedTimer frameTimer;
    
    // Zone tracking
    std::vector<ControllerZone*> activeZones;
    std::vector<SpatialControllerZone*> spatialZones;
    QMap<BaseEffect*, ControllerZone*> previews;
    
    // Thread management
    std::map<BaseEffect*, std::thread*> effectThreads;
    std::mutex mutex;
};

} // namespace Lightscape