#include "core/SettingsManager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

SettingsManager::SettingsManager(QObject* parent)
    : QObject(parent)
    , spatialGrid(nullptr)
    , nonRGBDeviceManager(nullptr)
    , deviceManager(nullptr)
{
}

void SettingsManager::initialize(SpatialGrid* grid, NonRGBDeviceManager* nonRGBManager, DeviceManager* deviceManager)
{
    this->spatialGrid = grid;
    this->nonRGBDeviceManager = nonRGBManager;
    this->deviceManager = deviceManager;

    // Connect to relevant signals for auto-saving
    if (spatialGrid)
    {
        connect(spatialGrid, &SpatialGrid::assignmentsChanged, this, &SettingsManager::saveState);
        connect(spatialGrid, &SpatialGrid::userPositionChanged, this, &SettingsManager::saveState);
        connect(spatialGrid, &SpatialGrid::layerLabelChanged, this, &SettingsManager::saveState);
    }

    if (nonRGBDeviceManager)
    {
        connect(nonRGBDeviceManager, &NonRGBDeviceManager::deviceAdded, this, &SettingsManager::saveState);
        connect(nonRGBDeviceManager, &NonRGBDeviceManager::deviceRemoved, this, &SettingsManager::saveState);
    }

    if (deviceManager)
    {
        connect(deviceManager, &DeviceManager::deviceAssignmentChanged, this, &SettingsManager::saveState);
    }

    // Load existing state if any
    loadState();
}

QString SettingsManager::getSettingsPath() const
{
    // Get OpenRGB plugins directory
    QString configPath;
    
    #ifdef _WIN32
        configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        configPath = configPath.replace("/Lightscape", "/OpenRGB/plugins");
    #elif __APPLE__
        configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        configPath = configPath.replace("/Lightscape", "/OpenRGB/plugins");
    #else // Linux
        configPath = QDir::homePath() + "/.config/OpenRGB/plugins";
    #endif

    return configPath + "/" + settingsFileName;
}

