#pragma once

#include <QObject>
#include <QtPlugin>
#include "OpenRGBPluginInterface.h"
#include "core/ResourceHandler.h"
#include "core/TrayMenuManager.h"
#include "core/StateManager.h"
#include "core/VersionManager.h"

class LightscapeWidget;

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
    
    void Load(bool dark_theme, ResourceManager* resource_manager) override;
    QWidget* GetWidget() override;
    QMenu* GetTrayMenu() override;
    void Unload() override;

private slots:
    void handleResourceError(const QString& error);
    void handleEffectToggled(const QString& effectName, bool enabled);
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
};