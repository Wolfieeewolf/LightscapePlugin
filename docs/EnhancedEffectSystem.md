# Lightscape Enhanced Effect System

*Project Repository: [https://github.com/Wolfieeewolf/LightscapePlugin](https://github.com/Wolfieeewolf/LightscapePlugin)*

This document details the enhanced effect system implemented in Phase 3 of the Lightscape development roadmap. The system now supports both spatial effects (specific to Lightscape) and traditional effects (compatible with OpenRGBEffectsPlugin).

## System Architecture

### Core Components

1. **BaseEffect Class**
   - Provides a unified interface for all effects
   - Supports both spatial and non-spatial interfaces
   - Automatically handles different types of devices/zones

2. **SpatialControllerZone Adapter**
   - Bridges between OpenRGB's device/zone system and Lightscape's spatial grid
   - Implements the ControllerZone interface
   - Stores position information for spatial effects

3. **EffectManager**
   - Manages effect lifecycle (creation, update, application, destruction)
   - Handles both spatial and non-spatial zones
   - Supports optional threading for better performance
   - Implements preview functionality

4. **EffectRegistry**
   - Registers effects with category support
   - Creates effect instances on demand
   - Organizes effects into logical groups

## Integration Approach

The enhanced system enables developers to:

1. Create effects that work in both Lightscape's spatial environment and traditional RGB setups
2. Leverage a single codebase for different interfaces
3. Focus primarily on the spatial color calculation while getting reasonable defaults for non-spatial devices

## BaseEffect Interface

The BaseEffect class now provides two primary interfaces:

### 1. Spatial Interface (Required)

```cpp
RGBColor getColorForPosition(const GridPosition& pos, float time);
```

This is the core method that all effects must implement. It calculates a color based on:
- 3D position in the grid
- Current time
- Effect parameters

### 2. OpenRGBEffectsPlugin-style Interface (Optional)

```cpp
void StepEffect(std::vector<ControllerZone*> zones);
void OnControllerZonesListChanged(std::vector<ControllerZone*> zones);
```

These methods provide compatibility with OpenRGBEffectsPlugin's approach:
- `StepEffect` - Called on each update to process all zones
- `OnControllerZonesListChanged` - Called when zones are added/removed

The default implementation of `StepEffect` automatically:
1. Separates zones into spatial and non-spatial types
2. Uses `getColorForPosition` for spatial zones
3. Provides reasonable fallback for non-spatial zones

## SpatialControllerZone Adapter

This class bridges between traditional zone-based control and spatial positioning:

```cpp
class SpatialControllerZone : public ControllerZone {
    // Core data
    int deviceIndex;
    int zoneIndex;
    GridPosition position;
    
    // Spatial operations
    float getDistanceFrom(const GridPosition& reference);
    float getAngleFrom(const GridPosition& reference, int axis);
    
    // Color operations
    void setLED(int led_idx, RGBColor color, int brightness);
    void setAllLEDs(RGBColor color, int brightness);
};
```

Key features:
- Implements the ControllerZone interface for compatibility
- Stores spatial position information
- Provides spatial operations like distance and angle calculations
- Maps between zone/LED indices and spatial positions

## Effect Development Workflow

### Basic Approach

For the simplest case, developers only need to:

1. Implement `getColorForPosition()`
2. Register the effect with a category

The base class handles everything else, providing decent fallback behavior for non-spatial zones.

### Advanced Approach

For more control, developers can:

1. Override `StepEffect()` to handle specific zone types differently
2. Implement custom non-spatial logic while using spatial logic for compatible devices
3. Override `OnControllerZonesListChanged()` for setup actions when zones change

## Example Implementation

```cpp
// SpatialRainbow.h
class SpatialRainbow : public BaseEffect
{
public:
    // Required spatial interface
    RGBColor getColorForPosition(const GridPosition& pos, float time) override;
    
    // Optional: Custom handling for non-spatial devices
    void StepEffect(std::vector<ControllerZone*> zones) override;
};

// SpatialRainbow.cpp
RGBColor SpatialRainbow::getColorForPosition(const GridPosition& pos, float time)
{
    // Calculate color based on position and time
    float position_value = pos.x; // Use X-axis for position
    float hue = fmodf((position_value / wave_size) + time, 1.0f);
    return ColorUtils::HSVtoRGB(hue * 360.0f, 1.0f, 1.0f);
}

void SpatialRainbow::StepEffect(std::vector<ControllerZone*> zones)
{
    // We can use the default implementation, which will:
    // - Use getColorForPosition() for spatial zones
    // - Create a simplified rainbow for non-spatial zones
    BaseEffect::StepEffect(zones);
    
    // Or we could implement custom behavior for specific zone types
}
```

## Effect Registration with Categories

Effects are now registered with a category to improve organization:

```cpp
REGISTER_EFFECT(SpatialRainbow::GetStaticInfo(), SpatialRainbow, "Rainbow")
```

This creates a visual grouping in the UI and enables better organization of effects.

## Threading Support

The EffectManager now includes threading support for better performance:

1. Each effect can run in its own thread
2. FPS is controlled per-effect rather than globally
3. Threaded operation is optional for compatibility

## Next Steps

The enhanced architecture provides the foundation for:

1. Multiple simultaneous effects
2. Effect layering and composition
3. More sophisticated preview functionality
4. Advanced spatial effects leveraging the 3D grid

These features will be built on top of the current architecture in the upcoming phases of development.