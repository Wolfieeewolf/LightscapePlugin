#include "../include/DeviceManager.h"
#include <QtGlobal>

DeviceManager::DeviceManager(ResourceManager* manager, QObject* parent)
    : QObject(parent)
    , resource_manager(manager)
{
}

size_t DeviceManager::GetDeviceCount() const
{
    if (!resource_manager)
    {
        SetError("No resource manager available");
        return 0;
    }
    return resource_manager->GetRGBControllers().size();
}

QString DeviceManager::GetDeviceName(unsigned int device_index) const
{
    if (!ValidateDeviceIndex(device_index))
        return QString();
    return QString::fromStdString(resource_manager->GetRGBControllers()[device_index]->name);
}

QString DeviceManager::GetZoneName(unsigned int device_index, int zone_index) const
{
    if (!ValidateZoneIndex(device_index, zone_index))
        return QString();
    return QString::fromStdString(resource_manager->GetRGBControllers()[device_index]->zones[zone_index].name);
}

QString DeviceManager::GetLEDName(unsigned int device_index, int led_index) const
{
    if (!ValidateLEDIndex(device_index, led_index))
        return QString();
    return QString::fromStdString(resource_manager->GetRGBControllers()[device_index]->leds[led_index].name);
}

size_t DeviceManager::GetZoneCount(unsigned int device_index) const
{
    if (!ValidateDeviceIndex(device_index))
        return 0;
    return resource_manager->GetRGBControllers()[device_index]->zones.size();
}

size_t DeviceManager::GetLEDCount(unsigned int device_index) const
{
    if (!ValidateDeviceIndex(device_index))
        return 0;
    return resource_manager->GetRGBControllers()[device_index]->leds.size();
}

size_t DeviceManager::GetZoneLEDCount(unsigned int device_index, int zone_index) const
{
    if (!ValidateZoneIndex(device_index, zone_index))
        return 0;
    return resource_manager->GetRGBControllers()[device_index]->zones[zone_index].leds_count;
}

bool DeviceManager::SetLEDColor(unsigned int device_index, int led_index, RGBColor color)
{
    if (!ValidateLEDIndex(device_index, led_index))
        return false;
    
    try
    {
        auto controller = resource_manager->GetRGBControllers()[device_index];
        controller->SetLED(static_cast<unsigned int>(led_index), color);
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to set LED color: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::SetZoneColor(unsigned int device_index, int zone_index, RGBColor color)
{
    if (!ValidateZoneIndex(device_index, zone_index))
        return false;
    
    try
    {
        auto controller = resource_manager->GetRGBControllers()[device_index];
        auto& zone = controller->zones[zone_index];
        
        for (size_t i = 0; i < zone.leds_count; i++)
        {
            controller->SetLED(static_cast<unsigned int>(zone.start_idx + i), color);
        }
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to set zone color: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::SetDeviceColor(unsigned int device_index, RGBColor color)
{
    if (!ValidateDeviceIndex(device_index))
        return false;
    
    try
    {
        auto controller = resource_manager->GetRGBControllers()[device_index];
        
        for (size_t i = 0; i < controller->leds.size(); i++)
        {
            controller->SetLED(static_cast<unsigned int>(i), color);
        }
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to set device color: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::UpdateDevice(unsigned int device_index)
{
    if (!ValidateDeviceIndex(device_index))
        return false;
    
    try
    {
        resource_manager->GetRGBControllers()[device_index]->UpdateLEDs();
        emit deviceUpdated(device_index);
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to update device: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::SetResourceManager(ResourceManager* manager)
{
    if (resource_manager != manager)
    {
        resource_manager = manager;
        emit deviceListChanged();
    }
    return true;
}

bool DeviceManager::ValidateDeviceIndex(unsigned int device_index) const
{
    if (!resource_manager)
    {
        SetError("No resource manager available");
        return false;
    }
    
    if (device_index >= GetDeviceCount())
    {
        SetError(QString("Invalid device index: %1").arg(device_index));
        return false;
    }
    
    return true;
}

bool DeviceManager::ValidateZoneIndex(unsigned int device_index, int zone_index) const
{
    if (!ValidateDeviceIndex(device_index))
        return false;
    
    if (zone_index < 0 || static_cast<size_t>(zone_index) >= GetZoneCount(device_index))
    {
        SetError(QString("Invalid zone index: %1").arg(zone_index));
        return false;
    }
    
    return true;
}

bool DeviceManager::ValidateLEDIndex(unsigned int device_index, int led_index) const
{
    if (!ValidateDeviceIndex(device_index))
        return false;
    
    if (led_index < 0 || static_cast<size_t>(led_index) >= GetLEDCount(device_index))
    {
        SetError(QString("Invalid LED index: %1").arg(led_index));
        return false;
    }
    
    return true;
}