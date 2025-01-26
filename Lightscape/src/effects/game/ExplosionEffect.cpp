#include "effects/game/ExplosionEffect.h"
#include <cmath>

ExplosionEffect::ExplosionEffect(SpatialGrid* grid, const GridPosition& position)
    : GameEffectBase(grid, "explosion")
    , center(position)
    , radius(0.0f)
    , maxRadius(5.0f)  // Can be adjusted based on grid size
    , duration(500.0f) // 500ms explosion
    , currentTime(0.0f)
    , intensity(1.0f)
    , baseColor(255, 100, 0)  // Orange explosion by default
{
    type = "explosion";
}

void ExplosionEffect::update(qint64 deltaTime)
{
    currentTime += deltaTime;
    
    // Calculate explosion progress (0 to 1)
    float progress = currentTime / duration;
    if (progress >= 1.0f)
    {
        finished = true;
        return;
    }
    
    // Update explosion radius
    radius = maxRadius * (1.0f - std::pow(1.0f - progress, 2));  // Easing function
    
    // Update all grid positions
    GridDimensions dims = grid->GetDimensions();
    for (int z = 0; z < dims.depth; z++)
    {
        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (!grid->HasAssignments(pos))
                    continue;
                    
                float distance = calculateDistance(pos);
                RGBColor color = calculateColor(distance);
                updateGridColor(pos, color);
            }
        }
    }
}

float ExplosionEffect::calculateDistance(const GridPosition& pos) const
{
    float dx = pos.x - center.x;
    float dy = pos.y - center.y;
    float dz = pos.z - center.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

RGBColor ExplosionEffect::calculateColor(float distance) const
{
    if (distance > radius)
        return ToRGBColor(0, 0, 0);
        
    // Calculate falloff (1 at center, 0 at radius)
    float falloff = 1.0f - (distance / radius);
    falloff = std::pow(falloff, 2.0f);  // Squared falloff for more dramatic effect
    falloff *= intensity;
    
    // Apply falloff to base color
    return ToRGBColor(
        static_cast<uint8_t>(baseColor.red() * falloff),
        static_cast<uint8_t>(baseColor.green() * falloff),
        static_cast<uint8_t>(baseColor.blue() * falloff)
    );
}
