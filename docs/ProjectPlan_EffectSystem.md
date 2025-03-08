# Lightscape Spatial Effects System - Implementation Plan

## Project Overview

The goal is to enhance Lightscape's spatial framework by integrating concepts from OpenRGBEffectsPlugin to create a comprehensive, user-friendly effects system. This document outlines our approach to combining the strengths of both systems while maintaining Lightscape's unique 3D spatial capabilities.

## Current Structure Review

### Lightscape's Existing Components

1. **Spatial Grid System**
   - `SpatialGrid` for positioning devices in 3D space
   - Support for reference points and distance-based effects
   - Device assignment to grid positions

2. **Effect Framework**
   - `BaseEffect` abstract class for spatial effects
   - `EffectManager` to handle effect lifecycle
   - `EffectRegistry` for registering new effects
   - `TestEffect` as a proof-of-concept implementation

3. **Core Management**
   - `ResourceHandler` for OpenRGB resources
   - `DeviceManager` for device control
   - `StateManager` for plugin state

### OpenRGBEffectsPlugin Features to Adopt

1. **Modular Effect Structure**
   - Individual folders for each effect
   - Self-contained UI, implementation, and registration
   - Easy addition of new effects without recompiling

2. **Rich UI System**
   - Tabbed interface for multiple active effects
   - Device selection and assignment
   - Per-effect settings panels

3. **Effect Control System**
   - Threading model for effect updates
   - Controls for speed, brightness, etc.
   - Preview functionality

## Integration Strategy

We'll maintain Lightscape's spatial capabilities while incorporating OpenRGBEffectsPlugin's modular design and UI features.

### 1. File Structure Adjustments

Keep the existing Lightscape structure but reorganize the effects directory:

```
effects/
├── BaseEffect.h/cpp        (Existing)
├── EffectInfo.h            (Existing)
├── EffectList.h/cpp        (Existing)
├── EffectManager.h/cpp     (Existing)
├── EffectRegistry.h/cpp    (Existing)
├── EffectWidget.h/cpp      (Existing)
├── CategoryA/              (New folder structure by category)
│   ├── Effect1/
│   │   ├── Effect1.h
│   │   ├── Effect1.cpp
│   │   └── Effect1.ui
│   └── Effect2/
│       ├── Effect2.h
│       ├── Effect2.cpp
│       └── Effect2.ui
└── CategoryB/
    └── ...
```

### 2. Base Class Enhancements

Extend the existing `BaseEffect` class to include OpenRGBEffectsPlugin-style features:

```cpp
// Existing BaseEffect.h with enhancements
class BaseEffect : public QWidget
{
    Q_OBJECT

public:
    // Existing methods
    virtual RGBColor getColorForPosition(const GridPosition& pos, float time) = 0;
    
    // New methods from OpenRGBEffectsPlugin
    virtual void StepEffect(std::vector<ControllerZone*> zones);
    virtual void OnControllerZonesListChanged(std::vector<ControllerZone*> zones);
    
    // Common settings
    virtual void SetFPS(unsigned int value);
    virtual unsigned int GetFPS();
    
    virtual void SetSpeed(unsigned int value);
    virtual unsigned int GetSpeed();
    
    virtual void SetRandomColorsEnabled(bool value);
    virtual bool IsRandomColorsEnabled();
    
    // ... other common settings
    
protected:
    // Existing properties
    GridPosition _reference_point;
    
    // New properties
    unsigned int FPS = 60;
    unsigned int Speed = 50;
    std::vector<RGBColor> UserColors;
    bool RandomColorsEnabled = false;
};
```

### 3. Adapter for Controller Zones

Create an adapter class that connects OpenRGB's `ControllerZone` with Lightscape's `GridPosition`:

```cpp
class SpatialControllerZone
{
public:
    SpatialControllerZone(ControllerZone* zone, const GridPosition& position);
    
    ControllerZone* zone;
    GridPosition position;
    
    // Helper methods
    float getDistanceFrom(const GridPosition& reference) const;
    RGBColor getColor() const;
    void setColor(const RGBColor& color);
    
    // Serialization
    QJsonObject toJson() const;
    static SpatialControllerZone fromJson(const QJsonObject& json, DeviceManager* device_manager);
};
```

### 4. Effect Manager Enhancements

Extend the existing `EffectManager` to handle both spatial and non-spatial zones:

