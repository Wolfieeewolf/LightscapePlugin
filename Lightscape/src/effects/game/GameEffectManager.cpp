#include "effects/game/GameEffectManager.h"
#include "effects/game/ExplosionEffect.h"
#include <QDateTime>

GameEffectManager::GameEffectManager(SpatialGrid* grid, QObject* parent)
    : QObject(parent)
    , grid(grid)
    , updateTimer(new QTimer(this))
{
    updateTimer->setInterval(UPDATE_INTERVAL);
    connect(updateTimer, &QTimer::timeout, this, &GameEffectManager::updateEffects);
    updateTimer->start();
}

GameEffectManager::~GameEffectManager()
{
    updateTimer->stop();
    qDeleteAll(effects);
}

void GameEffectManager::registerEffect(GameEffectBase* effect)
{
    if (!effect)
        return;
        
    const QString& name = effect->getName();
    if (effects.contains(name))
    {
        delete effects[name];
    }
    
    effects[name] = effect;
}

void GameEffectManager::removeEffect(const QString& name)
{
    if (effects.contains(name))
    {
        stopEffect(name);
        delete effects[name];
        effects.remove(name);
    }
}

void GameEffectManager::startEffect(const QString& name)
{
    if (effects.contains(name))
    {
        activeEffects.insert(name);
    }
}

void GameEffectManager::stopEffect(const QString& name)
{
    activeEffects.remove(name);
}

void GameEffectManager::stopAllEffects()
{
    activeEffects.clear();
}

void GameEffectManager::handleGameData(const GameData& data)
{
    if (data.dataType == "explosion")
    {
        // Extract explosion parameters from game data
        GridPosition position(
            data.parameters["x"].toFloat(),
            data.parameters["y"].toFloat(),
            data.parameters["z"].toFloat()
        );
        
        // Create new explosion effect
        auto explosion = new ExplosionEffect(grid, position);
        
        // Set optional parameters if provided
        if (data.parameters.contains("intensity"))
            explosion->setIntensity(data.parameters["intensity"].toFloat());
            
        if (data.parameters.contains("color"))
        {
            QColor color(
                data.parameters["color"].toMap()["r"].toInt(),
                data.parameters["color"].toMap()["g"].toInt(),
                data.parameters["color"].toMap()["b"].toInt()
            );
            explosion->setColor(color);
        }
        
        // Register and start the effect
        QString effectName = QString("explosion_%1").arg(QDateTime::currentMSecsSinceEpoch());
        registerEffect(explosion);
        startEffect(effectName);
    }
}

void GameEffectManager::updateEffects()
{
    // Update all active effects
    bool needsUpdate = false;
    
    QSet<QString> finishedEffects;
    
    for (const QString& name : activeEffects)
    {
        if (effects.contains(name))
        {
            GameEffectBase* effect = effects[name];
            effect->update(UPDATE_INTERVAL);
            
            if (effect->isFinished())
            {
                finishedEffects.insert(name);
            }
            
            needsUpdate = true;
        }
    }
    
    // Remove finished effects
    for (const QString& name : finishedEffects)
    {
        activeEffects.remove(name);
    }
    
    if (needsUpdate)
    {
        emit updateRequired();
    }
}