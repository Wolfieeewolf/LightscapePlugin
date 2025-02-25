#include "core/DeviceManager.h"

DeviceManager::DeviceManager(ResourceManager* resourceManager, QObject* parent)
    : QObject(parent)
    , resourceManager(resourceManager)
    , currentDeviceIndex(-1)
    , currentDeviceType(Lightscape::DeviceType::RGB)
{
}

QString DeviceManager::GetDeviceName(unsigned int index, Lightscape::DeviceType type) const
{
    switch (type) {
        case Lightscape::DeviceType::RGB:
            return GetRGBDeviceName(index);
        case Lightscape::DeviceType::NonRGB:
            return GetNonRGBDeviceName(index);
    }
    return QString();
}

unsigned int DeviceManager::GetDeviceCount(Lightscape::DeviceType type) const
{
    switch (type) {
        case Lightscape::DeviceType::RGB:
            return GetRGBDeviceCount();
        case Lightscape::DeviceType::NonRGB:
            return GetNonRGBDeviceCount();
    }
    return 0;
}

unsigned int DeviceManager::GetRGBDeviceCount() const
{
    if (!resourceManager) return 0;
    return static_cast<unsigned int>(resourceManager->GetRGBControllers().size());
}

QString DeviceManager::GetRGBDeviceName(unsigned int index) const
{
    if (!ValidateDeviceIndex(index, Lightscape::DeviceType::RGB)) return QString();
    
    auto& controllers = resourceManager->GetRGBControllers();
    if (index < controllers.size()) {
        return QString::fromStdString(controllers[index]->name);
    }
    return QString();
}

size_t DeviceManager::GetZoneCount(int deviceIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex, Lightscape::DeviceType::RGB)) return 0;
    
    auto& controllers = resourceManager->GetRGBControllers();
    if (static_cast<size_t>(deviceIndex) < controllers.size()) {
        return controllers[deviceIndex]->zones.size();
    }
    return 0;
}

QString DeviceManager::GetZoneName(int deviceIndex, int zoneIndex) const
{
    if (!ValidateZoneIndex(deviceIndex, zoneIndex)) return QString();
    
    auto& controllers = resourceManager->GetRGBControllers();
    if (static_cast<size_t>(deviceIndex) < controllers.size()) {
        return QString::fromStdString(controllers[deviceIndex]->zones[zoneIndex].name);
    }
    return QString();
}

size_t DeviceManager::GetLEDCount(int deviceIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex, Lightscape::DeviceType::RGB)) return 0;
    
    auto& controllers = resourceManager->GetRGBControllers();
    if (static_cast<size_t>(deviceIndex) < controllers.size()) {
        return controllers[deviceIndex]->leds.size();
    }
    return 0;
}

QString DeviceManager::GetLEDName(int deviceIndex, int ledIndex) const
{
    if (!ValidateLEDIndex(deviceIndex, ledIndex)) return QString();
    
    auto& controllers = resourceManager->GetRGBControllers();
    if (static_cast<size_t>(deviceIndex) < controllers.size()) {
        return QString::fromStdString(controllers[deviceIndex]->leds[ledIndex].name);
    }
    return QString();
}

bool DeviceManager::SetLEDColor(int deviceIndex, int ledIndex, RGBColor color)
{
    if (!ValidateLEDIndex(deviceIndex, ledIndex)) return false;

    try {
        auto& controllers = resourceManager->GetRGBControllers();
        if (static_cast<size_t>(deviceIndex) < controllers.size()) {
            auto controller = controllers[deviceIndex];
            
            // Update the colors array only
            controller->colors[ledIndex] = color;
            
            // Let the controller handle LED updating
            controller->UpdateSingleLED(ledIndex);
            return true;
        }
        return false;
    }
    catch (...) {
        SetError("Failed to set LED color");
        return false;
    }
}

bool DeviceManager::SetZoneColor(int deviceIndex, int zoneIndex, RGBColor color)
{
    if (!ValidateZoneIndex(deviceIndex, zoneIndex)) return false;

    try {
        auto& controllers = resourceManager->GetRGBControllers();
        if (static_cast<size_t>(deviceIndex) < controllers.size()) {
            auto controller = controllers[deviceIndex];
            
            // Set the colors in the colors array
            auto& zone = controller->zones[zoneIndex];
            unsigned int start = zone.start_idx;
            
            for (unsigned int i = 0; i < zone.leds_count; i++) {
                controller->colors[start + i] = color;
            }
            
            // Update the zone
            controller->UpdateZoneLEDs(zoneIndex);
            return true;
        }
        return false;
    }
    catch (...) {
        SetError("Failed to set zone color");
        return false;
    }
}

bool DeviceManager::SetDeviceColor(int deviceIndex, RGBColor color)
{
    if (!ValidateDeviceIndex(deviceIndex, Lightscape::DeviceType::RGB)) return false;

    try {
        auto& controllers = resourceManager->GetRGBControllers();
        if (static_cast<size_t>(deviceIndex) < controllers.size()) {
            auto controller = controllers[deviceIndex];
            
            // Set all colors in the colors array
            for (unsigned int i = 0; i < controller->colors.size(); i++) {
                controller->colors[i] = color;
            }
            
            // Update all LEDs
            controller->UpdateLEDs();
            return true;
        }
        return false;
    }
    catch (...) {
        SetError("Failed to set device color");
        return false;
    }
}

