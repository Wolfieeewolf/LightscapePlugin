# Lightscape Spatial Effects System - Project Handover

*Project Repository: [https://github.com/Wolfieeewolf/LightscapePlugin](https://github.com/Wolfieeewolf/LightscapePlugin)*

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

We have completed Phase 1 and Phase 2 and fixed several important issues:

- ✓ Created the `BaseEffect` abstract class for all spatial effects
- ✓ Implemented a basic `TestEffect` that demonstrates position-based coloring
- ✓ Added the `EffectManager` to handle effect updates and application
- ✓ Updated the `EffectWidget` UI to work with the new effect system
- ✓ Enhanced state persistence through `SettingsManager` for reliable device assignment
- ✓ Restructured the project to use OpenRGB as a Git submodule for easier development
- ✓ Updated the codebase to match OpenRGB coding standards
- ✓ Fixed the plugin interface to match the required API
- ✓ Fixed the DLL naming issue (removed the version number from the filename)
- ✓ Fixed color display in the assignments widget (RGB vs BGR format)
- ✓ Reorganized the file structure to follow a more logical categorization by functionality

## Coding Standards

We follow these coding standards to maintain consistency with OpenRGB:

### 1. File Organization

- Headers (.h) go in the include/ directory
- Implementations (.cpp) go in the src/ directory
- Keep matching directory structures in include/ and src/
- Group related files in subdirectories based on functionality
- Each file should be in the directory that matches its purpose:
  - `core/` - Core plugin functionality
  - `devices/` - Device management
  - `grid/` - Spatial grid system
  - `effects/` - Effects system
  - `assignments/` - Device assignment UI

### 2. File Headers

All source and header files should start with this standard header:

```cpp
/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| fileName.h/cpp                                            |
|                                                           |
| Class description                                         |
\*---------------------------------------------------------*/
```

### 3. Naming Conventions

- **Member Variables**: Use snake_case with underscore prefix
  - Example: `_variable_name`, `_device_manager`
  
- **Function Parameters**: Use snake_case
  - Example: `parameter_name`, `device_index`
  
- **Class Names**: Use PascalCase
  - Example: `BaseEffect`, `EffectManager`
  
- **Method Names**: Use camelCase
  - Example: `getColorForPosition()`, `calculateDistance()`
  
- **File Names**: Use snake_case
  - Example: `base_effect.h`, `effect_manager.cpp`

### 4. Debug Output

Use printf-style debug output with consistent format:

```cpp
printf("[Lightscape][ModuleName] Message text.\n");
```

For example:
```cpp
printf("[Lightscape][EffectManager] Starting effect: %s\n", effect_id.toStdString().c_str());
```

### 5. Static Variables

Static variables should be declared at the top of implementation files:

```cpp
bool ClassName::StaticVariable = false;
ResourceManager* ClassName::RMPointer = nullptr;
```

### 6. OpenRGB Plugin Interface

The plugin must implement the OpenRGBPluginInterface with these methods:
```cpp
OpenRGBPluginInfo GetPluginInfo() override;
unsigned int GetPluginAPIVersion() override;
void Load(ResourceManagerInterface* resource_manager_ptr) override;
QWidget* GetWidget() override;
QMenu* GetTrayMenu() override;
void Unload() override;
```

### 7. Working with Colors

Always use the OpenRGB helper functions for color manipulation:
```cpp
// Creating colors (RGB order)
RGBColor color = ToRGBColor(red, green, blue);

// Extracting components
int red = RGBGetRValue(color);
int green = RGBGetGValue(color);
int blue = RGBGetBValue(color);
```

Do not manually construct RGB values with bitwise operations as this can lead to RGB/BGR confusion.

### 8. Code Structure

- Keep functions focused on a single task
- Keep function implementations under 50 lines when possible
- Add appropriate comments for complex logic
- Use the Q_UNUSED macro for unused parameters in Qt slots

## Project Structure

### Repository Structure

The project is set up as a standalone repository with OpenRGB as a Git submodule, with files organized by functionality:

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
│   ├── assignments/    # Assignment implementation
│   ├── core/           # Core implementation
│   ├── devices/        # Device implementation 
│   ├── effects/        # Effect implementation
│   └── grid/           # Grid implementation
├── ui/                 # UI definition files
├── resources.qrc       # Resource file
└── Lightscape.pro      # Project file
```

### Key Files to Understand

1. **include/core/LightscapePlugin.h & src/core/LightscapePlugin.cpp**
   - Main plugin class that implements the OpenRGBPluginInterface
   - Initializes all plugin components
   - Manages resources and plugin lifecycle

2. **include/devices/DeviceManager.h & src/devices/DeviceManager.cpp**
   - Manages all RGB and non-RGB devices
   - Handles device assignments and control

3. **include/effects/BaseEffect.h & src/effects/BaseEffect.cpp**
   - Abstract base class that all effects inherit from
   - Provides position-based color calculation
   - Handles common effect properties and device application

4. **include/effects/EffectManager.h & src/effects/EffectManager.cpp**
   - Singleton manager for effects
   - Controls update timing
   - Applies effects to devices

5. **include/effects/TestEffect/TestEffect.h & src/effects/TestEffect/TestEffect.cpp**
   - Example effect implementation
   - Shows basic spatial coloring based on grid position

6. **include/effects/EffectRegistry.h & src/effects/EffectRegistry.cpp**
   - Registration system for effects
   - Manages effect creation

7. **include/core/SettingsManager.h & src/core/SettingsManager.cpp**
   - Handles saving and loading of plugin state
   - Implements persistent device assignments across OpenRGB restarts
   - Uses device names and other identifiers for reliable persistence

8. **include/grid/SpatialGrid.h & src/grid/SpatialGrid.cpp**
   - 3D grid system for positioning devices
   - Maintains device assignments and positions

9. **include/grid/ReferencePointSelector.h & src/grid/ReferencePointSelector.cpp**
   - Manages reference points for spatial effects
   - Enables selection and configuration of reference points in 3D space

10. **include/assignments/AssignmentsWidget.h & src/assignments/AssignmentsWidget.cpp**
    - UI for managing device assignments
    - Handles displaying and modifying device assignments

## Phase-by-Phase Implementation Plan

### Phase 1: Base Effect Framework ✓ (COMPLETED)

- ✓ Create BaseEffect abstract class
- ✓ Implement the TestEffect as proof of concept
- ✓ Create EffectManager for timing and device application
- ✓ Update EffectWidget for UI integration
- ✓ Implement device assignment persistence across sessions
- ✓ Restructure to use OpenRGB as a submodule
- ✓ Standardize code to match OpenRGB style guidelines
- ✓ Fix plugin interface to match OpenRGB API
- ✓ Fix DLL naming issues
- ✓ Fix color display in assignment widget (RGB vs BGR format)

### Phase 2: Code Reorganization ✓ (COMPLETED)

- ✓ Reorganize files into logical directories based on functionality
- ✓ Move DeviceManager to devices/ directory
- ✓ Move ReferencePointSelector to grid/ directory
- ✓ Update include paths throughout the codebase
- ✓ Ensure clean compilation after reorganization

### Phase 3: Effect Manager Enhancements (NEXT PHASE)

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

### Phase 4: First Spatial Effect - SpatialRainbow

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

### Phase 5: Additional Spatial Effects

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

### Phase 6: Effect Layering and Composition

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

### Example of a New Effect (Updated Style)

```cpp
/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| NewEffect.h                                               |
|                                                           |
| Description of the new effect                             |
\*---------------------------------------------------------*/

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
    QSlider* _parameter_slider = nullptr;
    
    // Effect parameters
    int _parameter = 50;
};

