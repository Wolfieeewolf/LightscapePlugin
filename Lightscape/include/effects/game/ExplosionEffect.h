#pragma once

#include "GameEffectBase.h"
#include <QColor>

class ExplosionEffect : public GameEffectBase {
public:
    ExplosionEffect(SpatialGrid* grid, const GridPosition& position);
    
    void update(qint64 deltaTime) override;
    void setIntensity(float intensity) { this->intensity = intensity; }
    void setColor(const QColor& color) { baseColor = color; }
    
private:
    GridPosition center;     // Explosion center
    float radius;           // Current radius
    float maxRadius;        // Maximum radius
    float duration;         // How long explosion lasts (ms)
    float currentTime;      // Current time in effect (ms)
    float intensity;        // Effect intensity (0-1)
    QColor baseColor;       // Base explosion color
    
    // Helper methods
    float calculateDistance(const GridPosition& pos) const;
    RGBColor calculateColor(float distance) const;
};