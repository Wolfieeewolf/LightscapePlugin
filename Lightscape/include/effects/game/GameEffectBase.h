#pragma once

#include <QString>
#include <QColor>
#include "grid/SpatialGrid.h"

class GameEffectBase {
public:
    GameEffectBase(SpatialGrid* grid, const QString& name);
    virtual ~GameEffectBase() = default;
    
    // Core effect functionality
    virtual void update(qint64 deltaTime) = 0;
    virtual bool isFinished() const { return finished; }
    
    // Effect identification
    QString getName() const { return name; }
    QString getType() const { return type; }

protected:
    QString name;        // Unique effect name
    QString type;        // Effect type (explosion, health, etc)
    SpatialGrid* grid;   // Reference to grid for updates
    bool finished;       // Whether effect has completed

    // Utility function for direct color updates
    void updateGridColor(const GridPosition& pos, const RGBColor& color);
};