#include "core/TrayMenuManager.h"
#include <QAction>

TrayMenuManager::TrayMenuManager(ResourceHandler* resourceHandler, QObject* parent)
    : QObject(parent)
    , resource_handler(resourceHandler)
    , main_menu(nullptr)
{
}

TrayMenuManager::~TrayMenuManager()
{
    if (main_menu)
    {
        delete main_menu;
    }
}

QMenu* TrayMenuManager::createTrayMenu()
{
    if (main_menu)
    {
        delete main_menu;
    }

    main_menu = new QMenu("Lightscape");

    // Effects submenu
    QMenu* effectsMenu = main_menu->addMenu("Effects");
    setupEffectsMenu(effectsMenu);

    // Grid submenu
    QMenu* gridMenu = main_menu->addMenu("Grid");
    setupGridMenu(gridMenu);

    // Utility submenu
    QMenu* utilityMenu = main_menu->addMenu("Utilities");
    setupUtilityMenu(utilityMenu);

    return main_menu;
}

void TrayMenuManager::setupEffectsMenu(QMenu* effectsMenu)
{
    QAction* explosionEffect = effectsMenu->addAction("Explosion Effect");
    explosionEffect->setCheckable(true);
    connect(explosionEffect, &QAction::triggered, this, &TrayMenuManager::handleEffectAction);

    QAction* customEffect = effectsMenu->addAction("Custom Effect");
    customEffect->setCheckable(true);
    connect(customEffect, &QAction::triggered, this, &TrayMenuManager::handleEffectAction);

    effectsMenu->addSeparator();

    QAction* stopAll = effectsMenu->addAction("Stop All Effects");
    connect(stopAll, &QAction::triggered, this, [this]() {
        emit effectToggled("", false);
    });
}

void TrayMenuManager::setupGridMenu(QMenu* gridMenu)
{
    QAction* showGrid = gridMenu->addAction("Show Grid");
    showGrid->setCheckable(true);
    connect(showGrid, &QAction::triggered, this, &TrayMenuManager::handleGridVisibility);

    QAction* configureGrid = gridMenu->addAction("Configure Grid...");
    connect(configureGrid, &QAction::triggered, this, [this]() {
        // Signal to open grid configuration
        emit gridVisibilityChanged(true);
    });
}

void TrayMenuManager::setupUtilityMenu(QMenu* utilityMenu)
{
    QAction* resetAll = utilityMenu->addAction("Reset All");
    connect(resetAll, &QAction::triggered, this, &TrayMenuManager::handleReset);

    QAction* refreshDevices = utilityMenu->addAction("Refresh Devices");
    connect(refreshDevices, &QAction::triggered, this, [this]() {
        if (resource_handler && resource_handler->isInitialized())
        {
            // Trigger device refresh through resource handler
            // Add implementation as needed
        }
    });
}

void TrayMenuManager::handleEffectAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
    {
        emit effectToggled(action->text(), action->isChecked());
    }
}

void TrayMenuManager::handleGridVisibility(bool visible)
{
    emit gridVisibilityChanged(visible);
}

void TrayMenuManager::handleReset()
{
    emit resetRequested();
}

void TrayMenuManager::updateMenuState()
{
    // Update menu items based on current state
    if (main_menu)
    {
        // Update checkable items, enable/disable items based on current state
        // Implementation depends on how state is tracked
    }
}