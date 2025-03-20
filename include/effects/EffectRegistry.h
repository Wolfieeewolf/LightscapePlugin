#pragma once

#include <functional>
#include <QMap>
#include "effects/EffectList.h"

namespace Lightscape {

class EffectRegistry
{
public:
    using EffectCreator = std::function<void*()>;

    static EffectRegistry& getInstance();

    // Registration
    void registerEffect(const EffectInfo& info, EffectCreator creator, const QString& category = "");
    void unregisterEffect(const QString& effectId);

    // Effect creation
    void* createEffect(const QString& effectId);
    bool hasEffect(const QString& effectId) const;
    
    // Categorization
    QList<QString> getCategories() const;
    QList<EffectInfo> getEffectsInCategory(const QString& category) const;
    QMap<QString, QList<EffectInfo>> getCategorizedEffects() const;

private:
    EffectRegistry();
    ~EffectRegistry() = default;

    // Prevent copying
    EffectRegistry(const EffectRegistry&) = delete;
    EffectRegistry& operator=(const EffectRegistry&) = delete;

    struct RegistryEntry {
        EffectInfo info;
        EffectCreator creator;
        QString category;
    };

    QMap<QString, RegistryEntry> registry;
    QMap<QString, QList<EffectInfo>> categorizedEffects;
};

// Registration helper macro
#define REGISTER_EFFECT(info, type, category) \
    static bool registered_##type = []() { \
        EffectRegistry::getInstance().registerEffect(info, []() -> void* { return new type(); }, category); \
        return true; \
    }();

} // namespace Lightscape