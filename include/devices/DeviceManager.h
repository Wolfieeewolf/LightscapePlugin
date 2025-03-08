#pragma once

#include <QString>
#include <QObject>
#include <QList>
#include <QMap>
#include <QPair>
#include "ResourceManager.h"
#include "RGBController.h"
#include "core/Types.h"
#include "devices/NonRGBDevice.h"
#include "grid/GridTypes.h"

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(ResourceManager* resourceManager = nullptr, QObject* parent = nullptr);
    virtual ~DeviceManager() {}

    // Resource Manager Access (needed by DeviceEffectManager)
    ResourceManager* resourceManager;

    // Basic Device Methods
    QString GetDeviceName(unsigned int index, Lightscape::DeviceType type) const;
    unsigned int GetDeviceCount(Lightscape::DeviceType type) const;

    // RGB Device Control Methods
    unsigned int GetRGBDeviceCount() const;
    QString GetRGBDeviceName(unsigned int index) const;
    size_t GetZoneCount(int deviceIndex) const;
    QString GetZoneName(int deviceIndex, int zoneIndex) const;
    size_t GetLEDCount(int deviceIndex) const;
    QString GetLEDName(int deviceIndex, int ledIndex) const;
    bool SetLEDColor(int deviceIndex, int ledIndex, RGBColor color);
    bool SetZoneColor(int deviceIndex, int zoneIndex, RGBColor color);
    bool SetDeviceColor(int deviceIndex, RGBColor color);
    bool UpdateDevice(int deviceIndex);

    // Non-RGB Device Methods
    unsigned int GetNonRGBDeviceCount() const;
    QString GetNonRGBDeviceName(unsigned int index) const;
    void AddNonRGBDevice(NonRGBDevice* device);
    void RemoveNonRGBDevice(const QString& name);
    NonRGBDevice* GetNonRGBDevice(unsigned int index) const;
    QList<NonRGBDevice*> GetNonRGBDevices() const { return nonRGBDevices; }

    // Assignment Methods
    bool IsDeviceAssigned(unsigned int index, Lightscape::DeviceType type) const;
    void SetDeviceAssignment(unsigned int index, Lightscape::DeviceType type, 
                           const QString& layer, const GridPosition& position);
    void RemoveDeviceAssignment(unsigned int index, Lightscape::DeviceType type);
    bool GetDeviceAssignmentInfo(unsigned int index, Lightscape::DeviceType type,
                                QString& layer, GridPosition& position) const;
    QList<unsigned int> GetUnassignedDevices(Lightscape::DeviceType type) const;

    // Current Selection Management
    int GetCurrentDeviceIndex() const { return currentDeviceIndex; }
    QString GetCurrentSelectionName() const { return currentSelectionName; }
    Lightscape::DeviceType GetCurrentDeviceType() const { return currentDeviceType; }
    
    void SetCurrentDeviceIndex(int index) { currentDeviceIndex = index; }
    void SetCurrentSelectionName(const QString& name) { currentSelectionName = name; }
    void SetCurrentDeviceType(Lightscape::DeviceType type) { currentDeviceType = type; }
    
    // Validation method (public for use by SettingsManager)
    bool ValidateDeviceIndex(int deviceIndex, Lightscape::DeviceType type) const;

signals:
    void deviceListChanged();
    void deviceUpdated(int deviceIndex, Lightscape::DeviceType type);
    void errorOccurred(const QString& error);
    void nonRGBDeviceAdded(NonRGBDevice* device);
    void nonRGBDeviceRemoved(const QString& deviceName);
    void nonRGBDeviceModified(NonRGBDevice* device);
    void deviceAssignmentChanged(unsigned int index, Lightscape::DeviceType type, bool isAssigned);

private:
    struct DeviceAssignment {
        QString layer;
        GridPosition position;
        bool isAssigned;
    };

    QList<NonRGBDevice*> nonRGBDevices;
    QMap<QPair<unsigned int, Lightscape::DeviceType>, DeviceAssignment> deviceAssignments;
    int currentDeviceIndex;
    QString currentSelectionName;
    Lightscape::DeviceType currentDeviceType;
    mutable QString lastError;
    bool ValidateZoneIndex(int deviceIndex, int zoneIndex) const;
    bool ValidateLEDIndex(int deviceIndex, int ledIndex) const;
    void SetError(const QString& error) const;
};