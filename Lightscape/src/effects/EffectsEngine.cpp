#include "effects/EffectsEngine.h"

EffectsEngine::EffectsEngine(QObject *parent)
    : QObject(parent)
{
}

void EffectsEngine::registerEffect(EffectBase* effect)
{
    if (effect && !effects.contains(effect->getName())) {
        effects[effect->getName()] = effect;
    }
}

void EffectsEngine::removeEffect(const QString& effectName)
{
    if (effects.contains(effectName)) {
        delete effects[effectName];
        effects.remove(effectName);
    }
}

void EffectsEngine::updateEffects(qint64 deltaTime)
{
    for (auto effect : effects) {
        if (effect->isActive()) {
            effect->update(deltaTime);
        }
    }
    emit effectsUpdated();
}

QVector<QColor> EffectsEngine::getCurrentColors()
{
    QVector<QColor> colors;
    // TODO: Implement blending of active effects
    for (auto effect : effects) {
        if (effect->isActive()) {
            auto effectColors = effect->getColors();
            // For now, just return colors from first active effect
            if (!effectColors.isEmpty()) {
                return effectColors;
            }
        }
    }
    return colors;
}