#include "effects/TestEffect/TestEffect.h"

namespace Lightscape {

TestEffect::TestEffect(QWidget *parent)
    : BaseEffect(parent)
{
    // Add any TestEffect-specific initialization here
}

EffectInfo TestEffect::GetStaticInfo()
{
    EffectInfo info;
    info.name = QString("Test Effect");
    info.id = QString("test_effect");
    info.description = QString("A simple test effect that changes color based on position");
    info.category = EffectCategory::Basic;
    info.requiresReferencePoint = false;
    info.supportsPreview = true;
    return info;
}

RGBColor TestEffect::getColorForPosition(const GridPosition& pos, float time)
{
    // Simple test color calculation based on position and time
    int r = (pos.x * 20 + static_cast<int>(time * 50)) % 255;
    int g = (pos.y * 20 + static_cast<int>(time * 30)) % 255;
    int b = (pos.z * 20 + static_cast<int>(time * 70)) % 255;
    
    return ToRGBColor(r, g, b);
}

} // namespace Lightscape