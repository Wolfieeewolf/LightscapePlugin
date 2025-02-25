#pragma once

#include <QObject>
#include <QtPlugin>
#include "OpenRGBPluginInterface.h"
#include "core/ResourceHandler.h"
#include "core/TrayMenuManager.h"
#include "core/StateManager.h"
#include "core/VersionManager.h"

class LightscapeWidget;
class DeviceManager;

class LightscapePlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID)
    Q_INTERFACES(OpenRGBPluginInterface)

public:
    LightscapePlugin();
    ~LightscapePlugin();

    // Plugin Interface Functions
    OpenRGBPluginInfo GetPluginInfo() override;
    unsigned int GetPluginAPIVersion() override;
    
    void Load(ResourceManagerInterface* resource_manager_ptr) override;
    QWidget* GetWidget() override;
    QMenu* GetTrayMenu() override;
    void Unload() override;

private slots:
    void handleResourceError(const QString& error);
    void handleGridVisibilityChanged(bool visible);
    void handleReset();
    void handleStateChanged(StateManager::PluginState newState);

private:
    bool initializePlugin(bool dark_theme, ResourceManager* resource_manager);
    void handlePluginError(const QString& error);

    bool dark_theme;
    ResourceHandler* resource_handler;
    TrayMenuManager* tray_manager;
    StateManager* state_manager;
    LightscapeWidget* widget;
    DeviceManager* device_manager;
};