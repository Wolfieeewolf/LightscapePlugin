#include "core/DeviceManager.h"
#include "RGBController.h"
#include <climits>

DeviceManager::DeviceManager(ResourceManager* resourceManager, QObject* parent) :
    QObject(parent),
    resourceManager(resourceManager),
    currentDeviceIndex(-1)
{
}

void DeviceManager::SetError(const QString& error) const
{
    lastError = error;
    emit const_cast<DeviceManager*>(this)->errorOccurred(error);
}

bool DeviceManager::ValidateDeviceIndex(int deviceIndex) const
{
    if (!resourceManager)
    {
        SetError("No resource manager available");
        return false;
    }
    
    if (deviceIndex < 0 || static_cast<unsigned int>(deviceIndex) >= GetDeviceCount())
    {
        SetError(QString("Invalid device index: %1").arg(deviceIndex));
        return false;
    }
    
    return true;
}

bool DeviceManager::ValidateZoneIndex(int deviceIndex, int zoneIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex))
        return false;
    
    if (zoneIndex < 0 || static_cast<size_t>(zoneIndex) >= GetZoneCount(deviceIndex))
    {
        SetError(QString("Invalid zone index: %1").arg(zoneIndex));
        return false;
    }
    
    return true;
}

bool DeviceManager::ValidateLEDIndex(int deviceIndex, int ledIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex))
        return false;
    
    if (ledIndex < 0 || static_cast<size_t>(ledIndex) >= GetLEDCount(deviceIndex))
    {
        SetError(QString("Invalid LED index: %1").arg(ledIndex));
        return false;
    }
    
    return true;
}

unsigned int DeviceManager::GetDeviceCount() const
{
    if (!resourceManager)
        return 0;
    auto count = resourceManager->GetRGBControllers().size();
    if (count > UINT_MAX)
    {
        SetError("Device count exceeds maximum supported value");
        return UINT_MAX;
    }
    return static_cast<unsigned int>(count);
}

QString DeviceManager::GetDeviceName(unsigned int index) const
{
    if (!ValidateDeviceIndex(index))
        return QString();
    
    return QString::fromStdString(resourceManager->GetRGBControllers()[index]->name);
}

size_t DeviceManager::GetZoneCount(int deviceIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex))
        return 0;
    
    return resourceManager->GetRGBControllers()[deviceIndex]->zones.size();
}

QString DeviceManager::GetZoneName(int deviceIndex, int zoneIndex) const
{
    if (!ValidateZoneIndex(deviceIndex, zoneIndex))
        return QString();
    
    return QString::fromStdString(resourceManager->GetRGBControllers()[deviceIndex]->zones[zoneIndex].name);
}

size_t DeviceManager::GetLEDCount(int deviceIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex))
        return 0;
    
    return resourceManager->GetRGBControllers()[deviceIndex]->leds.size();
}

QString DeviceManager::GetLEDName(int deviceIndex, int ledIndex) const
{
    if (!ValidateLEDIndex(deviceIndex, ledIndex))
        return QString();
    
    return QString::fromStdString(resourceManager->GetRGBControllers()[deviceIndex]->leds[ledIndex].name);
}

bool DeviceManager::SetLEDColor(int deviceIndex, int ledIndex, RGBColor color)
{
    if (!ValidateLEDIndex(deviceIndex, ledIndex))
        return false;
    
    try
    {
        auto controller = resourceManager->GetRGBControllers()[deviceIndex];
        controller->SetLED(static_cast<unsigned int>(ledIndex), color);
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to set LED color: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::SetZoneColor(int deviceIndex, int zoneIndex, RGBColor color)
{
    if (!ValidateZoneIndex(deviceIndex, zoneIndex))
        return false;
    
    try
    {
        auto controller = resourceManager->GetRGBControllers()[deviceIndex];
        auto& zone = controller->zones[zoneIndex];
        
        for (unsigned int i = 0; i < zone.leds_count; i++)
        {
            controller->SetLED(zone.start_idx + i, color);
        }
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to set zone color: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::SetDeviceColor(int deviceIndex, RGBColor color)
{
    if (!ValidateDeviceIndex(deviceIndex))
        return false;
    
    try
    {
        auto controller = resourceManager->GetRGBControllers()[deviceIndex];
        auto ledCount = controller->leds.size();
        if (ledCount > UINT_MAX)
        {
            SetError("LED count exceeds maximum supported value");
            return false;
        }
        
        for (unsigned int i = 0; i < static_cast<unsigned int>(ledCount); i++)
        {
            controller->SetLED(i, color);
        }
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to set device color: %1").arg(e.what()));
        return false;
    }
}

bool DeviceManager::UpdateDevice(int deviceIndex)
{
    if (!ValidateDeviceIndex(deviceIndex))
        return false;
    
    try
    {
        resourceManager->GetRGBControllers()[deviceIndex]->UpdateLEDs();
        emit deviceUpdated(deviceIndex);
        return true;
    }
    catch (const std::exception& e)
    {
        SetError(QString("Failed to update device: %1").arg(e.what()));
        return false;
    }
}