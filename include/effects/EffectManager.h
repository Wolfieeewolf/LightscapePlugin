#pragma once

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include "effects/BaseEffect.h"
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"

namespace Lightscape {

class EffectManager : public QObject
{
    Q_OBJECT

public:
    static EffectManager& getInstance();
    
    void initialize(DeviceManager* deviceManager, SpatialGrid* grid);
    
    // Effect control
    bool startEffect(const QString& effectId);
    void stopEffect();
    bool isEffectRunning() const { return activeEffect != nullptr && isRunning; }
    QString getCurrentEffectId() const { return currentEffectId; }
    BaseEffect* getCurrentEffect() const { return activeEffect; }
    
    // Device selection
    void setActiveDevices(const QList<DeviceInfo>& devices);
    QList<DeviceInfo> getActiveDevices() const { return activeDevices; }
    
    // Preview
    void setPreviewEnabled(bool enabled);
    bool isPreviewEnabled() const { return previewEnabled; }
    void setReducedFps(bool reduced);

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
    
    QTimer* updateTimer;
    DeviceManager* deviceManager = nullptr;
    SpatialGrid* spatialGrid = nullptr;
    BaseEffect* activeEffect = nullptr;
    QString currentEffectId;
    QList<DeviceInfo> activeDevices;
    bool isRunning = false;
    bool previewEnabled = true;
    int updateInterval = 33; // ~30 FPS
    QElapsedTimer frameTimer;
};

} // namespace Lightscape