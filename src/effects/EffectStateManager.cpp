/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectStateManager.cpp                                    |
|                                                           |
| Centralized manager for effect state                      |
\*---------------------------------------------------------*/

#include "effects/EffectStateManager.h"
#include "effects/EffectManager.h"

namespace Lightscape {

EffectStateManager* EffectStateManager::_instance = nullptr;

EffectStateManager& EffectStateManager::getInstance()
{
    if (!_instance) {
        _instance = new EffectStateManager();
    }
    return *_instance;
}

EffectStateManager::EffectStateManager()
{
    printf("[Lightscape] EffectStateManager initialized\n");
}

EffectStateManager::~EffectStateManager()
{
    printf("[Lightscape] EffectStateManager destroyed\n");
}

void EffectStateManager::setSelectedEffect(BaseEffect* effect)
{
    if (_selectedEffect == effect) return;
    
    _selectedEffect = effect;
    printf("[Lightscape] Selected effect changed: %s\n", 
           (effect ? effect->GetStaticInfo().name.toStdString().c_str() : "none"));
    
    emit selectedEffectChanged(effect);
}

BaseEffect* EffectStateManager::getSelectedEffect() const
{
    return _selectedEffect;
}

void EffectStateManager::setEffectRunning(BaseEffect* effect, bool running)
{
    if (!effect) return;
    
    // Check if state is actually changing
    if (_runningEffects.contains(effect) && _runningEffects[effect] == running) {
        printf("[Lightscape][StateManager] Effect already in requested state: %s - %s\n",
               effect->GetStaticInfo().name.toStdString().c_str(),
               running ? "Running" : "Stopped");
        return;
    }
    
    printf("[Lightscape][StateManager] Setting effect running state: %s -> %s\n", 
           effect->GetStaticInfo().name.toStdString().c_str(), 
           running ? "Running" : "Stopped");
    
    // Update our state tracking immediately to avoid race conditions
    _runningEffects[effect] = running;
    
    // Update the actual effect object
    effect->setEnabled(running);
    
    // Use the EffectManager to handle the actual effect processing
    auto& effectManager = EffectManager::getInstance();
    bool success = false;
    
    if (running) {
        // First, make sure devices are assigned to the effect manager before starting
        if (_effectDevices.contains(effect)) {
            QList<DeviceInfo> devices = _effectDevices[effect];
            printf("[Lightscape][StateManager] Setting %d devices for running effect\n", devices.size());
            effectManager.setActiveDevicesForEffect(effect, devices);
        } else {
            printf("[Lightscape][StateManager] WARNING: No devices assigned to effect\n");
        }
        
        // Start effect
        bool started = effectManager.startEffect(effect->GetStaticInfo().id, effect);
        if (!started) {
            // Revert state if start failed
            _runningEffects[effect] = false;
            effect->setEnabled(false);
            running = false;
            printf("[Lightscape][StateManager] WARNING: Failed to start effect: %s\n", 
                   effect->GetStaticInfo().name.toStdString().c_str());
        } else {
            // Force an immediate color update after starting
            if (_effectDevices.contains(effect)) {
                QList<DeviceInfo> devices = _effectDevices[effect];
                printf("[Lightscape][StateManager] Forcing immediate color update for %d devices\n", devices.size());
                effect->applyToDevices(devices);
                success = true;
            }
        }
    } else {
        // Stop effect
        effectManager.stopEffect(effect);
        success = true; // Stopping typically doesn't fail
    }
    
    printf("[Lightscape][StateManager] Effect %s setting to %s: %s\n", 
           effect->GetStaticInfo().name.toStdString().c_str(),
           running ? "Running" : "Stopped",
           success ? "SUCCESS" : "FAILED");
    
    // Signal the change - important for UI updates!
    emit effectRunningStateChanged(effect, running);
    
    // Request preview update
    updatePreview();
}

bool EffectStateManager::isEffectRunning(BaseEffect* effect) const
{
    if (!effect) return false;
    return _runningEffects.value(effect, false);
}

void EffectStateManager::setDevicesForEffect(BaseEffect* effect, const QList<DeviceInfo>& devices)
{
    if (!effect) return;
    
    // Store the device information
    _effectDevices[effect] = devices;
    
    printf("[Lightscape][StateManager] Set %d devices for effect: %s\n", 
           devices.size(), 
           effect->GetStaticInfo().name.toStdString().c_str());
    
    // Print device details for debugging
    for (int i = 0; i < devices.size(); i++) {
        const DeviceInfo& device = devices[i];
        printf("[Lightscape][StateManager] Device %d: index=%d, pos=(%d,%d,%d)\n", 
               i, device.index, device.position.x, device.position.y, device.position.z);
    }
    
    // If the effect is running, update the active devices
    if (isEffectRunning(effect)) {
        printf("[Lightscape][StateManager] Effect is running, updating active devices immediately\n");
        EffectManager::getInstance().setActiveDevicesForEffect(effect, devices);
    } else {
        printf("[Lightscape][StateManager] Effect is NOT running, devices will be applied when started\n");
    }
    
    // Signal the change
    emit effectDevicesChanged(effect, devices);
}

QList<DeviceInfo> EffectStateManager::getDevicesForEffect(BaseEffect* effect) const
{
    if (!effect) return QList<DeviceInfo>();
    return _effectDevices.value(effect, QList<DeviceInfo>());
}

void EffectStateManager::addEffect(BaseEffect* effect)
{
    if (!effect || _effects.contains(effect)) {
        printf("[Lightscape][StateManager] Cannot add effect: %s (nullptr or already exists)\n",
               effect ? effect->GetStaticInfo().name.toStdString().c_str() : "NULL");
        return;
    }
    
    // Add to our list
    _effects.append(effect);
    
    // Initialize state - always starts disabled
    _runningEffects[effect] = false;
    effect->setEnabled(false);
    effect->stop(); // Ensure it's really stopped
    
    EffectInfo info = effect->GetStaticInfo();
    printf("[Lightscape][StateManager] Added effect: %s (ID: %s)\n", 
           info.name.toStdString().c_str(),
           info.id.toStdString().c_str());
    
    // Signal addition
    emit effectAdded(effect);
    
    // If this is the first effect, select it
    if (_effects.size() == 1) {
        printf("[Lightscape][StateManager] Setting first effect as selected: %s\n",
               info.name.toStdString().c_str());
        setSelectedEffect(effect);
    }
}

void EffectStateManager::removeEffect(BaseEffect* effect)
{
    if (!effect || !_effects.contains(effect)) return;
    
    // Stop first if running
    if (isEffectRunning(effect)) {
        setEffectRunning(effect, false);
    }
    
    // Remove from our lists
    _effects.removeOne(effect);
    _runningEffects.remove(effect);
    _effectDevices.remove(effect);
    
    printf("[Lightscape] Removed effect: %s\n", 
           effect->GetStaticInfo().name.toStdString().c_str());
    
    // If this was the selected effect, clear or change selection
    if (_selectedEffect == effect) {
        setSelectedEffect(_effects.isEmpty() ? nullptr : _effects.first());
    }
    
    // Signal removal
    emit effectRemoved(effect);
}

QList<BaseEffect*> EffectStateManager::getAllEffects() const
{
    return _effects;
}

void EffectStateManager::updatePreview()
{
    emit previewUpdateRequested();
}

} // namespace Lightscape