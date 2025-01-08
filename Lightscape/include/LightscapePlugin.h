#pragma once

#include <QObject>
#include <QtPlugin>
#include "OpenRGBPluginInterface.h"

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

private:
    bool dark_theme;
    ResourceManager* resource_manager;
    LightscapeWidget* widget;
};