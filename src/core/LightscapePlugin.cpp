/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| LightscapePlugin.cpp                                      |
|                                                           |
| Main plugin implementation                                |
\*---------------------------------------------------------*/

#include "core/LightscapePlugin.h"
#include "devices/DeviceManager.h"
#include "core/EnhancedLightscapeWidget.h"
#include "core/ThemeManager.h"
#include "core/LoggingManager.h"
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
    // Set log level to Error (will only show errors)
    Lightscape::LoggingManager::getInstance().setLogLevel(Lightscape::LogLevel::Error);
    Lightscape::LoggingManager::getInstance().installHandler();
    
    // Enable file logging
    Lightscape::LoggingManager::getInstance().enableFileLogging(true);
    
    // Just log that the plugin loaded
    LOG_INFO("Lightscape Plugin loaded successfully");
    
    connect(resource_handler, &ResourceHandler::resourceError,
            this, &LightscapePlugin::handleResourceError);
            
    connect(state_manager, &StateManager::stateChanged,
            this, &LightscapePlugin::handleStateChanged);
}

LightscapePlugin::~LightscapePlugin()
{
    // Logging disabled, except for errors
    Unload();
}

OpenRGBPluginInfo LightscapePlugin::GetPluginInfo()
{
    // Logging disabled, except for errors
    
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
    // Logging disabled, except for errors
    return OPENRGB_PLUGIN_API_VERSION;
}

void LightscapePlugin::Load(ResourceManagerInterface* resource_manager_ptr)
{
    // Logging disabled, except for errors
    
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
    // Logging disabled, except for errors
    
    // Initialize the registry with test effect
    printf("[Lightscape][Plugin] Registering TestEffect\n");
    
    // Manually register TestEffect
    Lightscape::EffectInfo testEffectInfo = Lightscape::TestEffect::GetStaticInfo();
    Lightscape::EffectRegistry::getInstance().registerEffect(
        testEffectInfo,
        []() -> void* { return new Lightscape::TestEffect(); },
        "Basic"
    );
    
    printf("[Lightscape][Plugin] Registered TestEffect with name: %s, id: %s\n",
           testEffectInfo.name.toStdString().c_str(),
           testEffectInfo.id.toStdString().c_str());
    
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
        widget = new EnhancedLightscapeWidget(resource_handler->getResourceManager());
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
    // Logging disabled, except for errors
    
    if (state_manager->getCurrentState() != StateManager::PluginState::Running)
    {
        qWarning() << "Attempting to get widget while plugin is not running";
        return nullptr;
    }
    return widget;
}

QMenu* LightscapePlugin::GetTrayMenu()
{
    // Logging disabled, except for errors
    
    if (state_manager->getCurrentState() != StateManager::PluginState::Running || 
        !tray_manager)
    {
        return nullptr;
    }
    return tray_manager->createTrayMenu();
}

void LightscapePlugin::Unload()
{
    // Logging disabled, except for errors
    
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
    LOG_ERROR("[Lightscape] Plugin error: " + error);
    
    qWarning() << "Plugin error:" << error;
    state_manager->setError(error);
    
    if (widget)
    {
        QMessageBox::warning(widget, 
                           "Lightscape Plugin Error",
                           error);
    }
}
