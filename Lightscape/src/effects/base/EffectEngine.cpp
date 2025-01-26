#include "effects/base/EffectEngine.h"
#include <cmath>

EffectEngine::EffectEngine(SpatialGrid* grid, QObject* parent)
    : QObject(parent)
    , grid(grid)
    , timer(new QTimer(this))
    , currentEffect(None)
    , time(0)
    , speed(50)
    , intensity(100)
    , baseColor(0x000000)    // Black
    , targetColor(0xFFFFFF)  // White
    , colorMode(false)
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
        emit effectStarted(effect);
    }
}

void EffectEngine::stopEffect()
{
    if (timer->isActive())
    {
        timer->stop();
        currentEffect = None;
        emit effectStopped();
    }
}

void EffectEngine::setSpeed(int speed)
{
    this->speed = speed;
}

void EffectEngine::setIntensity(int intensity)
{
    this->intensity = intensity;
}

void EffectEngine::setColor(RGBColor color)
{
    targetColor = color;
    baseColor = 0x000000;  // Start from black
    colorMode = true;
}

void EffectEngine::getRGB(RGBColor color, uint8_t& r, uint8_t& g, uint8_t& b)
{
    r = color & 0xFF;
    g = (color >> 8) & 0xFF;
    b = (color >> 16) & 0xFF;
}

RGBColor EffectEngine::interpolateColors(RGBColor color1, RGBColor color2, float factor)
{
    uint8_t r1, g1, b1, r2, g2, b2;
    getRGB(color1, r1, g1, b1);
    getRGB(color2, r2, g2, b2);

    uint8_t r = r1 + (r2 - r1) * factor;
    uint8_t g = g1 + (g2 - g1) * factor;
    uint8_t b = b1 + (b2 - b1) * factor;

    return (b << 16) | (g << 8) | r;
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
    emit colorsUpdated();
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
                
                RGBColor color;
                if (colorMode)
                {
                    color = interpolateColors(baseColor, targetColor, fadeIntensity);
                }
                else
                {
                    uint8_t value = static_cast<uint8_t>(255 * fadeIntensity);
                    color = (value << 16) | (value << 8) | value;  // White with intensity
                }

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
                
                RGBColor color;
                if (colorMode)
                {
                    color = interpolateColors(baseColor, targetColor, wave);
                }
                else
                {
                    uint8_t value = static_cast<uint8_t>(255 * wave);
                    color = (value << 16) | (value << 8) | value;  // White with intensity
                }

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
                
                RGBColor color;
                if (colorMode)
                {
                    color = interpolateColors(baseColor, targetColor, ripple);
                }
                else
                {
                    uint8_t value = static_cast<uint8_t>(255 * ripple);
                    color = (value << 16) | (value << 8) | value;  // White with intensity
                }

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
        
        RGBColor color;
        if (colorMode)
        {
            color = interpolateColors(baseColor, targetColor, layerIntensity);
        }
        else
        {
            uint8_t value = static_cast<uint8_t>(255 * layerIntensity);
            color = (value << 16) | (value << 8) | value;  // White with intensity
        }

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