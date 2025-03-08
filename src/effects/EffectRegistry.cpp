#include "effects/EffectRegistry.h"

namespace Lightscape {

EffectRegistry& EffectRegistry::getInstance()
{
    static EffectRegistry instance;
    return instance;
}

void EffectRegistry::registerEffect(const EffectInfo& info, EffectCreator creator, const QString& category)
{
    if (!info.id.isEmpty() && !hasEffect(info.id))
    {
        // Create the registry entry
        RegistryEntry entry{info, creator, category};
        registry.insert(info.id, entry);

        // Add to effect list
        EffectList::getInstance().addEffect(info);
        
        // Add to categorized effects
        QString effectCategory = category.isEmpty() ? QString::fromUtf8("Uncategorized") : category;
        
        // Create category if it doesn't exist
        if (!categorizedEffects.contains(effectCategory))
        {
            categorizedEffects.insert(effectCategory, QList<EffectInfo>());
        }
        
        // Add effect to the category
        categorizedEffects[effectCategory].append(info);
    }
}

void EffectRegistry::unregisterEffect(const QString& effectId)
{
    if (hasEffect(effectId))
    {
        RegistryEntry entry = registry.value(effectId);
        
        // Remove from category list
        QString category = entry.category.isEmpty() ? QString::fromUtf8("Uncategorized") : entry.category;
        if (categorizedEffects.contains(category))
        {
            QList<EffectInfo>& effects = categorizedEffects[category];
            for (int i = 0; i < effects.size(); i++)
            {
                if (effects[i].id == effectId)
                {
                    effects.removeAt(i);
                    break;
                }
            }
            
            // Remove category if empty
            if (effects.isEmpty())
            {
                categorizedEffects.remove(category);
            }
        }
        
        // Remove from main registry
        registry.remove(effectId);
        EffectList::getInstance().removeEffect(effectId);
    }
}

void* EffectRegistry::createEffect(const QString& effectId)
{
    if (hasEffect(effectId))
    {
        return registry.value(effectId).creator();
    }
    return nullptr;
}

bool EffectRegistry::hasEffect(const QString& effectId) const
{
    return registry.contains(effectId);
}

QList<QString> EffectRegistry::getCategories() const
{
    return categorizedEffects.keys();
}

QList<EffectInfo> EffectRegistry::getEffectsInCategory(const QString& category) const
{
    if (categorizedEffects.contains(category))
    {
        return categorizedEffects.value(category);
    }
    return QList<EffectInfo>();
}

QMap<QString, QList<EffectInfo>> EffectRegistry::getCategorizedEffects() const
{
    return categorizedEffects;
}

} // namespace Lightscape