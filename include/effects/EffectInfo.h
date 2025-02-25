#pragma once

#include <QString>

namespace Lightscape {

enum class EffectCategory {
    Spatial,     // Effects that work with spatial grid
    Basic,       // Simple effects like static color
    Advanced,    // Complex effects
    Custom       // User-created effects
};

struct EffectInfo {
    QString name;                // Display name
    QString id;                  // Unique identifier
    QString description;         // Effect description
    EffectCategory category;     // Effect category
    bool requiresReferencePoint; // Whether effect needs reference point
    bool supportsPreview;        // Whether effect can be previewed

    EffectInfo() 
        : name("")
        , id("")
        , description("")
        , category(EffectCategory::Basic)
        , requiresReferencePoint(false)
        , supportsPreview(true)
    {}
};

} // namespace Lightscape