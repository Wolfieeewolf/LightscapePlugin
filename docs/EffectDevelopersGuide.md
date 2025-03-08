# Lightscape Effect Developer's Guide

This guide explains how to create new effects for the Lightscape plugin. The system supports both traditional 2D effects (similar to OpenRGBEffectsPlugin) and spatial 3D effects that respond to device positions in a grid.

## New Dual-Interface System

The Lightscape effect system now supports two complementary interfaces:

1. **Spatial Interface (Lightscape-specific)** - Position-based color calculation
2. **Zone Interface (OpenRGBEffectsPlugin-compatible)** - Device/zone-based processing

Effects need only implement the spatial interface (`getColorForPosition()`) - the base class provides a default implementation for the zone interface that leverages your spatial calculations.

## Dual Interface Best Practices

The new system supports both spatial and zone-based interfaces. Here are some best practices for creating effects that work well with both:

### Implementing Only the Spatial Interface

The simplest approach is to implement only the required `getColorForPosition()` method and let the base class handle the rest:

```cpp
RGBColor MyEffect::getColorForPosition(const GridPosition& pos, float time)
{
    // Calculate color based on position
    // This will be used for both spatial and non-spatial zones
    return color;
}
```

The base class implementation of `StepEffect()` will:
1. Identify which zones are spatial vs. non-spatial
2. Use your `getColorForPosition()` for spatial zones
3. Create a reasonable fallback for non-spatial zones

### Customizing the Zone-Based Interface

If you need more control over non-spatial devices, you can override `StepEffect()`:

```cpp
void MyEffect::StepEffect(std::vector<ControllerZone*> zones)
{
    // Option 1: Use base implementation first, then add custom behavior
    BaseEffect::StepEffect(zones);
    
    // Add additional custom processing here
    
    // Option 2: Implement completely custom behavior
    /*
    for (auto* zone : zones) {
        SpatialControllerZone* spatial = dynamic_cast<SpatialControllerZone*>(zone);
        if (spatial) {
            // Handle spatial zone
            RGBColor color = getColorForPosition(spatial->position, time);
            spatial->setAllLEDs(color);
        } else {
            // Handle non-spatial zone differently
            // Example: special handling for keyboards, etc.
        }
    }
    */
}
```

### When to Override StepEffect

Override the `StepEffect` method when you need to:

1. Handle different device types differently (e.g., keyboards vs. strips)
2. Implement matrix-specific effects (e.g., using X/Y coordinates within a zone)
3. Provide special behavior for non-spatial devices that differs from spatial calculation
4. Optimize performance by using direct zone manipulation

### When to Keep the Default Implementation

Stick with the default implementation when:

1. Your effect works well with the same color calculation for all devices
2. You want to maintain simple, maintainable code
3. Your focus is primarily on spatial effects with basic support for non-spatial devices

The dual interface system is designed to make it easy to get started while providing flexibility for more advanced effects when needed.

## Effect Basics

### Effect Structure

Each effect consists of:

1. **Header (.h) file** - Class declaration and properties
2. **Implementation (.cpp) file** - Effect logic and color calculations
3. **UI (.ui) file** - Qt Designer file for the effect's settings panel
4. **Registration** - Code to register the effect with the system

### File Organization

Effects should be organized in the following directory structure:

```
effects/
├── Category/                  (e.g., Rainbow, Audio, Simple)
│   └── EffectName/
│       ├── EffectName.h
│       ├── EffectName.cpp
│       └── EffectName.ui
```

## Creating a New Effect

### Step 1: Create the Files

1. Create a new folder in the appropriate category directory
2. Create the header, implementation, and UI files

### Step 2: Set Up the Header File

```cpp
#pragma once

#include "effects/BaseEffect.h"

namespace Lightscape {

class MyNewEffect : public BaseEffect
{
    Q_OBJECT

public:
    explicit MyNewEffect(QWidget *parent = nullptr);
    ~MyNewEffect() = default;

    // Required method for spatial effects
    RGBColor getColorForPosition(const GridPosition& pos, float time) override;
    
    // Optional method for customizing non-spatial behavior (can use default)
    void StepEffect(std::vector<ControllerZone*> zones) override;
    
    // Effect information
    static EffectInfo GetStaticInfo();
    
    // Settings serialization
    void loadSettings(const QJsonObject& json) override;
    QJsonObject saveSettings() const override;

private slots:
    // Add slots for your UI controls
    void onSpeedChanged(int value);
    void onParameterChanged(int value);

private:
    Ui::MyNewEffect *ui;
    
    // Effect parameters
    int _parameter1 = 50;
    int _parameter2 = 10;
    
    // Helper methods
    RGBColor calculateColor(float x, float y, float z, float time);
};

// Register the effect
REGISTER_EFFECT(MyNewEffect::GetStaticInfo(), MyNewEffect, "Category")

} // namespace Lightscape
```

