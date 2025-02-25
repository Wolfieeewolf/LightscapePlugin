#pragma once

#include <QObject>
#include "ResourceManager.h"

class ResourceHandler : public QObject
{
    Q_OBJECT

public:
    explicit ResourceHandler(QObject* parent = nullptr);
    ~ResourceHandler();

    bool initialize(ResourceManager* manager);
    void cleanup();
    bool isInitialized() const;
    ResourceManager* getResourceManager() const;

    // Error handling
    bool hasError() const;
    QString getLastError() const;

signals:
    void resourceError(const QString& error);
    void resourcesInitialized();
    void resourcesCleanedUp();

private:
    bool validateResourceManager(ResourceManager* manager);
    void setError(const QString& error);

    ResourceManager* resource_manager;
    bool initialized;
    QString last_error;
};