REGISTER_EFFECT(NewEffect::GetStaticInfo(), NewEffect)

} // namespace Lightscape
```

```cpp
/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| NewEffect.cpp                                             |
|                                                           |
| Implementation of the new effect                          |
\*---------------------------------------------------------*/

#include "effects/NewEffect/NewEffect.h"
#include <QVBoxLayout>
#include <QLabel>

namespace Lightscape {

NewEffect::NewEffect(QWidget *parent)
    : BaseEffect(parent)
{
    printf("[Lightscape][NewEffect] Creating effect.\n");
    
    // Set up UI
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* label = new QLabel("Parameter:", this);
    _parameter_slider = new QSlider(Qt::Horizontal, this);
    _parameter_slider->setRange(0, 100);
    _parameter_slider->setValue(_parameter);
    
    layout->addWidget(label);
    layout->addWidget(_parameter_slider);
    
    setLayout(layout);
    
    // Connect signals
    connect(_parameter_slider, &QSlider::valueChanged, 
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
    float distance = calculateDistance(pos, _reference_point);
    
    // Example calculation using position and parameter
    int r = (pos.x * 20 + static_cast<int>(time * 50)) % 255;
    int g = (pos.y * 20 + static_cast<int>(time * 30)) % 255;
    int b = (_parameter * 2 + static_cast<int>(distance * 10)) % 255;
    
    return ToRGBColor(r, g, b);
}

void NewEffect::onParameterChanged(int value)
{
    _parameter = value;
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
   - Make sure to use OpenRGB helper functions for color operations (RGB vs BGR issues)

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

6. **Plugin not loading in OpenRGB**
   - Check that the DLL name is correct (should be "Lightscape.dll" without version number)
   - Verify API version matches OpenRGB's expectations
   - Ensure all dependencies are available

7. **printf format specifier errors**
   - Use %d for integers instead of %zu for size_t
   - Cast size_t to int: `static_cast<int>(container.size())`

8. **Include path errors**
   - Make sure all files use the correct include paths after reorganization
   - Include paths should match the directory structure (e.g., "devices/DeviceManager.h")

## Resources

- OpenRGB Documentation: https://gitlab.com/CalcProgrammer1/OpenRGB/-/wikis/home
- Qt Documentation: https://doc.qt.io/
- Color Theory: https://en.wikipedia.org/wiki/Color_theory

## Contact

For questions or clarification, reach out to the original developer [Your Contact Info].