/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectManager.cpp                                         |
|                                                           |
| Manager for effect lifecycle and application              |
\*---------------------------------------------------------*/

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
    : _updateTimer(new QTimer(this))
{
    connect(_updateTimer, &QTimer::timeout, this, &EffectManager::updateEffect);
    _frameTimer.start();
}

EffectManager::~EffectManager()
{
    // Stop all effects
    for (auto it = _runningEffects.begin(); it != _runningEffects.end(); ++it) {
        BaseEffect* effect = it.key();
        if (effect) {
            effect->stop();
        }
    }
    
    _runningEffects.clear();
    
    // Legacy: Stop current effect
    stopEffect();
    
    // Clean up any spatial zones
    for (auto* zone : _spatialZones)
    {
        delete zone;
    }
    _spatialZones.clear();
}

void EffectManager::initialize(::DeviceManager* manager, ::SpatialGrid* grid)
{
    _deviceManager = manager;
    _spatialGrid = grid;
}

bool EffectManager::startEffect(const QString& effectId)
{
    // Legacy method that creates a new effect instance
    // Stop any current effect
    stopEffect();
    
    // Create new effect
    void* rawEffect = EffectRegistry::getInstance().createEffect(effectId);
    if (!rawEffect) return false;
    
    _activeEffect = static_cast<BaseEffect*>(rawEffect);
    _currentEffectId = effectId;
    
    // Initialize effect
    _activeEffect->initialize(_deviceManager, _spatialGrid);
    _activeEffect->start();
    
    // Start update timer
    _isRunning = true;
    if (!_updateTimer->isActive()) {
        _updateTimer->start(_updateInterval);
    }
    _frameTimer.restart();
    
    // Add to running effects
    _runningEffects[_activeEffect] = true;
    
    emit effectStarted(effectId);
    emit effectStarted(_activeEffect);
    return true;
}

void EffectManager::stopEffect()
{
    // Legacy method that stops the current effect
    if (_activeEffect)
    {
        if (!_updateTimer->isActive()) {
            _updateTimer->stop();
        }
        _activeEffect->stop();
        
        // Clean up any effect thread
        auto it = _effectThreads.find(_activeEffect);
        if (it != _effectThreads.end())
        {
            std::thread* thread = it->second;
            if (thread && thread->joinable())
            {
                thread->join();
                delete thread;
            }
            _effectThreads.erase(it);
        }
        
        // Remove from running effects
        _runningEffects.remove(_activeEffect);
        
        emit effectStopped();
        emit effectStopped(_activeEffect);
        
        delete _activeEffect;
        _activeEffect = nullptr;
        _currentEffectId = "";
        _isRunning = false;
    }
}

bool EffectManager::startEffect(const QString& effectId, BaseEffect* existingEffect)
{
    // Enhanced method that uses an existing effect instance
    if (!existingEffect) {
        // Fall back to legacy method
        return startEffect(effectId);
    }
    
    printf("[Lightscape][EffectManager] Starting effect: %s (ptr: %p)\n", 
           effectId.toStdString().c_str(), (void*)existingEffect);
    
    // Check if effect is already running
    if (_runningEffects.contains(existingEffect) && _runningEffects[existingEffect]) {
        printf("[Lightscape][EffectManager] Effect already running\n");
        return true; // Already running
    }
    
    // Start the effect
    existingEffect->start();
    
    // Start update timer if not already running
    if (!_updateTimer->isActive()) {
        _updateTimer->start(_updateInterval);
        printf("[Lightscape][EffectManager] Started update timer\n");
    }
    
    // Add to running effects
    _runningEffects[existingEffect] = true;
    
    // Legacy: update current effect if none is set
    if (!_activeEffect) {
        _activeEffect = existingEffect;
        _currentEffectId = effectId;
        _isRunning = true;
    }
    
    // Get the device info for this effect 
    QList<DeviceInfo> deviceInfo = _effectDevices.value(existingEffect, QList<DeviceInfo>());
    printf("[Lightscape][EffectManager] Effect has %d devices\n", deviceInfo.size());
    
    // Force an immediate update
    if (!deviceInfo.isEmpty()) {
        printf("[Lightscape][EffectManager] Applying immediate colors on effect start\n");
        existingEffect->applyToDevices(deviceInfo);
    }
    
    emit effectStarted(effectId);
    emit effectStarted(existingEffect);
    return true;
}

