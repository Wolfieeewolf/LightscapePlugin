#pragma once

#include <QString>
#include <QObject>
#include "ResourceManager.h"
#include "RGBController.h"

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(ResourceManager* manager = nullptr, QObject* parent = nullptr);
    
    // Device Information
    size_t GetDeviceCount() const;
    QString GetDeviceName(unsigned int device_index) const;
    QString GetZoneName(unsigned int device_index, int zone_index) const;
    QString GetLEDName(unsigned int device_index, int led_index) const;
    
    // Device Properties
    size_t GetZoneCount(unsigned int device_index) const;
    size_t GetLEDCount(unsigned int device_index) const;
    size_t GetZoneLEDCount(unsigned int device_index, int zone_index) const;
    
    // Device Control
    bool SetLEDColor(unsigned int device_index, int led_index, RGBColor color);
    bool SetZoneColor(unsigned int device_index, int zone_index, RGBColor color);
    bool SetDeviceColor(unsigned int device_index, RGBColor color);
    bool UpdateDevice(unsigned int device_index);
    
    // Resource Management
    bool SetResourceManager(ResourceManager* manager);
    bool IsValid() const { return resource_manager != nullptr; }
    
    // Error Handling
    QString GetLastError() const { return last_error; }

signals:
    void deviceListChanged();
    void deviceUpdated(unsigned int device_index);
    void errorOccurred(const QString& error);

private:
    ResourceManager* resource_manager;
    mutable QString last_error;

    // Validation helpers
    bool ValidateDeviceIndex(unsigned int device_index) const;
    bool ValidateZoneIndex(unsigned int device_index, int zone_index) const;
    bool ValidateLEDIndex(unsigned int device_index, int led_index) const;
    
    // Error handling helper
    void SetError(const QString& error) const { 
        last_error = error; 
        const_cast<DeviceManager*>(this)->errorOccurred(error);
    }
};