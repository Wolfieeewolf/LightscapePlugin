#pragma once

#include <QObject>
#include <QMenu>
#include "ResourceHandler.h"

class TrayMenuManager : public QObject
{
    Q_OBJECT

public:
    explicit TrayMenuManager(ResourceHandler* resourceHandler, QObject* parent = nullptr);
    ~TrayMenuManager();

    QMenu* createTrayMenu();
    void updateMenuState();

signals:
    void gridVisibilityChanged(bool visible);
    void resetRequested();

private slots:
    void handleGridVisibility(bool visible);
    void handleReset();

private:
    void setupGridMenu(QMenu* gridMenu);
    void setupUtilityMenu(QMenu* utilityMenu);

    ResourceHandler* resource_handler;
    QMenu* main_menu;
};