void EffectManager::stopEffect(BaseEffect* effect)
{
    // Enhanced method that stops a specific effect
    if (!effect) {
        // Fall back to legacy method
        stopEffect();
        return;
    }
    
    // Check if effect is actually running
    if (!_runningEffects.contains(effect) || !_runningEffects[effect]) {
        return; // Not running
    }
    
    // Stop the effect
    effect->stop();
    
    // Remove from running effects
    _runningEffects[effect] = false;
    
    // Check if we should stop the timer
    bool anyRunning = false;
    for (auto it = _runningEffects.begin(); it != _runningEffects.end(); ++it) {
        if (it.value()) {
            anyRunning = true;
            break;
        }
    }
    
    if (!anyRunning) {
        _updateTimer->stop();
    }
    
    // Legacy: clear current effect if it's this one
    if (_activeEffect == effect) {
        _activeEffect = nullptr;
        _currentEffectId = "";
        _isRunning = false;
    }
    
    emit effectStopped(effect);
    if (_activeEffect == effect) {
        emit effectStopped();
    }
}

bool EffectManager::isEffectRunning(BaseEffect* effect) const
{
    return _runningEffects.contains(effect) && _runningEffects[effect];
}

QList<BaseEffect*> EffectManager::getRunningEffects() const
{
    QList<BaseEffect*> running;
    for (auto it = _runningEffects.begin(); it != _runningEffects.end(); ++it) {
        if (it.value()) {
            running.append(it.key());
        }
    }
    return running;
}

void EffectManager::setActiveDevices(const QList<DeviceInfo>& devices)
{
    _activeDevices = devices;
    updateZonesFromDevices();
    
    // Legacy: if current effect is set, update its devices
    if (_activeEffect) {
        _effectDevices[_activeEffect] = devices;
    }
}

void EffectManager::setActiveDevicesForEffect(BaseEffect* effect, const QList<DeviceInfo>& devices)
{
    if (!effect) return;
    
    printf("[Lightscape][EffectManager] Setting %d active devices for effect: %s\n", 
           devices.size(), 
           effect->GetStaticInfo().id.toStdString().c_str());
    
    _effectDevices[effect] = devices;
    
    // Update active devices for the current effect
    if (_activeEffect == effect) {
        _activeDevices = devices;
        updateZonesFromDevices();
    }
    
    // If the effect is running, update the device colors immediately
    if (isEffectRunning(effect)) {
        printf("[Lightscape][EffectManager] Effect is running, applying colors immediately\n");
        // Force an update by calling applyToDevices directly
        effect->applyToDevices(devices);
    }
}

QList<DeviceInfo> EffectManager::getActiveDevicesForEffect(BaseEffect* effect) const
{
    return _effectDevices.value(effect, QList<DeviceInfo>());
}

void EffectManager::setActiveZones(const std::vector<ControllerZone*>& zones)
{
    _activeZones = zones;
}

std::vector<ControllerZone*> EffectManager::getActiveZones() const
{
    return _activeZones;
}

void EffectManager::setSpatialZones(const std::vector<SpatialControllerZone*>& zones)
{
    _spatialZones = zones;
}

std::vector<SpatialControllerZone*> EffectManager::getSpatialZones() const
{
    return _spatialZones;
}

void EffectManager::updateZonesFromDevices()
{
    // Clear existing zones
    for (auto* zone : _spatialZones)
    {
        delete zone;
    }
    _spatialZones.clear();
    
    // Create new spatial zones from active devices
    for (const DeviceInfo& device : _activeDevices)
    {
        if (device.type == DeviceType::RGB && device.zoneIndex >= 0)
        {
            SpatialControllerZone* zone = SpatialControllerZone::fromDeviceInfo(device, _deviceManager);
            if (zone)
            {
                _spatialZones.push_back(zone);
            }
        }
    }
    
    // Update the active zones list to include spatial zones
    _activeZones.clear();
    for (auto* zone : _spatialZones)
    {
        _activeZones.push_back(zone);
    }
}

void EffectManager::setPreviewEnabled(bool enabled)
{
    _previewEnabled = enabled;
}

void EffectManager::setPreviewRenderer(PreviewRenderer* renderer)
{
    _previewRenderer = renderer;
}

void EffectManager::addPreview(BaseEffect* effect, ControllerZone* preview)
{
    if (effect && preview)
    {
        _previews[effect] = preview;
    }
}

void EffectManager::removePreview(BaseEffect* effect)
{
    if (_previews.contains(effect))
    {
        _previews.remove(effect);
    }
}

void EffectManager::setReducedFps(bool reduced)
{
    _updateInterval = reduced ? 100 : 33; // 10 FPS or 30 FPS
    if (_updateTimer->isActive()) {
        _updateTimer->setInterval(_updateInterval);
    }
}

