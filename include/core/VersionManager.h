#pragma once

#include <QString>

class VersionManager
{
public:
    static bool isApiVersionCompatible(unsigned int hostVersion);
    static bool isFeatureSupported(const QString& featureName);
    static QString getPluginVersion();
    static QString getRequiredApiVersion();
    static QString getMinimumApiVersion();

private:
    static const unsigned int MINIMUM_API_VERSION;
    static const unsigned int CURRENT_API_VERSION;
    static const QString PLUGIN_VERSION;
};