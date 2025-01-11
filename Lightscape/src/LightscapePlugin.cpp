#include "../include/LightscapePlugin.h"
#include "../include/LightscapeWidget.h"
#include <QMenu>

LightscapePlugin::LightscapePlugin() :
    dark_theme(false),
    resource_manager(nullptr),
    widget(nullptr)
{
}

LightscapePlugin::~LightscapePlugin()
{
    Unload();
}

OpenRGBPluginInfo LightscapePlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;
    info.Name = "Lightscape";
    info.Description = "3D Spatial LED Control System";
    info.Version = "1.0.0";
    info.Commit = "experimental";
    info.URL = "https://openrgb.org";
    info.Location = OPENRGB_PLUGIN_LOCATION_TOP;
    info.Label = "Lightscape";
    return info;
}

unsigned int LightscapePlugin::GetPluginAPIVersion()
{
    return OPENRGB_PLUGIN_API_VERSION;
}

void LightscapePlugin::Load(bool dark, ResourceManager* manager)
{
    dark_theme = dark;
    resource_manager = manager;

    if (widget == nullptr)
    {
        widget = new LightscapeWidget(resource_manager);
        if (dark_theme)
        {
            widget->setStyleSheet("QWidget { background-color: #2D2D2D; color: #FFFFFF; }");
        }
    }
}

QWidget* LightscapePlugin::GetWidget()
{
    return widget;
}

QMenu* LightscapePlugin::GetTrayMenu()
{
    return nullptr;
}

void LightscapePlugin::Unload()
{
    if (widget != nullptr)
    {
        delete widget;
        widget = nullptr;
    }
    resource_manager = nullptr;
}