QJsonObject EffectManager::saveProfile() const
{
    QJsonObject profile;
    QJsonArray effectsArray;
    
    // Get all running effects
    QList<BaseEffect*> runningEffects = getRunningEffects();
    
    // Save each effect
    for (BaseEffect* effect : runningEffects) {
        if (!effect) continue;
        
        QJsonObject effectObject;
        
        // Save basic info
        effectObject["id"] = effect->GetStaticInfo().id;
        effectObject["running"] = true;
        
        // Save effect settings
        effectObject["settings"] = effect->saveSettings();
        
        // Save associated devices
        QJsonArray devicesArray;
        QList<DeviceInfo> devices = _effectDevices.value(effect, QList<DeviceInfo>());
        
        for (const DeviceInfo& device : devices) {
            QJsonObject deviceObject;
            deviceObject["index"] = device.index;
            deviceObject["type"] = static_cast<int>(device.type);
            
            if (device.zoneIndex >= 0) {
                deviceObject["zone_index"] = device.zoneIndex;
            }
            
            if (device.ledIndex >= 0) {
                deviceObject["led_index"] = device.ledIndex;
            }
            
            QJsonObject posObject;
            posObject["x"] = device.position.x;
            posObject["y"] = device.position.y;
            posObject["z"] = device.position.z;
            deviceObject["position"] = posObject;
            
            devicesArray.append(deviceObject);
        }
        
        effectObject["devices"] = devicesArray;
        
        // Add to array
        effectsArray.append(effectObject);
    }
    
    profile["effects"] = effectsArray;
    profile["version"] = 1.0;
    
    return profile;
}

bool EffectManager::loadProfile(const QJsonObject& profile)
{
    // Check format
    if (!profile.contains("effects") || !profile.contains("version")) {
        return false;
    }
    
    // Stop all running effects
    for (auto it = _runningEffects.begin(); it != _runningEffects.end(); ++it) {
        BaseEffect* effect = it.key();
        if (effect) {
            stopEffect(effect);
        }
    }
    
    // Clear effect devices
    _effectDevices.clear();
    
    // Load effects
    QJsonArray effectsArray = profile["effects"].toArray();
    for (const QJsonValue& effectValue : effectsArray) {
        QJsonObject effectObject = effectValue.toObject();
        
        // Check for required fields
        if (!effectObject.contains("id") || !effectObject.contains("settings")) {
            continue;
        }
        
        // Get effect ID
        QString effectId = effectObject["id"].toString();
        
        // Create effect
        BaseEffect* effect = static_cast<BaseEffect*>(EffectRegistry::getInstance().createEffect(effectId));
        if (!effect) continue;
        
        // Initialize
        effect->initialize(_deviceManager, _spatialGrid);
        
        // Load settings
        QJsonObject settingsObject = effectObject["settings"].toObject();
        effect->loadSettings(settingsObject);
        
        // Load devices
        QList<DeviceInfo> devices;
        if (effectObject.contains("devices")) {
            QJsonArray devicesArray = effectObject["devices"].toArray();
            
            for (const QJsonValue& deviceValue : devicesArray) {
                QJsonObject deviceObject = deviceValue.toObject();
                
                // Create device info
                DeviceInfo device;
                device.index = deviceObject["index"].toInt();
                device.type = static_cast<DeviceType>(deviceObject["type"].toInt());
                
                // Optional zone and LED indices
                if (deviceObject.contains("zone_index")) {
                    device.zoneIndex = deviceObject["zone_index"].toInt();
                }
                
                if (deviceObject.contains("led_index")) {
                    device.ledIndex = deviceObject["led_index"].toInt();
                }
                
                // Position
                if (deviceObject.contains("position")) {
                    QJsonObject posObject = deviceObject["position"].toObject();
                    device.position.x = posObject["x"].toInt();
                    device.position.y = posObject["y"].toInt();
                    device.position.z = posObject["z"].toInt();
                }
                
                devices.append(device);
            }
        }
        
        // Store devices
        _effectDevices[effect] = devices;
        
        // Start effect if needed
        bool running = effectObject["running"].toBool(true);
        if (running) {
            startEffect(effectId, effect);
        }
    }
    
    return true;
}

