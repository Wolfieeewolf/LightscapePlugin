#include "effects/TestEffect/TestEffect.h"
#include "effects/SpatialControllerZone.h"

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

void TestEffect::StepEffect(std::vector<ControllerZone*> zones)
{
    // Call the base class StepEffect to use our getColorForPosition method
    // This demonstrates using the existing spatial calculation for both interfaces
    BaseEffect::StepEffect(zones);
    
    // If we wanted custom behavior for non-spatial zones, we could add it here
    // For example, we could identify special devices and treat them differently
}

} // namespace Lightscape