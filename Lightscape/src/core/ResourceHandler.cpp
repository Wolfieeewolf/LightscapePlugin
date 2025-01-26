#include "core/ResourceHandler.h"

ResourceHandler::ResourceHandler(QObject* parent)
    : QObject(parent)
    , resource_manager(nullptr)
    , initialized(false)
{
}

ResourceHandler::~ResourceHandler()
{
    cleanup();
}

bool ResourceHandler::initialize(ResourceManager* manager)
{
    if (initialized)
    {
        setError("Resource handler already initialized");
        return false;
    }

    if (!validateResourceManager(manager))
    {
        return false;
    }

    resource_manager = manager;
    initialized = true;
    emit resourcesInitialized();
    return true;
}

void ResourceHandler::cleanup()
{
    if (initialized)
    {
        resource_manager = nullptr;
        initialized = false;
        emit resourcesCleanedUp();
    }
}

bool ResourceHandler::isInitialized() const
{
    return initialized;
}

ResourceManager* ResourceHandler::getResourceManager() const
{
    return resource_manager;
}

bool ResourceHandler::hasError() const
{
    return !last_error.isEmpty();
}

QString ResourceHandler::getLastError() const
{
    return last_error;
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
    last_error = error;
    emit resourceError(error);
}