### Step 3: Implement the Effect

```cpp
#include "effects/Category/MyNewEffect/MyNewEffect.h"
#include "ui_MyNewEffect.h"
#include "ColorUtils.h"

namespace Lightscape {

MyNewEffect::MyNewEffect(QWidget *parent)
    : BaseEffect(parent)
    , ui(new Ui::MyNewEffect)
{
    ui->setupUi(this);
    
    // Connect UI signals
    connect(ui->parameterSlider, &QSlider::valueChanged,
            this, &MyNewEffect::onParameterChanged);
    
    // Set default values
    ui->parameterSlider->setValue(_parameter1);
    
    // Initialize colors if needed
    _user_colors.append(ColorUtils::RED);
    _user_colors.append(ColorUtils::BLUE);
}

MyNewEffect::~MyNewEffect()
{
    delete ui;
}

EffectInfo MyNewEffect::GetStaticInfo()
{
    EffectInfo info;
    info.name = QString("My New Effect");
    info.id = QString("my_new_effect");
    info.description = QString("Description of what the effect does");
    info.category = EffectCategory::Category;
    info.requiresReferencePoint = true;  // Set to true if the effect uses a reference point
    info.supportsPreview = true;
    return info;
}

RGBColor MyNewEffect::getColorForPosition(const GridPosition& pos, float time)
{
    // This is the key method for spatial effects
    // Calculate color based on 3D position and time
    
    // Example: Color based on distance from reference point
    float distance = calculateDistance(pos, _reference_point);
    
    // Apply parameters
    float adjusted_distance = distance * (_parameter1 / 50.0f);
    
    // Create pulsing effect based on time
    float pulse = (sin(time * _speed / 50.0f) + 1.0f) / 2.0f;
    
    // Adjust with distance
    float intensity = pulse * (1.0f - (adjusted_distance / 10.0f));
    intensity = std::max(0.0f, std::min(1.0f, intensity));
    
    // Use the user's selected color
    RGBColor base_color = _user_colors.isEmpty() ? ColorUtils::WHITE : _user_colors.first();
    
    // Apply intensity
    return applyBrightness(base_color, intensity);
}

void MyNewEffect::StepEffect(std::vector<ControllerZone*> zones)
{
    // Override this if you need custom behavior for non-spatial zones
    // Otherwise, the base implementation will use getColorForPosition for spatial zones
    // and apply a simplified effect for non-spatial zones
    
    // By default, just call the base implementation
    BaseEffect::StepEffect(zones);
}

void MyNewEffect::loadSettings(const QJsonObject& json)
{
    // Load base settings first
    BaseEffect::loadSettings(json);
    
    // Load effect-specific settings
    if (json.contains("parameter1"))
    {
        _parameter1 = json["parameter1"].toInt();
        ui->parameterSlider->setValue(_parameter1);
    }
    
    if (json.contains("parameter2"))
    {
        _parameter2 = json["parameter2"].toInt();
    }
}

QJsonObject MyNewEffect::saveSettings() const
{
    // Get base settings
    QJsonObject json = BaseEffect::saveSettings();
    
    // Add effect-specific settings
    json["parameter1"] = _parameter1;
    json["parameter2"] = _parameter2;
    
    return json;
}

void MyNewEffect::onParameterChanged(int value)
{
    _parameter1 = value;
    emit settingsChanged();
}

} // namespace Lightscape
```

### Step 4: Create the UI

Use Qt Designer to create the UI file (MyNewEffect.ui), or create it programmatically in the constructor.

The UI should include controls for all effect-specific parameters, such as:
- Sliders for speed, size, frequency, etc.
- Color selection widgets
- Option selectors (combo boxes)
- Checkboxes for toggles

### Step 5: Register the Effect

The effect is registered automatically by the `REGISTER_EFFECT` macro in the header file.

```cpp
REGISTER_EFFECT(MyNewEffect::GetStaticInfo(), MyNewEffect, "Category")
```

This adds the effect to the registry and makes it available in the UI.

## Spatial Effects

