#include "effects/EffectEngine.h"
#include <cmath>

EffectEngine::EffectEngine(SpatialGrid* grid, QObject* parent)
    : QObject(parent)
    , grid(grid)
    , timer(new QTimer(this))
    , currentEffect(None)
    , time(0)
    , speed(50)
    , intensity(100)
{
    timer->setInterval(16); // ~60 FPS
    connect(timer, &QTimer::timeout, this, &EffectEngine::updateEffect);
}

EffectEngine::~EffectEngine()
{
    stopEffect();
}

void EffectEngine::startEffect(Effect effect)
{
    stopEffect();
    currentEffect = effect;
    time = 0;
    if (effect != None)
    {
        timer->start();
    }
}

void EffectEngine::stopEffect()
{
    timer->stop();
    currentEffect = None;
}

void EffectEngine::setSpeed(int speed)
{
    this->speed = speed;
}

void EffectEngine::setIntensity(int intensity)
{
    this->intensity = intensity;
}

void EffectEngine::updateEffect()
{
    switch (currentEffect)
    {
        case RadialFade:
            updateRadialFade();
            break;
        case Wave:
            updateWave();
            break;
        case Ripple:
            updateRipple();
            break;
        case LayerCascade:
            updateLayerCascade();
            break;
        default:
            break;
    }
    time += 0.016f * speed / 50.0f;
    emit colorsUpdated(); // Add this line to update devices after each effect update
}

void EffectEngine::updateRadialFade()
{
    if (!grid->HasUserPosition()) return;

    GridPosition userPos = grid->GetUserPosition().value();
    GridDimensions dims = grid->GetDimensions();
    float maxDist = sqrt(dims.width * dims.width + dims.height * dims.height + dims.depth * dims.depth);

    for (int z = 0; z < dims.depth; z++)
    {
        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (!grid->HasAssignments(pos)) continue;

                float dx = pos.x - userPos.x;
                float dy = pos.y - userPos.y;
                float dz = (pos.z - userPos.z) * 2.0f; // Layer distance weighted more
                float dist = sqrt(dx*dx + dy*dy + dz*dz);
                
                float fadeIntensity = (1.0f - dist/maxDist) * intensity/100.0f;
                fadeIntensity = std::max(0.0f, std::min(1.0f, fadeIntensity));
                
                uint8_t value = static_cast<uint8_t>(255 * fadeIntensity);
                RGBColor color = ToRGBColor(value, value, value);

                auto assignments = grid->GetAssignments(pos);
                for (int i = 0; i < assignments.size(); i++)
                {
                    grid->UpdateAssignmentColor(pos, i, color);
                }
            }
        }
    }
}

void EffectEngine::updateWave()
{
    if (!grid->HasUserPosition()) return;

    GridPosition userPos = grid->GetUserPosition().value();
    GridDimensions dims = grid->GetDimensions();

    for (int z = 0; z < dims.depth; z++)
    {
        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (!grid->HasAssignments(pos)) continue;

                float dx = pos.x - userPos.x;
                float dy = pos.y - userPos.y;
                float dz = (pos.z - userPos.z) * 2.0f;
                float dist = sqrt(dx*dx + dy*dy + dz*dz);
                
                float wave = sin(dist - time * 3.0f) * 0.5f + 0.5f;
                wave *= intensity/100.0f;
                
                uint8_t value = static_cast<uint8_t>(255 * wave);
                RGBColor color = ToRGBColor(value, value, value);

                auto assignments = grid->GetAssignments(pos);
                for (int i = 0; i < assignments.size(); i++)
                {
                    grid->UpdateAssignmentColor(pos, i, color);
                }
            }
        }
    }
}

void EffectEngine::updateRipple()
{
    if (!grid->HasUserPosition()) return;

    GridPosition userPos = grid->GetUserPosition().value();
    GridDimensions dims = grid->GetDimensions();

    for (int z = 0; z < dims.depth; z++)
    {
        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (!grid->HasAssignments(pos)) continue;

                float dx = pos.x - userPos.x;
                float dy = pos.y - userPos.y;
                float dz = (pos.z - userPos.z) * 2.0f;
                float dist = sqrt(dx*dx + dy*dy + dz*dz);
                
                float ripple = 1.0f - fabs(dist - (time * 2.0f));
                ripple = std::max(0.0f, ripple);
                ripple *= intensity/100.0f;
                
                uint8_t value = static_cast<uint8_t>(255 * ripple);
                RGBColor color = ToRGBColor(value, value, value);

                auto assignments = grid->GetAssignments(pos);
                for (int i = 0; i < assignments.size(); i++)
                {
                    grid->UpdateAssignmentColor(pos, i, color);
                }
            }
        }
    }
}

void EffectEngine::updateLayerCascade()
{
    GridDimensions dims = grid->GetDimensions();
    
    for (int z = 0; z < dims.depth; z++)
    {
        float layerOffset = z * 0.5f;
        float layerIntensity = sin(time + layerOffset) * 0.5f + 0.5f;
        layerIntensity *= intensity/100.0f;
        
        uint8_t value = static_cast<uint8_t>(255 * layerIntensity);
        RGBColor color = ToRGBColor(value, value, value);

        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (!grid->HasAssignments(pos)) continue;

                auto assignments = grid->GetAssignments(pos);
                for (int i = 0; i < assignments.size(); i++)
                {
                    grid->UpdateAssignmentColor(pos, i, color);
                }
            }
        }
    }
}