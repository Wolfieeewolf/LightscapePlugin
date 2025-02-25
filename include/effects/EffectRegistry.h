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
    void registerEffect(const EffectInfo& info, EffectCreator creator);
    void unregisterEffect(const QString& effectId);

    // Effect creation
    void* createEffect(const QString& effectId);
    bool hasEffect(const QString& effectId) const;

private:
    EffectRegistry() = default;
    ~EffectRegistry() = default;

    // Prevent copying
    EffectRegistry(const EffectRegistry&) = delete;
    EffectRegistry& operator=(const EffectRegistry&) = delete;

    struct RegistryEntry {
        EffectInfo info;
        EffectCreator creator;
    };

    QMap<QString, RegistryEntry> registry;
};

// Registration helper macro
#define REGISTER_EFFECT(info, type) \
    static bool registered_##type = []() { \
        EffectRegistry::getInstance().registerEffect(info, []() -> void* { return new type(); }); \
        return true; \
    }();

} // namespace Lightscape