Lightscape specializes in spatial effects that respond to device positions in 3D space. Here are some techniques for creating effective spatial effects:

### Distance-Based Effects

Calculating the distance from a reference point:

```cpp
float distance = calculateDistance(pos, _reference_point);

// Create an effect that fades with distance
float intensity = 1.0f - (distance / max_distance);
intensity = std::max(0.0f, std::min(1.0f, intensity));
```

### Direction-Based Effects

Creating effects that use directional information:

```cpp
// Calculate direction vector
float dx = pos.x - _reference_point.x;
float dy = pos.y - _reference_point.y;
float dz = pos.z - _reference_point.z;

// Calculate angle in XZ plane (horizontal)
float angle = atan2(dz, dx);
```

### Wave Effects

Creating wave effects that travel through space:

```cpp
// Distance-based wave
float wave = sin(distance * _frequency + time * _speed);

// Directional wave (e.g., in X direction)
float wave = sin(pos.x * _frequency + time * _speed);
```

### RGB Gradient Effects

Creating smooth color transitions across space:

```cpp
// Gradient based on height (Y position)
float normalized_height = (pos.y - min_height) / (max_height - min_height);
RGBColor color = ColorUtils::interpolateColors(ColorUtils::BLUE, ColorUtils::RED, normalized_height);
```

## Best Practices

### Performance

1. **Minimize Calculations**
   - Precalculate values when possible
   - Use lookup tables for complex functions
   - Avoid unnecessary loops and complex math

2. **Smooth Transitions**
   - Use easing functions for smoother effects
   - Implement fade-in/fade-out when starting/stopping effects

3. **Fallbacks for Non-Spatial Devices**
   - Always provide fallback behavior for devices without spatial information
   - Use device indices or zones when spatial data isn't available

### UI Design

1. **Keep It Simple**
   - Include only necessary controls
   - Group related controls together
   - Use consistent naming and ranges

2. **Intuitive Controls**
   - Use sliders for numeric values with appropriate ranges
   - Use color pickers for color selection
   - Include tooltips for complex parameters

3. **Responsive Feedback**
   - Update the effect when parameters change
   - Provide visual feedback for control adjustments

### Settings

1. **Save All Parameters**
   - Make sure all effect-specific parameters are included in `saveSettings()`
   - Load them properly in `loadSettings()`

2. **Handle Defaults**
   - Set reasonable default values
   - Initialize UI controls with defaults
   - Reset to defaults when requested

## Examples

### Spatial Rainbow Effect

A rainbow effect that creates color patterns based on spatial position:

```cpp
RGBColor SpatialRainbow::getColorForPosition(const GridPosition& pos, float time)
{
    // Get the selected direction
    float position_value;
    switch(_direction)
    {
        case 0: position_value = pos.x; break; // X axis
        case 1: position_value = pos.y; break; // Y axis
        case 2: position_value = pos.z; break; // Z axis
        case 3: // Distance from reference point
            position_value = calculateDistance(pos, _reference_point);
            break;
    }
    
    // Calculate hue based on position and time
    float hue = fmodf((position_value / _wave_size) + (time * _speed / 100.0f), 1.0f);
    
    // Convert HSV to RGB
    return ColorUtils::HSVtoRGB(hue * 360.0f, 1.0f, 1.0f);
}
```

### Pulse Effect

A pulsing effect that emanates from a reference point:

```cpp
RGBColor PulseEffect::getColorForPosition(const GridPosition& pos, float time)
{
    // Calculate distance from reference point
    float distance = calculateDistance(pos, _reference_point);
    
    // Create moving wave based on distance and time
    float wave = sin(distance * _frequency - time * _speed);
    
    // Adjust to 0-1 range
    float intensity = (wave + 1.0f) / 2.0f;
    
    // Apply pulse width
    if (intensity < _pulse_width)
    {
        intensity = intensity / _pulse_width;
    }
    else
    {
        intensity = 0.0f;
    }
    
    // Use the user's selected color
    RGBColor base_color = _user_colors.isEmpty() ? ColorUtils::WHITE : _user_colors.first();
    
    // Apply intensity
    return applyBrightness(base_color, intensity);
}
```

## Troubleshooting

### Common Issues

1. **Effect not appearing in the list**
   - Check that the `REGISTER_EFFECT` macro is included
   - Verify the namespace is correct
   - Make sure the effect is compiled and linked

