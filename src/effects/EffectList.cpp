#include "effects/EffectList.h"

namespace Lightscape {

EffectList& EffectList::getInstance()
{
    static EffectList instance;
    return instance;
}

void EffectList::addEffect(const EffectInfo& info)
{
    if (!info.id.isEmpty() && !hasEffect(info.id))
    {
        effects[info.id] = info;
        emit effectAdded(info);
    }
}

void EffectList::removeEffect(const QString& effectId)
{
    if (hasEffect(effectId))
    {
        effects.remove(effectId);
        emit effectRemoved(effectId);
    }
}

bool EffectList::hasEffect(const QString& effectId) const
{
    return effects.contains(effectId);
}

EffectInfo EffectList::getEffect(const QString& effectId) const
{
    return effects.value(effectId);
}

QList<EffectInfo> EffectList::getEffects() const
{
    return effects.values();
}

QList<EffectInfo> EffectList::getEffectsByCategory(EffectCategory category) const
{
    QList<EffectInfo> result;
    for (const auto& effect : effects)
    {
        if (effect.category == category)
        {
            result.append(effect);
        }
    }
    return result;
}

QStringList EffectList::getCategories() const
{
    QSet<QString> categories;
    for (const auto& effect : effects)
    {
        // Convert enum to display string
        QString category;
        switch (effect.category)
        {
            case EffectCategory::Spatial:
                category = "Spatial";
                break;
            case EffectCategory::Basic:
                category = "Basic";
                break;
            case EffectCategory::Advanced:
                category = "Advanced";
                break;
            case EffectCategory::Custom:
                category = "Custom";
                break;
        }
        categories.insert(category);
    }
    return QStringList(categories.values());
}

} // namespace Lightscape