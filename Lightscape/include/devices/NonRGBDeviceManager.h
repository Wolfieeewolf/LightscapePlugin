#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include "devices/NonRGBDevice.h"

class NonRGBDeviceManager : public QObject {
    Q_OBJECT

public:
    explicit NonRGBDeviceManager(QObject* parent = nullptr);
    ~NonRGBDeviceManager();
    
    // Device management
    bool addDevice(NonRGBDevice* device);
    bool removeDevice(const QString& name);
    NonRGBDevice* getDevice(const QString& name) const;
    QVector<NonRGBDevice*> getAllDevices() const;
    QVector<NonRGBDevice*> getDevicesByType(NonRGBDeviceType type) const;
    
    // Utilities
    QStringList getDeviceNames() const;
    bool hasDevice(const QString& name) const;
    int getDeviceCount() const;
    int getDeviceCountByType(NonRGBDeviceType type) const;

signals:
    void deviceAdded(NonRGBDevice* device);
    void deviceRemoved(const QString& name);
    void deviceModified(NonRGBDevice* device);
    void errorOccurred(const QString& error);

private:
    QMap<QString, NonRGBDevice*> devices;
};