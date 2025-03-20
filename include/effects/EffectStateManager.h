/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectStateManager.h                                      |
|                                                           |
| Centralized manager for effect state                      |
\*---------------------------------------------------------*/

#pragma once

#include <QObject>
#include <QMap>
#include "effects/BaseEffect.h"
#include "devices/DeviceManager.h"

namespace Lightscape {

class EffectStateManager : public QObject
{
    Q_OBJECT

public:
    static EffectStateManager& getInstance();
    
    // Effect selection
    void setSelectedEffect(BaseEffect* effect);
    BaseEffect* getSelectedEffect() const;
    
    // Effect running state
    void setEffectRunning(BaseEffect* effect, bool running);
    bool isEffectRunning(BaseEffect* effect) const;
    
    // Device assignment
    void setDevicesForEffect(BaseEffect* effect, const QList<DeviceInfo>& devices);
    QList<DeviceInfo> getDevicesForEffect(BaseEffect* effect) const;
    
    // Effect list management
    void addEffect(BaseEffect* effect);
    void removeEffect(BaseEffect* effect);
    QList<BaseEffect*> getAllEffects() const;
    
    // Preview management
    void updatePreview();
    
signals:
    void selectedEffectChanged(BaseEffect* effect);
    void effectRunningStateChanged(BaseEffect* effect, bool running);
    void effectDevicesChanged(BaseEffect* effect, const QList<DeviceInfo>& devices);
    void effectAdded(BaseEffect* effect);
    void effectRemoved(BaseEffect* effect);
    void previewUpdateRequested();
    
private:
    EffectStateManager();
    ~EffectStateManager();
    
    static EffectStateManager* _instance;
    
    BaseEffect* _selectedEffect = nullptr;
    QMap<BaseEffect*, bool> _runningEffects;
    QMap<BaseEffect*, QList<DeviceInfo>> _effectDevices;
    QList<BaseEffect*> _effects;
};

} // namespace Lightscape