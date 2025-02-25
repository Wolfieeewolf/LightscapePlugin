#include "core/VersionManager.h"
#include "OpenRGBPluginInterface.h"

const unsigned int VersionManager::MINIMUM_API_VERSION = 1;
const unsigned int VersionManager::CURRENT_API_VERSION = OPENRGB_PLUGIN_API_VERSION;
const QString VersionManager::PLUGIN_VERSION = "1.0.0";

bool VersionManager::isApiVersionCompatible(unsigned int hostVersion)
{
    return (hostVersion >= MINIMUM_API_VERSION && 
            hostVersion <= CURRENT_API_VERSION);
}

bool VersionManager::isFeatureSupported(const QString& /* featureName */)
{
    // Add feature checks based on API version if needed
    return true;
}

QString VersionManager::getPluginVersion()
{
    return PLUGIN_VERSION;
}

QString VersionManager::getRequiredApiVersion()
{
    return QString::number(CURRENT_API_VERSION);
}

QString VersionManager::getMinimumApiVersion()
{
    return QString::number(MINIMUM_API_VERSION);
}