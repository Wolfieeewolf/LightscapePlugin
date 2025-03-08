/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| ResourceHandler.cpp                                       |
|                                                           |
| Implementation of resource management                     |
\*---------------------------------------------------------*/

#include "core/ResourceHandler.h"

ResourceHandler::ResourceHandler(QObject* parent)
    : QObject(parent)
    , _resource_manager(nullptr)
    , _initialized(false)
{
    printf("[Lightscape][ResourceHandler] Creating resource handler.\n");
}

ResourceHandler::~ResourceHandler()
{
    printf("[Lightscape][ResourceHandler] Destroying resource handler.\n");
    cleanup();
}

bool ResourceHandler::initialize(ResourceManager* manager)
{
    printf("[Lightscape][ResourceHandler] Initializing with resource manager.\n");
    
    if (_initialized)
    {
        setError("Resource handler already initialized");
        return false;
    }

    if (!validateResourceManager(manager))
    {
        return false;
    }

    _resource_manager = manager;
    _initialized = true;
    emit resourcesInitialized();
    return true;
}

void ResourceHandler::cleanup()
{
    if (_initialized)
    {
        printf("[Lightscape][ResourceHandler] Cleaning up resources.\n");
        
        _resource_manager = nullptr;
        _initialized = false;
        emit resourcesCleanedUp();
    }
}

bool ResourceHandler::isInitialized() const
{
    return _initialized;
}

ResourceManager* ResourceHandler::getResourceManager() const
{
    return _resource_manager;
}

bool ResourceHandler::hasError() const
{
    return !_last_error.isEmpty();
}

QString ResourceHandler::getLastError() const
{
    return _last_error;
}

bool ResourceHandler::validateResourceManager(ResourceManager* manager)
{
    if (!manager)
    {
        setError("Invalid resource manager pointer");
        return false;
    }

    // Add any additional validation checks here
    // For example, check if required controllers are available

    return true;
}

void ResourceHandler::setError(const QString& error)
{
    printf("[Lightscape][ResourceHandler] Error: %s\n", error.toStdString().c_str());
    
    _last_error = error;
    emit resourceError(error);
}