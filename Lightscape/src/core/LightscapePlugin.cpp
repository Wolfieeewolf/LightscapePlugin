#include "core/LightscapePlugin.h"
#include "core/LightscapeWidget.h"
#include "core/ThemeManager.h"
#include <QMenu>
#include <QMessageBox>

LightscapePlugin::LightscapePlugin() :
    dark_theme(false),
    resource_handler(new ResourceHandler(this)),
    tray_manager(nullptr),
    state_manager(new StateManager(this)),
    widget(nullptr)
{
    connect(resource_handler, &ResourceHandler::resourceError,
            this, &LightscapePlugin::handleResourceError);
            
    connect(state_manager, &StateManager::stateChanged,
            this, &LightscapePlugin::handleStateChanged);
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
    info.Version = VersionManager::getPluginVersion().toStdString();
    info.Commit = "experimental";
    info.URL = "https://openrgb.org";
    info.Icon.load(":/images/Lightscape64.png");
    
    info.Location = OPENRGB_PLUGIN_LOCATION_TOP;
    info.Label = "Lightscape";
    info.TabIconString = "Lightscape";
    info.TabIcon.load(":/images/Lightscape16.png");
    
    return info;
}

unsigned int LightscapePlugin::GetPluginAPIVersion()
{
    return OPENRGB_PLUGIN_API_VERSION;
}

void LightscapePlugin::Load(bool dark, ResourceManager* manager)
{
    state_manager->setState(StateManager::PluginState::Initializing);
    
    if (!VersionManager::isApiVersionCompatible(OPENRGB_PLUGIN_API_VERSION))
    {
        handlePluginError(QString("Incompatible API version"));
        return;
    }

    if (!initializePlugin(dark, manager))
    {
        return;
    }

    state_manager->setState(StateManager::PluginState::Running);
    state_manager->setEnabled(true);
}

bool LightscapePlugin::initializePlugin(bool dark, ResourceManager* manager)
{
    dark_theme = dark;

    if (!resource_handler->initialize(manager))
    {
        handlePluginError(QString("Failed to initialize resource handler: ") + 
                         resource_handler->getLastError());
        return false;
    }

    if (widget == nullptr)
    {
        widget = new LightscapeWidget(resource_handler->getResourceManager());
        widget->setStyleSheet(ThemeManager::getStyleSheet(dark_theme));
    }

    if (tray_manager == nullptr)
    {
        tray_manager = new TrayMenuManager(resource_handler, this);
        connect(tray_manager, &TrayMenuManager::effectToggled,
                this, &LightscapePlugin::handleEffectToggled);
        connect(tray_manager, &TrayMenuManager::gridVisibilityChanged,
                this, &LightscapePlugin::handleGridVisibilityChanged);
        connect(tray_manager, &TrayMenuManager::resetRequested,
                this, &LightscapePlugin::handleReset);
    }

    return true;
}

QWidget* LightscapePlugin::GetWidget()
{
    if (state_manager->getCurrentState() != StateManager::PluginState::Running)
    {
        qWarning() << "Attempting to get widget while plugin is not running";
        return nullptr;
    }
    return widget;
}

QMenu* LightscapePlugin::GetTrayMenu()
{
    if (state_manager->getCurrentState() != StateManager::PluginState::Running || 
        !tray_manager)
    {
        return nullptr;
    }
    return tray_manager->createTrayMenu();
}

void LightscapePlugin::Unload()
{
    state_manager->setState(StateManager::PluginState::Disabled);

    if (widget != nullptr)
    {
        delete widget;
        widget = nullptr;
    }

    if (tray_manager != nullptr)
    {
        delete tray_manager;
        tray_manager = nullptr;
    }

    resource_handler->cleanup();
}

void LightscapePlugin::handleResourceError(const QString& error)
{
    handlePluginError(error);
}

void LightscapePlugin::handleEffectToggled(const QString& /* effectName */, bool /* enabled */)
{
    if (widget && state_manager->getCurrentState() == StateManager::PluginState::Running)
    {
        // Forward to widget's effect handling
        // Add implementation
    }
}

void LightscapePlugin::handleGridVisibilityChanged(bool /* visible */)
{
    if (widget && state_manager->getCurrentState() == StateManager::PluginState::Running)
    {
        // Forward to widget's grid handling
        // Add implementation
    }
}

void LightscapePlugin::handleReset()
{
    if (widget && state_manager->getCurrentState() == StateManager::PluginState::Running)
    {
        // Forward reset request to widget
        // Add implementation
    }
}

void LightscapePlugin::handleStateChanged(StateManager::PluginState newState)
{
    // Update UI elements based on state
    if (widget)
    {
        widget->setEnabled(newState == StateManager::PluginState::Running);
    }
    
    if (tray_manager)
    {
        tray_manager->updateMenuState();
    }
}

void LightscapePlugin::handlePluginError(const QString& error)
{
    qWarning() << "Plugin error:" << error;
    state_manager->setError(error);
    
    if (widget)
    {
        QMessageBox::warning(widget, 
                           "Lightscape Plugin Error",
                           error);
    }
}