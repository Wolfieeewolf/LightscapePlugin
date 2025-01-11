#pragma once

#include <QObject>
#include <QTimer>
#include "SpatialGrid.h"

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

signals:
    void colorsUpdated(); // Add this signal

private slots:
    void updateEffect();

private:
    void updateRadialFade();
    void updateWave();
    void updateRipple();
    void updateLayerCascade();

    SpatialGrid* grid;
    QTimer* timer;
    Effect currentEffect;
    float time;
    int speed;
    int intensity;
};