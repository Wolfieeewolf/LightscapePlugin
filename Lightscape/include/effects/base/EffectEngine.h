#pragma once

#include <QObject>
#include <QTimer>
#include "grid/SpatialGrid.h"

class EffectEngine : public QObject
{
    Q_OBJECT

public:
    enum Effect {
        None,
        RadialFade,
        Wave,
        Ripple,
        LayerCascade
    };

    explicit EffectEngine(SpatialGrid* grid, QObject* parent = nullptr);
    ~EffectEngine();

    void startEffect(Effect effect);
    void stopEffect();
    void setSpeed(int speed);
    void setIntensity(int intensity);
    void setColor(RGBColor color);
    void setBaseColor(RGBColor color) { baseColor = color; }
    void setTargetColor(RGBColor color) { targetColor = color; }
    
    bool isRunning() const { return timer->isActive(); }
    Effect getCurrentEffect() const { return currentEffect; }
    int getSpeed() const { return speed; }
    int getIntensity() const { return intensity; }
    RGBColor getBaseColor() const { return baseColor; }
    RGBColor getTargetColor() const { return targetColor; }

signals:
    void colorsUpdated();
    void effectStarted(Effect effect);
    void effectStopped();

private slots:
    void updateEffect();

private:
    void updateRadialFade();
    void updateWave();
    void updateRipple();
    void updateLayerCascade();
    
    // Helper function for color interpolation
    RGBColor interpolateColors(RGBColor color1, RGBColor color2, float factor);
    
    // Extract RGB components
    void getRGB(RGBColor color, uint8_t& r, uint8_t& g, uint8_t& b);

    SpatialGrid* grid;
    QTimer* timer;
    Effect currentEffect;
    float time;
    int speed;
    int intensity;
    RGBColor baseColor;     // Starting color for effects
    RGBColor targetColor;   // Target color for effects
    bool colorMode;         // True for color transition effects
};