```cpp
// Enhanced EffectManager.h
class EffectManager : public QObject
{
    Q_OBJECT
    
public:
    static EffectManager& getInstance();
    
    // Existing methods
    void initialize(DeviceManager* device_manager, SpatialGrid* grid);
    bool startEffect(const QString& effect_id);
    void stopEffect();
    
    // New methods
    void setActiveZones(const QList<ControllerZone*>& zones);
    void setSpatialZones(const QList<SpatialControllerZone*>& zones);
    QList<ControllerZone*> getActiveZones() const;
    QList<SpatialControllerZone*> getSpatialZones() const;
    
    // Preview system
    void setPreviewEnabled(bool enabled);
    bool isPreviewEnabled() const;
    void addPreview(BaseEffect* effect, ControllerZone* preview);
    void removePreview(BaseEffect* effect);
    
private:
    // Enhanced update method
    void updateEffect();
    void effectThreadFunction(BaseEffect* effect);
    
    // Zone tracking
    QMap<BaseEffect*, QList<ControllerZone*>> _effect_zones;
    QMap<BaseEffect*, QList<SpatialControllerZone*>> _effect_spatial_zones;
    QMap<BaseEffect*, ControllerZone*> _previews;
};
```

### 5. UI Component Enhancements

Enhance the UI to support both the grid system and OpenRGBEffectsPlugin-style controls:

1. **TabWidget for Effects**
   - Add tabs for each active effect
   - Include controls in each tab
   - Support closing and rearranging tabs

2. **Device Assignment Panel**
   - List of available devices/zones
   - Checkboxes for assignment to the current effect
   - Position controls for spatial assignment

3. **Grid Visualization**
   - Interactive 3D grid for positioning
   - Visual representation of device locations
   - Reference point controls

### 6. Effect Registration System

Maintain the existing registry but enhance with category support:

```cpp
// Enhanced effect registration
#define REGISTER_EFFECT(info, class_name, category) \
    static bool registered_##class_name = \
        EffectRegistry::getInstance().registerEffect(info, []() { return new class_name(); }, category);
```

## Implementation Phases

### Phase 1: Core Architecture Integration

1. Extend `BaseEffect` with OpenRGBEffectsPlugin-style interfaces
2. Create the `SpatialControllerZone` adapter
3. Enhance `EffectManager` to support both spatial and non-spatial zones
4. Update the UI framework to support tabs and device assignment

### Phase 2: Effect System Enhancement

1. Implement the new file structure for effects by category
2. Create template effects that showcase the combined functionality
3. Enhance the grid visualization with better controls
4. Add preview functionality for effects

### Phase 3: Profile and Settings System

1. Implement a profile system for saving/loading effect configurations
2. Add global settings for startup behavior
3. Create an export/import system for sharing configurations
4. Add a setup and testing wizard for spatial configuration

## Setup and Test System

A key addition will be a Setup and Test system for verifying spatial configurations:

1. **Configuration Wizard**
   - Step-by-step guide for device placement
   - Grid dimensioning and scaling
   - Reference point placement

2. **Test Patterns**
   - Sequential lighting test (similar to surround sound test)
   - Axis highlighting (X, Y, Z planes)
   - Distance-based patterns from reference point
   - Spatial waves for verification

3. **Diagnostics**
   - Visual representation of device locations
   - Connection and communication status
   - Performance monitoring

## Example Effect Implementation

Here's an example of how to implement a new effect in the enhanced system:

```cpp
// SpatialRainbow.h in effects/Rainbow/SpatialRainbow/
class SpatialRainbow : public BaseEffect
{
    Q_OBJECT

public:
    explicit SpatialRainbow(QWidget* parent = nullptr);
    ~SpatialRainbow() = default;
    
    // Spatial effect interface
    RGBColor getColorForPosition(const GridPosition& pos, float time) override;
    
    // OpenRGBEffectPlugin-style interface
    void StepEffect(std::vector<ControllerZone*> zones) override;
    
    // Effect information
    static EffectInfo GetStaticInfo();
    
private slots:
    void onWaveSizeChanged(int value);
    void onDirectionChanged(int index);
    
private:
    Ui::SpatialRainbow* ui;
    
    float wave_size = 10.0f;
    int direction = 0; // 0: X, 1: Y, 2: Z, 3: Distance
};

// Register the effect
REGISTER_EFFECT(SpatialRainbow::GetStaticInfo(), SpatialRainbow, "Rainbow")
```

## Benefits of this Approach

1. **Maintains Spatial Capabilities**: Preserves Lightscape's 3D positioning system
2. **Improves Usability**: Adopts OpenRGBEffectsPlugin's intuitive UI
3. **Enhances Extensibility**: Makes adding new effects easier through modularity
4. **Provides Flexibility**: Works with both spatial and non-spatial devices
5. **Enables Community Contributions**: Standard format for effect contributions

## Next Steps

1. Refine the core architecture integration plan
2. Develop test cases for the combined functionality
3. Create a sample effect implementing both interfaces
4. Design the enhanced UI components
5. Implement the setup and test wizard