void SettingsManager::saveState()
{
    if (!spatialGrid || !nonRGBDeviceManager || !deviceManager)
    {
        qWarning() << "SettingsManager: Cannot save state - managers not initialized";
        return;
    }

    QJsonObject state;

    // Save grid dimensions
    QJsonObject gridObject;
    GridDimensions dims = spatialGrid->GetDimensions();
    gridObject["width"] = dims.width;
    gridObject["height"] = dims.height;
    gridObject["depth"] = dims.depth;
    state["grid_dimensions"] = gridObject;

    // Save user position if set
    if (spatialGrid->HasUserPosition())
    {
        auto pos = spatialGrid->GetUserPosition();
        if (pos)
        {
            QJsonObject userPos;
            userPos["x"] = pos->x;
            userPos["y"] = pos->y;
            userPos["z"] = pos->z;
            state["user_position"] = userPos;
        }
    }

    // Save layer labels
    QJsonObject layerLabels;
    for (int i = 0; i < dims.depth; i++)
    {
        QString label = spatialGrid->GetLayerLabel(i);
        if (!label.isEmpty())
        {
            layerLabels[QString::number(i)] = label;
        }
    }
    state["layer_labels"] = layerLabels;

    // Save non-RGB devices
    QJsonArray nonRGBDevices;
    for (NonRGBDevice* device : nonRGBDeviceManager->getAllDevices())
    {
        QJsonObject deviceObj;
        deviceObj["name"] = device->getName();
        deviceObj["type"] = static_cast<int>(device->getType());
        
        // Save position
        GridPosition pos = device->getPosition();
        QJsonObject posObj;
        posObj["x"] = pos.x;
        posObj["y"] = pos.y;
        posObj["z"] = pos.z;
        deviceObj["position"] = posObj;

        // Save dimensions
        QJsonObject dimObj;
        QSize dims = device->getDimensions();
        dimObj["width"] = dims.width();
        dimObj["height"] = dims.height();
        dimObj["depth"] = device->getDepth();
        deviceObj["dimensions"] = dimObj;

        // Save reference points
        QJsonObject refPoints;
        for (const QString& name : device->getReferencePointNames())
        {
            GridPosition refPos = device->getReferencePoint(name);
            QJsonObject refPosObj;
            refPosObj["x"] = refPos.x;
            refPosObj["y"] = refPos.y;
            refPosObj["z"] = refPos.z;
            refPoints[name] = refPosObj;
        }
        deviceObj["reference_points"] = refPoints;

        nonRGBDevices.append(deviceObj);
    }
    state["non_rgb_devices"] = nonRGBDevices;

    // Save grid assignments
    QJsonArray assignments;
    for (int z = 0; z < dims.depth; z++)
    {
        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (spatialGrid->HasAssignments(pos))
                {
                    QJsonObject posAssignments;
                    posAssignments["x"] = x;
                    posAssignments["y"] = y;
                    posAssignments["z"] = z;

                    QJsonArray deviceAssignments;
                    for (const DeviceAssignment& assignment : spatialGrid->GetAssignments(pos))
                    {
                        QJsonObject assignObj;
                        assignObj["device_index"] = static_cast<int>(assignment.device_index);
                        assignObj["device_type"] = static_cast<int>(assignment.device_type);
                        assignObj["zone_index"] = assignment.zone_index;
                        assignObj["led_index"] = assignment.led_index;
                        assignObj["color"] = static_cast<int>(assignment.color);
                        
                        // Save device name for more reliable identification across sessions
                        if (assignment.device_type == Lightscape::DeviceType::RGB && 
                            deviceManager && deviceManager->ValidateDeviceIndex(assignment.device_index, Lightscape::DeviceType::RGB)) {
                            assignObj["device_name"] = deviceManager->GetRGBDeviceName(assignment.device_index);
                            
                            // Save zone/LED name if applicable
                            if (assignment.zone_index >= 0) {
                                assignObj["zone_name"] = deviceManager->GetZoneName(assignment.device_index, assignment.zone_index);
                            }
                            if (assignment.led_index >= 0) {
                                assignObj["led_name"] = deviceManager->GetLEDName(assignment.device_index, assignment.led_index);
                            }
                        }
                        else if (assignment.device_type == Lightscape::DeviceType::NonRGB && 
                                 deviceManager && deviceManager->ValidateDeviceIndex(assignment.device_index, Lightscape::DeviceType::NonRGB)) {
                            assignObj["device_name"] = deviceManager->GetNonRGBDeviceName(assignment.device_index);
                        }
                        
                        deviceAssignments.append(assignObj);
                    }
                    posAssignments["assignments"] = deviceAssignments;
                    assignments.append(posAssignments);
                }
            }
        }
    }
    state["assignments"] = assignments;

    // Save to file
    QJsonDocument doc(state);
    QFile file(getSettingsPath());
    
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "SettingsManager: Could not open settings file for writing:" << file.errorString();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    qDebug() << "SettingsManager: State saved successfully";
}

