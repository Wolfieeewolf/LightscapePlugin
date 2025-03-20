#include "effects/EffectRegistry.h"

namespace Lightscape {

EffectRegistry::EffectRegistry()
{
    // Print debug message on initialization
    printf("[Lightscape][EffectRegistry] EffectRegistry initialized\n");
}

EffectRegistry& EffectRegistry::getInstance()
{
    static EffectRegistry instance;
    return instance;
}

void EffectRegistry::registerEffect(const EffectInfo& info, EffectCreator creator, const QString& category)
{
    printf("[Lightscape][EffectRegistry] Registering effect: %s\n", info.id.toStdString().c_str());
    
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
        
        printf("[Lightscape][EffectRegistry] Successfully registered effect: %s in category: %s\n", 
               info.id.toStdString().c_str(), effectCategory.toStdString().c_str());
    }
    else {
        printf("[Lightscape][EffectRegistry] Failed to register effect: %s (id empty: %s, already exists: %s)\n", 
               info.id.toStdString().c_str(), 
               info.id.isEmpty() ? "true" : "false", 
               hasEffect(info.id) ? "true" : "false");
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
    printf("[Lightscape][EffectRegistry] Creating effect: %s\n", effectId.toStdString().c_str());
    
    // Debug: print all registered effects
    printf("[Lightscape][EffectRegistry] Available effects:\n");
    for (auto it = registry.begin(); it != registry.end(); ++it) {
        printf("  - %s\n", it.key().toStdString().c_str());
    }
    
    if (hasEffect(effectId))
    {
        void* result = registry.value(effectId).creator();
        printf("[Lightscape][EffectRegistry] Effect created: %s, result: %p\n", 
               effectId.toStdString().c_str(), result);
        return result;
    }
    
    printf("[Lightscape][EffectRegistry] Effect not found: %s\n", effectId.toStdString().c_str());
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