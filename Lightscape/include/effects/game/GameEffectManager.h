#pragma once

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QSet>
#include <QString>
#include "GameEffectBase.h"
#include "game/GameDataHandler.h"
#include "grid/SpatialGrid.h"

class GameEffectManager : public QObject {
    Q_OBJECT
public:
    explicit GameEffectManager(SpatialGrid* grid, QObject* parent = nullptr);
    ~GameEffectManager();
    
    void registerEffect(GameEffectBase* effect);
    void removeEffect(const QString& name);
    void startEffect(const QString& name);
    void stopEffect(const QString& name);
    void stopAllEffects();
    
    // Add getter for grid
    SpatialGrid* GetGrid() const { return grid; }

public slots:
    void handleGameData(const GameData& data);
    void updateEffects();

signals:
    void updateRequired();

private:
    SpatialGrid* grid;
    QMap<QString, GameEffectBase*> effects;
    QSet<QString> activeEffects;
    QTimer* updateTimer;
    
    static const int UPDATE_INTERVAL = 16; // ~60 FPS
};