#pragma once

#include <QObject>
#include <QJsonObject>
#include <QString>
#include "grid/SpatialGrid.h"
#include "devices/NonRGBDeviceManager.h"
#include "core/DeviceManager.h"

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    static SettingsManager& getInstance()
    {
        static SettingsManager instance;
        return instance;
    }

    void initialize(SpatialGrid* grid, NonRGBDeviceManager* nonRGBManager, DeviceManager* deviceManager);

private slots:
    void saveState();

private:
    SettingsManager(QObject* parent = nullptr);
    ~SettingsManager() = default;
    
    void loadState();
    QString getSettingsPath() const;

    SpatialGrid* spatialGrid;
    NonRGBDeviceManager* nonRGBDeviceManager;
    DeviceManager* deviceManager;

    QString settingsFileName = "lightscape_state.json";
};