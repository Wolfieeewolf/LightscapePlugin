# Lightscape Spatial Effects System - Project Handover

## Project Overview

The Lightscape plugin for OpenRGB is being enhanced with a spatial effects system that will allow RGB devices to respond to their position in 3D space. Unlike traditional effects systems that treat devices in isolation, this system will create cohesive lighting environments where effects can flow naturally through physical space based on device positions in a 3D grid.

## Project Goals

1. Create a flexible spatial effects framework that leverages the existing grid system
2. Allow effects to utilize 3D positional data and reference points
3. Make it easy for developers to create new effects
4. Support both RGB and non-RGB devices in the spatial environment
5. Provide an interactive preview system

## Key Principles

When working on this project, adhere to these principles:

1. **KISS (Keep It Simple, Stupid)**: Maintain simplicity in design and implementation
2. **DRY (Don't Repeat Yourself)**: Avoid code duplication
3. **SRP (Single Responsibility Principle)**: Each class should have one reason to change
4. **YAGNI (You Aren't Gonna Need It)**: Don't implement features until they're necessary

## Current Status

We have completed Phase 1 of the implementation and made several key enhancements:

- ✓ Created the `BaseEffect` abstract class for all spatial effects
- ✓ Implemented a basic `TestEffect` that demonstrates position-based coloring
- ✓ Added the `EffectManager` to handle effect updates and application
- ✓ Updated the `EffectWidget` UI to work with the new effect system
- ✓ Enhanced state persistence through `SettingsManager` for reliable device assignment
- ✓ Restructured the project to use OpenRGB as a Git submodule for easier development

## Project Structure

### Repository Structure

The project is now set up as a standalone repository with OpenRGB as a Git submodule:

```
LightscapePlugin/
├── docs/               # Documentation
├── images/             # Image resources
├── include/            # Header files organized by module
│   ├── assignments/    # Device assignment UI
│   ├── core/           # Core plugin functionality
│   ├── devices/        # Device management
│   ├── effects/        # Effects system
│   └── grid/           # Spatial grid system
├── OpenRGB/            # OpenRGB submodule
├── src/                # Source files (same structure as include)
├── ui/                 # UI definition files
├── resources.qrc       # Resource file
└── Lightscape.pro      # Project file
```

### Key Files to Understand

1. **include/effects/BaseEffect.h & src/effects/BaseEffect.cpp**
   - Abstract base class that all effects inherit from
   - Provides position-based color calculation
   - Handles common effect properties and device application

2. **include/effects/EffectManager.h & src/effects/EffectManager.cpp**
   - Singleton manager for effects
   - Controls update timing
   - Applies effects to devices

3. **include/effects/TestEffect/TestEffect.h & src/effects/TestEffect/TestEffect.cpp**
   - Example effect implementation
   - Shows basic spatial coloring based on grid position

4. **include/effects/EffectRegistry.h & src/effects/EffectRegistry.cpp**
   - Registration system for effects
   - Manages effect creation

5. **include/core/SettingsManager.h & src/core/SettingsManager.cpp**
   - Handles saving and loading of plugin state
   - Implements persistent device assignments across OpenRGB restarts
   - Uses device names and other identifiers for reliable persistence

6. **include/grid/SpatialGrid.h & src/grid/SpatialGrid.cpp**
   - 3D grid system for positioning devices
   - Maintains device assignments and positions

7. **include/core/DeviceManager.h & src/core/DeviceManager.cpp**
   - Manages device communication
   - Provides methods to set colors on devices

## Phase-by-Phase Implementation Plan

### Phase 1: Base Effect Framework ✓ (COMPLETED)

- ✓ Create BaseEffect abstract class
- ✓ Implement the TestEffect as proof of concept
- ✓ Create EffectManager for timing and device application
- ✓ Update EffectWidget for UI integration
- ✓ Implement device assignment persistence across sessions
- ✓ Restructure to use OpenRGB as a submodule

### Phase 2: Effect Manager Enhancements (NEXT PHASE)

This phase focuses on improving the EffectManager to handle more sophisticated effect control:

1. **Add Preview System**
   - Create a PreviewRenderer class that renders effects in a preview panel
   - Update EffectManager to send preview updates

2. **Implement Effect Parameter Management**
   - Create a framework for effect parameters
   - Add parameter serialization/deserialization for saving/loading presets

3. **Add Event System**
   - Create an event system for effects to respond to external triggers
   - Implement basic event types (time-based, device-based)

**Files to Modify:**
- include/effects/EffectManager.h & src/effects/EffectManager.cpp
- include/effects/BaseEffect.h & src/effects/BaseEffect.cpp
- Create new: include/effects/PreviewRenderer.h & src/effects/PreviewRenderer.cpp

### Phase 3: First Spatial Effect - SpatialRainbow

Implement the first full spatial effect that demonstrates the system's capabilities:

1. **Create SpatialRainbow Effect**
   - Position-based rainbow coloring
   - Distance-from-reference calculation
   - Direction controls
   - Color gradient customization

2. **Add Effect-Specific UI Controls**
   - Speed slider
   - Wave size control
   - Direction selector
   - Color editor

**Files to Create:**
- include/effects/SpatialRainbow/SpatialRainbow.h
- src/effects/SpatialRainbow/SpatialRainbow.cpp
- ui/effects/SpatialRainbow.ui (optional)

### Phase 4: Additional Spatial Effects

Expand the system with more spatial effects:

1. **Spatial Wave Effect**
   - Rippling waves emanating from reference point
   - Amplitude and frequency controls

2. **Height Map Effect**
   - Colors based on vertical (Y) position
   - Gradient customization

3. **Source-Based Effects**
   - Effects that emanate from non-RGB devices (like monitors)
   - Screen content color sampling

**Files to Create:**
- Appropriate .h and .cpp files for each effect in subdirectories
- Corresponding UI files if needed

### Phase 5: Effect Layering and Composition

Enable multiple effects to be combined:

1. **Layer Management**
   - Create a system for effect layers
   - Implement blend modes between layers

2. **Effect Groups**
   - Allow grouping of devices for different effects
   - Zone-specific effects

3. **Effect Sequences**
   - Time-based sequences of effects
   - Transitions between effects

**Files to Create:**
- include/effects/EffectLayer.h & src/effects/EffectLayer.cpp
- include/effects/EffectComposer.h & src/effects/EffectComposer.cpp

## Implementation Guidelines

### Creating a New Effect

1. **Create the effect class files**
   - Put them in a subdirectory of effects/ named after the effect
   - Inherit from BaseEffect
   - Implement the getColorForPosition method

2. **Register the effect**
   - Use the REGISTER_EFFECT macro
   - Provide static info via GetStaticInfo()

3. **Add UI controls**
   - Create controls in the constructor
   - Connect signals to update parameters

### Working with RGBColor

Note that OpenRGB's RGBColor is an unsigned int, not a struct with r/g/b members. Always use the helper functions:

- `ToRGBColor(r, g, b)` to create a color
- `RGBGetRValue(color)` to extract the red component
- `RGBGetGValue(color)` to extract the green component
- `RGBGetBValue(color)` to extract the blue component

### Example of a New Effect

```cpp
// include/effects/NewEffect/NewEffect.h
#pragma once

#include "effects/BaseEffect.h"

namespace Lightscape {

class NewEffect : public BaseEffect
{
    Q_OBJECT

public:
    explicit NewEffect(QWidget *parent = nullptr);
    ~NewEffect() = default;

    static EffectInfo GetStaticInfo();
    RGBColor getColorForPosition(const GridPosition& pos, float time) override;

private slots:
    void onParameterChanged(int value);

private:
    // UI controls
    QSlider* parameterSlider;
    
    // Effect parameters
    int parameter = 50;
};

REGISTER_EFFECT(NewEffect::GetStaticInfo(), NewEffect)

} // namespace Lightscape
```

```cpp
// src/effects/NewEffect/NewEffect.cpp
#include "effects/NewEffect/NewEffect.h"
#include <QVBoxLayout>
#include <QLabel>

namespace Lightscape {

NewEffect::NewEffect(QWidget *parent)
    : BaseEffect(parent)
{
    // Set up UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* label = new QLabel("Parameter:", this);
    parameterSlider = new QSlider(Qt::Horizontal, this);
    parameterSlider->setRange(0, 100);
    parameterSlider->setValue(parameter);
    
    layout->addWidget(label);
    layout->addWidget(parameterSlider);
    
    setLayout(layout);
    
    // Connect signals
    connect(parameterSlider, &QSlider::valueChanged, 
            this, &NewEffect::onParameterChanged);
}

EffectInfo NewEffect::GetStaticInfo()
{
    EffectInfo info;
    info.name = "New Effect";
    info.id = "new_effect";
    info.description = "Description of the new effect";
    info.category = EffectCategory::Spatial;
    info.requiresReferencePoint = true;
    info.supportsPreview = true;
    return info;
}

RGBColor NewEffect::getColorForPosition(const GridPosition& pos, float time)
{
    // Calculate color based on position and parameters
    float distance = calculateDistance(pos, referencePoint);
    
    // Example calculation using position and parameter
    int r = (pos.x * 20 + static_cast<int>(time * 50)) % 255;
    int g = (pos.y * 20 + static_cast<int>(time * 30)) % 255;
    int b = (parameter * 2 + static_cast<int>(distance * 10)) % 255;
    
    return ToRGBColor(r, g, b);
}

void NewEffect::onParameterChanged(int value)
{
    parameter = value;
}

} // namespace Lightscape
```

## Building the Project

### With OpenRGB Submodule (Standalone)

1. Clone the repository with the submodule:
   ```
   git clone [repository-url]
   cd LightscapePlugin
   git submodule init
   git submodule update
   ```

2. Open the project in Qt Creator:
   ```
   open Lightscape.pro
   ```

3. Build the plugin:
   - The compiled plugin will be placed in the OpenRGB plugins directory
   - For Windows: `%APPDATA%\OpenRGB\plugins`
   - For Linux: `~/.config/OpenRGB/plugins`
   - For macOS: `~/Library/Application Support/OpenRGB/plugins`

### Updating the OpenRGB Submodule

To update the OpenRGB submodule to the latest version:

```
cd OpenRGB
git pull origin master
cd ..
git add OpenRGB
git commit -m "Update OpenRGB submodule"
```

## Testing Approach

For each phase and implementation:

1. **Unit Testing**
   - Test individual effects and components in isolation
   - Verify correct color calculation

2. **Integration Testing**
   - Test effect behavior in the full system
   - Verify device application and preview rendering

3. **UI Testing**
   - Test user interface interaction
   - Verify controls affect effects properly

## Troubleshooting

Common issues and solutions:

1. **Effect not showing in list**
   - Check registration macro is correctly placed
   - Verify effect class and header are added to project file

2. **Colors not updating**
   - Check EffectManager update loop
   - Verify device assignments in SpatialGrid

3. **UI controls not working**
   - Ensure signals are connected
   - Verify parameter updates trigger effect changes

4. **Device assignments lost after restart**
   - Check if SettingsManager is properly saving device names
   - Verify the settings file exists in the plugins directory

5. **RGBColor errors**
   - Remember that RGBColor is an unsigned int, not a struct
   - Use ToRGBColor() and RGBGetXValue() functions
   - Cast to int when assigning to JSON objects: `static_cast<int>(RGBGetRValue(color))`

## Resources

- OpenRGB Documentation: https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/home
- Qt Documentation: https://doc.qt.io/
- Color Theory: https://en.wikipedia.org/wiki/Color_theory

## Contact

For questions or clarification, reach out to the original developer [Your Contact Info].
