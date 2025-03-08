#include "effects/EffectManager.h"
#include "effects/EffectRegistry.h"
#include "effects/SpatialControllerZone.h"
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include <chrono>
#include <thread>

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
    
    // Clean up any spatial zones
    for (auto* zone : spatialZones)
    {
        delete zone;
    }
    spatialZones.clear();
}

void EffectManager::initialize(::DeviceManager* manager, ::SpatialGrid* grid)
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
    
    // Create a thread for this effect if needed
    // For now we'll use the timer-based approach to ensure compatibility
    /*
    std::thread* effectThread = new std::thread(&EffectManager::effectThreadFunction, this, activeEffect);
    effectThreads[activeEffect] = effectThread;
    */
    
    emit effectStarted(effectId);
    return true;
}

void EffectManager::stopEffect()
{
    if (activeEffect)
    {
        updateTimer->stop();
        activeEffect->stop();
        
        // Clean up any effect thread
        auto it = effectThreads.find(activeEffect);
        if (it != effectThreads.end())
        {
            std::thread* thread = it->second;
            if (thread && thread->joinable())
            {
                thread->join();
                delete thread;
            }
            effectThreads.erase(it);
        }
        
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
    updateZonesFromDevices();
}

void EffectManager::setActiveZones(const std::vector<ControllerZone*>& zones)
{
    activeZones = zones;
}

std::vector<ControllerZone*> EffectManager::getActiveZones() const
{
    return activeZones;
}

void EffectManager::setSpatialZones(const std::vector<SpatialControllerZone*>& zones)
{
    spatialZones = zones;
}

std::vector<SpatialControllerZone*> EffectManager::getSpatialZones() const
{
    return spatialZones;
}

void EffectManager::updateZonesFromDevices()
{
    // Clear existing zones
    for (auto* zone : spatialZones)
    {
        delete zone;
    }
    spatialZones.clear();
    
    // Create new spatial zones from active devices
    for (const DeviceInfo& device : activeDevices)
    {
        if (device.type == DeviceType::RGB && device.zoneIndex >= 0)
        {
            SpatialControllerZone* zone = SpatialControllerZone::fromDeviceInfo(device, deviceManager);
            if (zone)
            {
                spatialZones.push_back(zone);
            }
        }
    }
    
    // Update the active zones list to include spatial zones
    activeZones.clear();
    for (auto* zone : spatialZones)
    {
        activeZones.push_back(zone);
    }
}

void EffectManager::setPreviewEnabled(bool enabled)
{
    previewEnabled = enabled;
}

void EffectManager::addPreview(BaseEffect* effect, ControllerZone* preview)
{
    if (effect && preview)
    {
        previews[effect] = preview;
    }
}

void EffectManager::removePreview(BaseEffect* effect)
{
    if (previews.contains(effect))
    {
        previews.remove(effect);
    }
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
    
    // Update the effect using traditional method
    activeEffect->update(deltaTime);
    
    // Apply effect using both interfaces
    
    // 1. Traditional device-based approach
    if (!activeDevices.isEmpty()) {
        activeEffect->applyToDevices(activeDevices);
    }
    
    // 2. OpenRGBEffectsPlugin-style approach
    if (!activeZones.empty()) {
        activeEffect->StepEffect(activeZones);
    }
    
    // Handle preview
    if (previewEnabled) {
        // Process previews for other effects if any
        for (auto it = previews.begin(); it != previews.end(); ++it) {
            if (it.key() != activeEffect && it.key() != nullptr) {
                // Update the preview effect
                it.key()->update(deltaTime);
                
                // Get the preview zone
                ControllerZone* previewZone = it.value();
                if (previewZone) {
                    // Apply to just this one zone
                    std::vector<ControllerZone*> previewZones = {previewZone};
                    it.key()->StepEffect(previewZones);
                }
            }
        }
        
        emit previewUpdated();
    }
}

void EffectManager::effectThreadFunction(BaseEffect* effect)
{
    if (!effect) return;
    
    std::vector<ControllerZone*> effectZones;
    
    // Get a copy of the zones this thread will work on
    {
        std::lock_guard<std::mutex> lock(mutex);
        effectZones = activeZones;
    }
    
    // Initial callback to inform the effect of available zones
    effect->OnControllerZonesListChanged(effectZones);
    
    // Main effect loop
    while (isRunning && effect == activeEffect)
    {
        // Calculate the frame delay based on FPS
        unsigned int fps = effect->getFPS();
        if (fps == 0) fps = 60; // Default to 60 FPS if not specified
        
        auto frameDelay = std::chrono::milliseconds(1000 / fps);
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Process the effect
        {
            std::lock_guard<std::mutex> lock(mutex);
            effect->StepEffect(effectZones);
        }
        
        // Calculate time spent and sleep for the remainder of the frame time
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (elapsedTime < frameDelay)
        {
            std::this_thread::sleep_for(frameDelay - elapsedTime);
        }
    }
}

void EffectManager::updateDevicePositions()
{
    // Sync spatial zones with device positions from the grid
    if (!spatialGrid) return;
    
    for (auto* zone : spatialZones)
    {
        // Look up the current position in the grid
        if (zone->deviceIndex >= 0 && zone->zoneIndex >= 0)
        {
            DeviceInfo deviceInfo;
            deviceInfo.index = zone->deviceIndex;
            deviceInfo.zoneIndex = zone->zoneIndex;
            
            std::optional<GridPosition> pos = spatialGrid->GetDevicePosition(deviceInfo);
            if (pos.has_value())
            {
                zone->position = pos.value();
            }
        }
    }
}

} // namespace Lightscape