#pragma once

#include "effects/EffectRegistry.h"
#include "effects/BaseEffect.h"

namespace Lightscape {

class TestEffect : public BaseEffect
{
    Q_OBJECT

public:
    explicit TestEffect(QWidget *parent = nullptr);
    ~TestEffect() = default;

    static std::string const ClassName() { return "TestEffect"; }
    static EffectInfo GetStaticInfo();
    
    // Implement the required virtual method for spatial color calculation
    RGBColor getColorForPosition(const GridPosition& pos, float time) override;
}; 

// Register effect outside the class definition
REGISTER_EFFECT(TestEffect::GetStaticInfo(), TestEffect)

} // namespace Lightscape