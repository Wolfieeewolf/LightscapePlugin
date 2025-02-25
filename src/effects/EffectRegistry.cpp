#include "effects/EffectRegistry.h"

namespace Lightscape {

EffectRegistry& EffectRegistry::getInstance()
{
    static EffectRegistry instance;
    return instance;
}

void EffectRegistry::registerEffect(const EffectInfo& info, EffectCreator creator)
{
    if (!info.id.isEmpty() && !hasEffect(info.id))
    {
        RegistryEntry entry{info, creator};
        registry.insert(info.id, entry);

        // Add to effect list
        EffectList::getInstance().addEffect(info);
    }
}

void EffectRegistry::unregisterEffect(const QString& effectId)
{
    if (hasEffect(effectId))
    {
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

} // namespace Lightscape