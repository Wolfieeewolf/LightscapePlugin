/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectManager.h                                           |
|                                                           |
| Manager for effect lifecycle and application              |
\*---------------------------------------------------------*/

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
#include "effects/PreviewRenderer.h"

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
    
    // Effect control - original interface
    bool startEffect(const QString& effectId);
    void stopEffect();
    bool isEffectRunning() const { return _activeEffect != nullptr && _isRunning; }
    QString getCurrentEffectId() const { return _currentEffectId; }
    BaseEffect* getCurrentEffect() const { return _activeEffect; }
    
    // Enhanced effect control - supports multiple effects and direct effect instances
    bool startEffect(const QString& effectId, BaseEffect* existingEffect);
    void stopEffect(BaseEffect* effect);
    bool isEffectRunning(BaseEffect* effect) const;
    QList<BaseEffect*> getRunningEffects() const;
    
    // Device selection
    void setActiveDevices(const QList<DeviceInfo>& devices);
    QList<DeviceInfo> getActiveDevices() const { return _activeDevices; }
    
    // Multiple device selections
    void setActiveDevicesForEffect(BaseEffect* effect, const QList<DeviceInfo>& devices);
    QList<DeviceInfo> getActiveDevicesForEffect(BaseEffect* effect) const;
    
    // Zone management (OpenRGBEffectsPlugin-style)
    void setActiveZones(const std::vector<ControllerZone*>& zones);
    std::vector<ControllerZone*> getActiveZones() const;
    
    // Spatial zone management
    void setSpatialZones(const std::vector<SpatialControllerZone*>& zones);
    std::vector<SpatialControllerZone*> getSpatialZones() const;
    
    // Convert device info to zones
    void updateZonesFromDevices();
    
    // Preview controls
    void setPreviewEnabled(bool enabled);
    bool isPreviewEnabled() const { return _previewEnabled; }
    void setPreviewRenderer(PreviewRenderer* renderer);
    void setReducedFps(bool reduced);
    
    // Preview zone management
    void addPreview(BaseEffect* effect, ControllerZone* preview);
    void removePreview(BaseEffect* effect);
    
    // Profile management
    QJsonObject saveProfile() const;
    bool loadProfile(const QJsonObject& profile);

signals:
    void effectStarted(const QString& effectId);
    void effectStopped();
    void effectStarted(BaseEffect* effect);
    void effectStopped(BaseEffect* effect);
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
    
    QTimer* _updateTimer;
    ::DeviceManager* _deviceManager = nullptr;
    ::SpatialGrid* _spatialGrid = nullptr;
    BaseEffect* _activeEffect = nullptr;  // Still keep for backwards compatibility
    QString _currentEffectId;
    QList<DeviceInfo> _activeDevices;
    bool _isRunning = false;
    bool _previewEnabled = true;
    int _updateInterval = 33; // ~30 FPS
    QElapsedTimer _frameTimer;
    
    // Multiple effect support
    QMap<BaseEffect*, bool> _runningEffects;
    QMap<BaseEffect*, QList<DeviceInfo>> _effectDevices;
    
    // Preview
    PreviewRenderer* _previewRenderer = nullptr;
    
    // Zone tracking
    std::vector<ControllerZone*> _activeZones;
    std::vector<SpatialControllerZone*> _spatialZones;
    QMap<BaseEffect*, ControllerZone*> _previews;
    
    // Thread management
    std::map<BaseEffect*, std::thread*> _effectThreads;
    std::mutex _mutex;
};

} // namespace Lightscape
