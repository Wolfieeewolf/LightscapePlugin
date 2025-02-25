#include "effects/EffectManager.h"
#include "effects/EffectRegistry.h"

namespace Lightscape {

EffectManager& EffectManager::getInstance()
{
    static EffectManager instance;
    return instance;
}

EffectManager::EffectManager()
    : updateTimer(new QTimer(this))
{
    connect(updateTimer, &QTimer::timeout, this, &EffectManager::updateEffect);
    frameTimer.start();
}

EffectManager::~EffectManager()
{
    stopEffect();
}

void EffectManager::initialize(DeviceManager* manager, SpatialGrid* grid)
{
    deviceManager = manager;
    spatialGrid = grid;
}

bool EffectManager::startEffect(const QString& effectId)
{
    // Stop any current effect
    stopEffect();
    
    // Create new effect
    void* rawEffect = EffectRegistry::getInstance().createEffect(effectId);
    if (!rawEffect) return false;
    
    activeEffect = static_cast<BaseEffect*>(rawEffect);
    currentEffectId = effectId;
    
    // Initialize effect
    activeEffect->initialize(deviceManager, spatialGrid);
    activeEffect->start();
    
    // Start update timer
    isRunning = true;
    updateTimer->start(updateInterval);
    frameTimer.restart();
    
    emit effectStarted(effectId);
    return true;
}

void EffectManager::stopEffect()
{
    if (activeEffect)
    {
        updateTimer->stop();
        activeEffect->stop();
        delete activeEffect;
        activeEffect = nullptr;
        currentEffectId = "";
        isRunning = false;
        emit effectStopped();
    }
}

void EffectManager::setActiveDevices(const QList<DeviceInfo>& devices)
{
    activeDevices = devices;
}

void EffectManager::setPreviewEnabled(bool enabled)
{
    previewEnabled = enabled;
}

void EffectManager::setReducedFps(bool reduced)
{
    updateInterval = reduced ? 100 : 33; // 10 FPS or 30 FPS
    if (updateTimer->isActive()) {
        updateTimer->setInterval(updateInterval);
    }
}

void EffectManager::updateEffect()
{
    if (!activeEffect || !isRunning) return;
    
    // Calculate delta time in seconds
    float deltaTime = frameTimer.elapsed() / 1000.0f;
    frameTimer.restart();
    
    // Update the effect
    activeEffect->update(deltaTime);
    
    // Apply to devices
    if (!activeDevices.isEmpty()) {
        activeEffect->applyToDevices(activeDevices);
    }
    
    // Emit preview update signal
    if (previewEnabled) {
        emit previewUpdated();
    }
}

} // namespace Lightscape