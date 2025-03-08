/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| LightscapePlugin.cpp                                      |
|                                                           |
| Main plugin implementation                                |
\*---------------------------------------------------------*/

#include "core/LightscapePlugin.h"
#include "devices/DeviceManager.h"
#include "core/LightscapeWidget.h"
#include "core/ThemeManager.h"
#include <QApplication>
#include <QMenu>
#include <QMessageBox>

bool LightscapePlugin::DarkTheme = false;
ResourceManager* LightscapePlugin::RMPointer = nullptr;

LightscapePlugin::LightscapePlugin() :
    resource_handler(new ResourceHandler(this)),
    tray_manager(nullptr),
    state_manager(new StateManager(this)),
    widget(nullptr),
    device_manager(nullptr)
{
    printf("[Lightscape] Plugin constructor.\n");
    
    connect(resource_handler, &ResourceHandler::resourceError,
            this, &LightscapePlugin::handleResourceError);
            
    connect(state_manager, &StateManager::stateChanged,
            this, &LightscapePlugin::handleStateChanged);
}

LightscapePlugin::~LightscapePlugin()
{
    printf("[Lightscape] Plugin destructor.\n");
    Unload();
}

OpenRGBPluginInfo LightscapePlugin::GetPluginInfo()
{
    printf("[Lightscape] Loading plugin info.\n");
    
    OpenRGBPluginInfo info;
    info.Name = "Lightscape";
    info.Description = "3D Spatial LED Control System";
    info.Version = VERSION_STRING;
    info.Commit = GIT_COMMIT_ID;
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
    printf("[Lightscape] Loading plugin API version.\n");
    return OPENRGB_PLUGIN_API_VERSION;
}

void LightscapePlugin::Load(ResourceManagerInterface* resource_manager_ptr)
{
    printf("[Lightscape] Loading plugin.\n");
    
    // Determine dark theme from application palette
    bool dark_theme = QApplication::palette().window().color().value() < 128;
    DarkTheme = dark_theme;
    
    // Cast to ResourceManager since we need specific functionality
    ResourceManager* resource_manager = dynamic_cast<ResourceManager*>(resource_manager_ptr);
    if (!resource_manager)
    {
        handlePluginError("Failed to cast ResourceManagerInterface to ResourceManager");
        return;
    }
    
    RMPointer = resource_manager;
    
    state_manager->setState(StateManager::PluginState::Initializing);
    
    if (!VersionManager::isApiVersionCompatible(OPENRGB_PLUGIN_API_VERSION))
    {
        handlePluginError(QString("Incompatible API version"));
        return;
    }

    if (!initializePlugin(dark_theme, resource_manager))
    {
        return;
    }

    state_manager->setState(StateManager::PluginState::Running);
    state_manager->setEnabled(true);
}

bool LightscapePlugin::initializePlugin(bool dark, ResourceManager* manager)
{
    printf("[Lightscape] Initializing plugin.\n");
    
    if (!resource_handler->initialize(manager))
    {
        handlePluginError(QString("Failed to initialize resource handler: ") + 
                         resource_handler->getLastError());
        return false;
    }

    // Initialize device management
    device_manager = new DeviceManager(resource_handler->getResourceManager(), this);

    if (widget == nullptr)
    {
        widget = new LightscapeWidget(resource_handler->getResourceManager());
        widget->setStyleSheet(ThemeManager::getStyleSheet(dark));
        widget->initializeGrid();
    }

    if (tray_manager == nullptr)
    {
        tray_manager = new TrayMenuManager(resource_handler, this);
        connect(tray_manager, &TrayMenuManager::gridVisibilityChanged,
                this, &LightscapePlugin::handleGridVisibilityChanged);
        connect(tray_manager, &TrayMenuManager::resetRequested,
                this, &LightscapePlugin::handleReset);
    }

    return true;
}

QWidget* LightscapePlugin::GetWidget()
{
    printf("[Lightscape] Creating widget.\n");
    
    if (state_manager->getCurrentState() != StateManager::PluginState::Running)
    {
        qWarning() << "Attempting to get widget while plugin is not running";
        return nullptr;
    }
    return widget;
}

QMenu* LightscapePlugin::GetTrayMenu()
{
    printf("[Lightscape] Creating tray menu.\n");
    
    if (state_manager->getCurrentState() != StateManager::PluginState::Running || 
        !tray_manager)
    {
        return nullptr;
    }
    return tray_manager->createTrayMenu();
}

void LightscapePlugin::Unload()
{
    printf("[Lightscape] Unloading plugin.\n");
    
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

    if (device_manager != nullptr)
    {
        delete device_manager;
        device_manager = nullptr;
    }

    resource_handler->cleanup();
}

void LightscapePlugin::handleResourceError(const QString& error)
{
    handlePluginError(error);
}

void LightscapePlugin::handleGridVisibilityChanged(bool visible)
{
    if (widget && state_manager->getCurrentState() == StateManager::PluginState::Running)
    {
        widget->handleGridVisibilityChanged(visible);
    }
}

void LightscapePlugin::handleReset()
{
    if (widget && state_manager->getCurrentState() == StateManager::PluginState::Running)
    {
        widget->handleReset();
    }
}

void LightscapePlugin::handleStateChanged(StateManager::PluginState newState)
{
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
    printf("[Lightscape] Plugin error: %s\n", error.toStdString().c_str());
    
    qWarning() << "Plugin error:" << error;
    state_manager->setError(error);
    
    if (widget)
    {
        QMessageBox::warning(widget, 
                           "Lightscape Plugin Error",
                           error);
    }
}