void SettingsManager::loadState()
{
    QFile file(getSettingsPath());
    if (!file.exists())
    {
        qDebug() << "SettingsManager: No saved state found";
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "SettingsManager: Could not open settings file for reading:" << file.errorString();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull())
    {
        qWarning() << "SettingsManager: Failed to parse settings file";
        return;
    }

    QJsonObject state = doc.object();

    // Restore grid dimensions
    if (state.contains("grid_dimensions"))
    {
        QJsonObject gridObject = state["grid_dimensions"].toObject();
        GridDimensions dims(
            gridObject["width"].toInt(),
            gridObject["height"].toInt(),
            gridObject["depth"].toInt()
        );
        spatialGrid->SetDimensions(dims);
    }

    // Restore user position
    if (state.contains("user_position"))
    {
        QJsonObject userPos = state["user_position"].toObject();
        GridPosition pos(
            userPos["x"].toInt(),
            userPos["y"].toInt(),
            userPos["z"].toInt()
        );
        spatialGrid->SetUserPosition(pos);
    }

    // Restore layer labels
    if (state.contains("layer_labels"))
    {
        QJsonObject layerLabels = state["layer_labels"].toObject();
        for (const QString& key : layerLabels.keys())
        {
            spatialGrid->SetLayerLabel(key.toInt(), layerLabels[key].toString());
        }
    }

    // Restore non-RGB devices
    if (state.contains("non_rgb_devices"))
    {
        QJsonArray devices = state["non_rgb_devices"].toArray();
        for (const QJsonValue& value : devices)
        {
            QJsonObject deviceObj = value.toObject();
            NonRGBDevice* device = new NonRGBDevice(
                deviceObj["name"].toString(),
                static_cast<NonRGBDeviceType>(deviceObj["type"].toInt())
            );

            // Restore position
            QJsonObject posObj = deviceObj["position"].toObject();
            device->setPosition(GridPosition(
                posObj["x"].toInt(),
                posObj["y"].toInt(),
                posObj["z"].toInt()
            ));

            // Restore dimensions
            QJsonObject dimObj = deviceObj["dimensions"].toObject();
            device->setDimensions(QSize(
                dimObj["width"].toInt(),
                dimObj["height"].toInt()
            ));
            device->setDepth(dimObj["depth"].toInt());

            // Restore reference points
            QJsonObject refPoints = deviceObj["reference_points"].toObject();
            for (const QString& name : refPoints.keys())
            {
                QJsonObject refPosObj = refPoints[name].toObject();
                device->addReferencePoint(name, GridPosition(
                    refPosObj["x"].toInt(),
                    refPosObj["y"].toInt(),
                    refPosObj["z"].toInt()
                ));
            }

            nonRGBDeviceManager->addDevice(device);
        }
    }

    // Restore grid assignments
    if (state.contains("assignments"))
    {
        QJsonArray assignments = state["assignments"].toArray();
        for (const QJsonValue& value : assignments)
        {
            QJsonObject posAssignments = value.toObject();
            GridPosition pos(
                posAssignments["x"].toInt(),
                posAssignments["y"].toInt(),
                posAssignments["z"].toInt()
            );

            QJsonArray deviceAssignments = posAssignments["assignments"].toArray();
            for (const QJsonValue& assignValue : deviceAssignments)
            {
                QJsonObject assignObj = assignValue.toObject();
                // Try to find device by name first, if available
                int deviceIndex = assignObj["device_index"].toInt();
                Lightscape::DeviceType deviceType = static_cast<Lightscape::DeviceType>(assignObj["device_type"].toInt());
                bool deviceFound = false;
                
                // If we have device name, try to find it by name
                if (assignObj.contains("device_name") && deviceManager) {
                    QString deviceName = assignObj["device_name"].toString();
                    
                    // Try to find the device by name
                    unsigned int count = deviceManager->GetDeviceCount(deviceType);
                    for (unsigned int i = 0; i < count; i++) {
                        if (deviceManager->GetDeviceName(i, deviceType) == deviceName) {
                            deviceIndex = i;
                            deviceFound = true;
                            break;
                        }
                    }
                }
                
                // If we didn't find the device by name and the original index is invalid, skip this assignment
                if (!deviceFound && !deviceManager->ValidateDeviceIndex(deviceIndex, deviceType)) {
                    qDebug() << "SettingsManager: Skipping assignment for device that no longer exists";
                    continue;
                }
                
                DeviceAssignment assignment(deviceIndex, deviceType);
                assignment.zone_index = assignObj["zone_index"].toInt();
                assignment.led_index = assignObj["led_index"].toInt();
                assignment.color = static_cast<RGBColor>(assignObj["color"].toInt());
                
                // Try to match zone/LED by name if provided
                if (deviceType == Lightscape::DeviceType::RGB && 
                    assignment.zone_index >= 0 && assignObj.contains("zone_name")) {
                    QString zoneName = assignObj["zone_name"].toString();
                    size_t zoneCount = deviceManager->GetZoneCount(deviceIndex);
                    
                    for (size_t i = 0; i < zoneCount; i++) {
                        if (deviceManager->GetZoneName(deviceIndex, static_cast<int>(i)) == zoneName) {
                            assignment.zone_index = static_cast<int>(i);
                            break;
                        }
                    }
                }
                
                if (deviceType == Lightscape::DeviceType::RGB && 
                    assignment.led_index >= 0 && assignObj.contains("led_name")) {
                    QString ledName = assignObj["led_name"].toString();
                    size_t ledCount = deviceManager->GetLEDCount(deviceIndex);
                    
                    for (size_t i = 0; i < ledCount; i++) {
                        if (deviceManager->GetLEDName(deviceIndex, static_cast<int>(i)) == ledName) {
                            assignment.led_index = static_cast<int>(i);
                            break;
                        }
                    }
                }

                spatialGrid->AddAssignment(pos, assignment);
                
                // Update device manager
                QString layerLabel = spatialGrid->GetLayerLabel(pos.z);
                deviceManager->SetDeviceAssignment(
                    assignment.device_index,
                    assignment.device_type,
                    layerLabel,
                    pos
                );
            }
        }
    }

    qDebug() << "SettingsManager: State loaded successfully";
}