void EffectManager::updateEffect()
{
    // Calculate delta time in seconds
    float deltaTime = _frameTimer.elapsed() / 1000.0f;
    _frameTimer.restart();
    
    // Create local copies to minimize lock duration
    QMap<BaseEffect*, bool> runningEffectsCopy;
    QMap<BaseEffect*, QList<DeviceInfo>> effectDevicesCopy;
    std::vector<ControllerZone*> activeZonesCopy;
    BaseEffect* activeEffectCopy = nullptr;
    
    {
        std::lock_guard<std::mutex> lock(_mutex);
        runningEffectsCopy = _runningEffects;
        effectDevicesCopy = _effectDevices;
        activeZonesCopy = _activeZones;
        activeEffectCopy = _activeEffect;
    }
    
    // Update all running effects
    for (auto it = runningEffectsCopy.begin(); it != runningEffectsCopy.end(); ++it) {
        if (!it.value()) continue; // Skip effects that aren't running
        
        BaseEffect* effect = it.key();
        if (!effect) continue;
        
        try {
            // Update the effect
            effect->update(deltaTime);
            
            // Get devices for this effect
            QList<DeviceInfo> effectDevices = effectDevicesCopy.value(effect, QList<DeviceInfo>());
            
            // Apply effect if it has devices
            if (!effectDevices.isEmpty()) {
                printf("[Lightscape][EffectManager] Applying effect to %d devices\n", effectDevices.size());
                effect->applyToDevices(effectDevices);
            } else {
                printf("[Lightscape][EffectManager] WARNING: Effect has NO devices assigned!\n");
            }
            
            // Legacy: also use zones for the current effect
            if (effect == activeEffectCopy && !activeZonesCopy.empty()) {
                // Create a copy to avoid modifying the vector during iteration
                std::vector<ControllerZone*> zonesCopy = activeZonesCopy;
                effect->StepEffect(zonesCopy);
            }
        } catch (const std::exception& e) {
            printf("[Lightscape][EffectManager] Exception in effect update: %s\n", e.what());
        } catch (...) {
            printf("[Lightscape][EffectManager] Unknown exception in effect update\n");
        }
    }
    
    // Handle preview (with separate lock scope)
    if (_previewEnabled) {
        // Update preview renderer if available
        PreviewRenderer* previewRendererCopy = nullptr;
        QMap<BaseEffect*, ControllerZone*> previewsCopy;
        
        {
            std::lock_guard<std::mutex> lock(_mutex);
            previewRendererCopy = _previewRenderer;
            previewsCopy = _previews;
        }
        
        // Update preview renderer
        if (previewRendererCopy) {
            previewRendererCopy->update();
        }
        
        // Process previews for effects
        for (auto it = previewsCopy.begin(); it != previewsCopy.end(); ++it) {
            BaseEffect* effect = it.key();
            ControllerZone* previewZone = it.value();
            
            if (effect && previewZone) {
                try {
                    // Update the effect
                    effect->update(deltaTime);
                    
                    // Apply to just this one zone
                    std::vector<ControllerZone*> previewZones = {previewZone};
                    effect->StepEffect(previewZones);
                } catch (const std::exception& e) {
                    printf("[Lightscape][EffectManager] Exception in preview update: %s\n", e.what());
                } catch (...) {
                    printf("[Lightscape][EffectManager] Unknown exception in preview update\n");
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
        std::lock_guard<std::mutex> lock(_mutex);
        effectZones = _activeZones;
    }
    
    // Initial callback to inform the effect of available zones
    effect->OnControllerZonesListChanged(effectZones);
    
    // Check if the effect is still running
    bool isRunning = _runningEffects.contains(effect) && _runningEffects[effect];
    
    // Main effect loop
    while (isRunning)
    {
        // Calculate the frame delay based on FPS
        unsigned int fps = effect->getFPS();
        if (fps == 0) fps = 60; // Default to 60 FPS if not specified
        
        auto frameDelay = std::chrono::milliseconds(1000 / fps);
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Process the effect
        {
            std::lock_guard<std::mutex> lock(_mutex);
            effect->StepEffect(effectZones);
        }
        
        // Calculate time spent and sleep for the remainder of the frame time
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        if (elapsedTime < frameDelay)
        {
            std::this_thread::sleep_for(frameDelay - elapsedTime);
        }
        
        // Check if effect is still running
        isRunning = _runningEffects.contains(effect) && _runningEffects[effect];
    }
}

void EffectManager::updateDevicePositions()
{
    // Sync spatial zones with device positions from the grid
    if (!_spatialGrid) return;
    
    for (auto* zone : _spatialZones)
    {
        // Look up the current position in the grid
        if (zone->deviceIndex >= 0 && zone->zoneIndex >= 0)
        {
            DeviceInfo deviceInfo;
            deviceInfo.index = zone->deviceIndex;
            deviceInfo.zoneIndex = zone->zoneIndex;
            
            std::optional<GridPosition> pos = _spatialGrid->GetDevicePosition(deviceInfo);
            if (pos.has_value())
            {
                zone->position = pos.value();
            }
        }
    }
}

} // namespace Lightscape