bool DeviceManager::UpdateDevice(int deviceIndex)
{
    if (!ValidateDeviceIndex(deviceIndex, Lightscape::DeviceType::RGB)) return false;

    try {
        auto& controllers = resourceManager->GetRGBControllers();
        if (static_cast<size_t>(deviceIndex) < controllers.size()) {
            auto controller = controllers[deviceIndex];
            controller->UpdateLEDs();
            emit deviceUpdated(deviceIndex, Lightscape::DeviceType::RGB);
            return true;
        }
        return false;
    }
    catch (...) {
        SetError("Failed to update device");
        return false;
    }
}

unsigned int DeviceManager::GetNonRGBDeviceCount() const
{
    return nonRGBDevices.size();
}

QString DeviceManager::GetNonRGBDeviceName(unsigned int index) const
{
    if (!ValidateDeviceIndex(index, Lightscape::DeviceType::NonRGB)) return QString();
    return nonRGBDevices[index]->getName();
}

void DeviceManager::AddNonRGBDevice(NonRGBDevice* device)
{
    if (!device) return;
    
    for (const auto& existing : nonRGBDevices) {
        if (existing->getName() == device->getName()) {
            SetError("A device with this name already exists");
            return;
        }
    }

    nonRGBDevices.append(device);
    emit nonRGBDeviceAdded(device);
    emit deviceListChanged();
}

void DeviceManager::RemoveNonRGBDevice(const QString& name)
{
    for (int i = 0; i < nonRGBDevices.size(); ++i) {
        if (nonRGBDevices[i]->getName() == name) {
            NonRGBDevice* device = nonRGBDevices.takeAt(i);
            emit nonRGBDeviceRemoved(name);
            emit deviceListChanged();
            delete device;
            return;
        }
    }
}

NonRGBDevice* DeviceManager::GetNonRGBDevice(unsigned int index) const
{
    if (!ValidateDeviceIndex(index, Lightscape::DeviceType::NonRGB)) return nullptr;
    return nonRGBDevices[index];
}

QList<unsigned int> DeviceManager::GetUnassignedDevices(Lightscape::DeviceType type) const
{
    QList<unsigned int> unassignedDevices;
    unsigned int count = GetDeviceCount(type);
    
    for (unsigned int i = 0; i < count; i++)
    {
        if (!IsDeviceAssigned(i, type))
        {
            unassignedDevices.append(i);
        }
    }
    
    return unassignedDevices;
}

bool DeviceManager::IsDeviceAssigned(unsigned int index, Lightscape::DeviceType type) const
{
    QPair<unsigned int, Lightscape::DeviceType> key(index, type);
    return deviceAssignments.contains(key) && deviceAssignments[key].isAssigned;
}

void DeviceManager::SetDeviceAssignment(unsigned int index, Lightscape::DeviceType type,
                                      const QString& layer, const GridPosition& position)
{
    if (!ValidateDeviceIndex(index, type))
    {
        SetError("Invalid device index for assignment");
        return;
    }

    QPair<unsigned int, Lightscape::DeviceType> key(index, type);
    DeviceAssignment& assignment = deviceAssignments[key];
    assignment.layer = layer;
    assignment.position = position;
    assignment.isAssigned = true;

    emit deviceAssignmentChanged(index, type, true);
    emit deviceListChanged();
}

void DeviceManager::RemoveDeviceAssignment(unsigned int index, Lightscape::DeviceType type)
{
    QPair<unsigned int, Lightscape::DeviceType> key(index, type);
    if (deviceAssignments.contains(key))
    {
        deviceAssignments[key].isAssigned = false;
        emit deviceAssignmentChanged(index, type, false);
        emit deviceListChanged();
    }
}

bool DeviceManager::GetDeviceAssignmentInfo(unsigned int index, Lightscape::DeviceType type,
                                          QString& layer, GridPosition& position) const
{
    QPair<unsigned int, Lightscape::DeviceType> key(index, type);
    if (deviceAssignments.contains(key) && deviceAssignments[key].isAssigned)
    {
        const DeviceAssignment& assignment = deviceAssignments[key];
        layer = assignment.layer;
        position = assignment.position;
        return true;
    }
    return false;
}

bool DeviceManager::ValidateDeviceIndex(int deviceIndex, Lightscape::DeviceType type) const
{
    if (deviceIndex < 0)
    {
        SetError("Invalid device index");
        return false;
    }

    unsigned int count = GetDeviceCount(type);
    if (static_cast<unsigned int>(deviceIndex) >= count)
    {
        SetError("Device index out of range");
        return false;
    }

    return true;
}

bool DeviceManager::ValidateZoneIndex(int deviceIndex, int zoneIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex, Lightscape::DeviceType::RGB)) return false;

    if (zoneIndex < 0 || static_cast<size_t>(zoneIndex) >= GetZoneCount(deviceIndex))
    {
        SetError("Zone index out of range");
        return false;
    }

    return true;
}

bool DeviceManager::ValidateLEDIndex(int deviceIndex, int ledIndex) const
{
    if (!ValidateDeviceIndex(deviceIndex, Lightscape::DeviceType::RGB)) return false;

    if (ledIndex < 0 || static_cast<size_t>(ledIndex) >= GetLEDCount(deviceIndex))
    {
        SetError("LED index out of range");
        return false;
    }

    return true;
}

void DeviceManager::SetError(const QString& error) const
{
    lastError = error;
    emit const_cast<DeviceManager*>(this)->errorOccurred(error);
}