2. **UI controls not updating effect**
   - Verify signal/slot connections
   - Make sure to emit `settingsChanged()` when parameters change
   - Check that UI control values are being properly set

3. **Effect not working with spatial positions**
   - Ensure `getColorForPosition()` is properly implemented
   - Check if the effect requires a reference point and one is set
   - Verify that devices have spatial positions assigned

4. **Performance issues**
   - Simplify math calculations
   - Reduce the update frequency (FPS)
   - Check for inefficient loops or repeated calculations

### Debugging Tips

1. **Logging**
   - Use `printf("[Lightscape][MyEffect] Debug info: %f\n", value);` for debug output
   - Log parameter changes and calculations

2. **Visual Debugging**
   - Add explicit colors to indicate different states
   - Create a debug mode that shows position information

3. **Testing**
   - Test with a single device first
   - Verify with both spatial and non-spatial devices
   - Check performance with increasing numbers of devices

## API Reference

### BaseEffect

Key methods and properties of the BaseEffect class:

```cpp
// Core spatial interface - required to implement
RGBColor getColorForPosition(const GridPosition& pos, float time);

// Zone-based interface - optional to override
void StepEffect(std::vector<ControllerZone*> zones);
void OnControllerZonesListChanged(std::vector<ControllerZone*> zones);

// Initialization and lifecycle
void initialize(DeviceManager* deviceManager, SpatialGrid* grid);
void update(float deltaTime);
void start();
void stop();

// Settings
void loadSettings(const QJsonObject& json);
QJsonObject saveSettings() const;

// Effect control
void setEnabled(bool enabled);
bool getEnabled() const;

// Common properties
void setSpeed(int value);
int getSpeed() const;
void setBrightness(int value);
int getBrightness() const;
void setReferencePoint(const GridPosition& pos);
GridPosition getReferencePoint() const;
void setColors(const QList<RGBColor>& colors);
QList<RGBColor> getColors() const;
void setRandomColors(bool value);
bool getRandomColors() const;
void setFPS(unsigned int value);
unsigned int getFPS() const;

// Helper methods
float calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const;
RGBColor applyBrightness(const RGBColor& color, float brightness_factor) const;
```

### SpatialControllerZone

The new adapter class that bridges between spatial and traditional zones:

```cpp
class SpatialControllerZone : public ControllerZone {
public:
    // Constructor
    SpatialControllerZone(int device_index, int zone_index, const GridPosition& position, DeviceManager* device_manager);
    
    // Core data
    int deviceIndex;
    int zoneIndex;
    GridPosition position;
    DeviceManager* deviceManager;
    
    // Spatial operations
    float getDistanceFrom(const GridPosition& reference) const;
    float getAngleFrom(const GridPosition& reference, int axis) const;
    
    // ControllerZone interface implementation
    unsigned int getLEDCount() const override;
    void setLED(int led_idx, RGBColor color, int brightness = 100, int temperature = 0, int tint = 0) override;
    void setAllLEDs(RGBColor color, int brightness = 100, int temperature = 0, int tint = 0) override;
    bool isMatrix() const override;
    unsigned int getMatrixWidth() const override;
    unsigned int getMatrixHeight() const override;
    ZoneType getZoneType() const override;
    
    // Factory methods
    static SpatialControllerZone* fromDeviceInfo(const DeviceInfo& info, DeviceManager* device_manager);
};
```

### ColorUtils

Useful color manipulation functions:

```cpp
// Basic colors
static const RGBColor RED;
static const RGBColor GREEN;
static const RGBColor BLUE;
static const RGBColor WHITE;
static const RGBColor BLACK;

// Color conversions
static RGBColor HSVtoRGB(float hue, float saturation, float value);
static void RGBtoHSV(RGBColor color, float& hue, float& saturation, float& value);

// Color manipulation
static RGBColor interpolateColors(RGBColor color1, RGBColor color2, float factor);
static RGBColor applyBrightness(RGBColor color, float brightness);
static RGBColor randomColor();
```

### GridPosition

3D position structure:

```cpp
struct GridPosition {
    float x;
    float y;
    float z;
    
    GridPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    
    float distanceTo(const GridPosition& other) const;
    
    // Operators
    bool operator==(const GridPosition& other) const;
    bool operator!=(const GridPosition& other) const;
    GridPosition operator+(const GridPosition& other) const;
    GridPosition operator-(const GridPosition& other) const;
    GridPosition operator*(float scalar) const;
    GridPosition operator/(float scalar) const;
};
```