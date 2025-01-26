#pragma once

#include <QString>
#include <QObject>
#include "ResourceManager.h"
#include "RGBController.h"

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(ResourceManager* resourceManager = nullptr, QObject* parent = nullptr);
    virtual ~DeviceManager() {}
    
    unsigned int GetDeviceCount() const;
    QString GetDeviceName(unsigned int index) const;
    size_t GetZoneCount(int deviceIndex) const;
    QString GetZoneName(int deviceIndex, int zoneIndex) const;
    size_t GetLEDCount(int deviceIndex) const;
    QString GetLEDName(int deviceIndex, int ledIndex) const;

    int GetCurrentDeviceIndex() const { return currentDeviceIndex; }
    QString GetCurrentSelectionName() const { return currentSelectionName; }
    
    void SetCurrentDeviceIndex(int index) { currentDeviceIndex = index; }
    void SetCurrentSelectionName(const QString& name) { currentSelectionName = name; }

    // Device Control
    bool SetLEDColor(int deviceIndex, int ledIndex, RGBColor color);
    bool SetZoneColor(int deviceIndex, int zoneIndex, RGBColor color);
    bool SetDeviceColor(int deviceIndex, RGBColor color);
    bool UpdateDevice(int deviceIndex);

Q_SIGNALS:
    void deviceListChanged();
    void deviceUpdated(int deviceIndex);
    void errorOccurred(const QString& error);

private:
    ResourceManager* resourceManager;
    int currentDeviceIndex;
    QString currentSelectionName;
    mutable QString lastError;

    bool ValidateDeviceIndex(int deviceIndex) const;
    bool ValidateZoneIndex(int deviceIndex, int zoneIndex) const;
    bool ValidateLEDIndex(int deviceIndex, int ledIndex) const;
    void SetError(const QString& error) const;
};