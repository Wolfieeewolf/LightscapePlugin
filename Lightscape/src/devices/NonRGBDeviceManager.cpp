#include "devices/NonRGBDeviceManager.h"

NonRGBDeviceManager::NonRGBDeviceManager(QObject* parent)
    : QObject(parent)
{
}

NonRGBDeviceManager::~NonRGBDeviceManager()
{
    // Clean up all devices
    qDeleteAll(devices);
    devices.clear();
}

bool NonRGBDeviceManager::addDevice(NonRGBDevice* device)
{
    if (!device) {
        emit errorOccurred("Attempt to add null device");
        return false;
    }

    QString name = device->getName();
    if (name.isEmpty()) {
        emit errorOccurred("Device must have a name");
        return false;
    }

    if (devices.contains(name)) {
        emit errorOccurred("Device with name '" + name + "' already exists");
        return false;
    }

    devices[name] = device;
    emit deviceAdded(device);
    return true;
}

bool NonRGBDeviceManager::removeDevice(const QString& name)
{
    NonRGBDevice* device = devices.take(name);
    if (device) {
        emit deviceRemoved(name);
        delete device;
        return true;
    }
    return false;
}

NonRGBDevice* NonRGBDeviceManager::getDevice(const QString& name) const
{
    return devices.value(name, nullptr);
}

QVector<NonRGBDevice*> NonRGBDeviceManager::getAllDevices() const
{
    return QVector<NonRGBDevice*>::fromList(devices.values());
}

QVector<NonRGBDevice*> NonRGBDeviceManager::getDevicesByType(NonRGBDeviceType type) const
{
    QVector<NonRGBDevice*> result;
    for (NonRGBDevice* device : devices) {
        if (device->getType() == type) {
            result.append(device);
        }
    }
    return result;
}

QStringList NonRGBDeviceManager::getDeviceNames() const
{
    return devices.keys();
}

bool NonRGBDeviceManager::hasDevice(const QString& name) const
{
    return devices.contains(name);
}

int NonRGBDeviceManager::getDeviceCount() const
{
    return devices.size();
}

int NonRGBDeviceManager::getDeviceCountByType(NonRGBDeviceType type) const
{
    int count = 0;
    for (NonRGBDevice* device : devices) {
        if (device->getType() == type) {
            count++;
        }
    }
    return count;
}