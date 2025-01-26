#pragma once
#include "EffectBase.h"
#include <QObject>
#include <QMap>

class EffectsEngine : public QObject {
    Q_OBJECT
public:
    explicit EffectsEngine(QObject *parent = nullptr);
    
    void registerEffect(EffectBase* effect);
    void removeEffect(const QString& effectName);
    void updateEffects(qint64 deltaTime);
    QVector<QColor> getCurrentColors();

signals:
    void effectsUpdated();

private:
    QMap<QString, EffectBase*> effects;
};