# Lightscape Enhanced Effect UI Framework

*Project Repository: [https://github.com/Wolfieeewolf/LightscapePlugin](https://github.com/Wolfieeewolf/LightscapePlugin)*

This document details the enhanced user interface framework implemented in Phase 3 of the Lightscape development roadmap. The new UI enables multiple simultaneous effects, improved previewing, and better device management.

## Overview

The Enhanced Effect UI introduces several key improvements:

1. **Tabbed Interface** - Run and configure multiple effects simultaneously
2. **Enhanced Preview** - Interactive visualization with togglable 2D/3D views
3. **Simplified Device Selection** - Easy device selection for controlling which devices respond to an effect
4. **Profile Management** - Save and load effect configurations

## Architecture

The UI framework is built around several primary components with a streamlined layout:

![UI Architecture](../images/EnhancedEffectUI_Architecture.png)

### Core Components

1. **EnhancedEffectWidget** - The main container widget that houses all effect UI components
2. **EffectTabWidget** - Manages multiple effect tabs with custom headers
3. **PreviewRenderer** - Unified visualization widget with togglable 2D/3D modes
4. **DeviceListWidget** - Simple list interface for selecting devices to control with effects

### Component Relationships

- **EnhancedEffectWidget** coordinates all components and contains the profile management system
- **EffectTabWidget** manages effect lifecycles and contains the effect control UI
- **PreviewRenderer** visualizes the currently selected effect in either 2D or 3D mode
- **DeviceListWidget** controls which devices are assigned to each effect

## UI Layout Design

The Enhanced UI is organized in a clean, intuitive layout with controls placed directly within their relevant panels:

```
┌──────────────┬──────────────┬──────────────┐
│              │              │              │
│   Effects    │ Effect       │   Effect     │
│    Tabs      │ Controls     │   Devices    │
│              │              │              │
│              │              │              │
│ Add/Save/Load│              │ Select/Clear │
├──────────────┴──────────────┴──────────────┤
│                                            │
│                                            │
│              Effects Preview               │
│                                            │
│                                            │
│                                            │
│          2D/3D - Change Layer - Reset     │
└────────────────────────────────────────────┘
```

This design provides:
- Clear separation of concerns with distinct areas for effects, controls, devices, and preview
- Controls placed directly within their related panels for more intuitive operation
- Larger unified preview area that switches between 2D and 3D views
- Consistent header styling across all panels
- Maximized screen real estate for the most important elements

## Main Components

### EnhancedEffectWidget

The primary container widget that orchestrates all UI components.

**Key Responsibilities:**
- Manages the overall UI layout
- Handles effect creation and initialization
- Coordinates device selection with the effect manager
- Provides profile saving and loading

**Usage:**
```cpp
// Initialize with required components
EnhancedEffectWidget* widget = new EnhancedEffectWidget(deviceManager, spatialGrid);

// Add to your layout
mainLayout->addWidget(widget);
```

### EffectTabWidget

A specialized tab widget that manages multiple effect instances.

**Key Features:**
- Custom tab headers with play/stop controls
- Individual effect settings preserved per tab
- Effect organization and management

**Architecture:**
- `EffectTabWidget` - The main tab container
- `EffectTabHeader` - Custom tab headers with controls

**Signals:**
- `effectChanged(BaseEffect*)` - Emitted when the current tab changes
- `effectClosed(BaseEffect*)` - Emitted when an effect tab is closed
- `effectStartStopChanged(BaseEffect*, bool)` - Emitted when an effect is started or stopped
- `effectRenamed(BaseEffect*, QString)` - Emitted when an effect is renamed

### PreviewRenderer

A unified visualization widget that supports both 2D and 3D views of effects.

**Key Features:**
- Togglable 2D/3D visualization modes
- Real-time effect previewing with color changes
- Interactive camera with rotation and zoom in 3D mode
- Layer navigation for examining specific layers in 2D mode
- Flat, clear representation of devices and effects

**Interactions:**
- 2D mode: Shows top-down view of a single layer
- 3D mode: Shows all layers with interactive rotation
- Left-click and drag to rotate the view (3D) or pan (2D)
- Mouse wheel to zoom in both modes
- Arrow keys to navigate between layers

**Controls:**
- `setEffect(BaseEffect*)` - Set the effect to preview
- `resetView()` - Reset the camera view
- `setShowGrid(bool)` - Toggle grid visibility
- `setActiveLayer(int)` - Set which layer to view in 2D mode
- `setViewMode(bool)` - Toggle between 2D and 3D views

### DeviceListWidget

A simplified widget for device selection.

**Key Features:**
- Device list with checkboxes for selection
- Select all/clear all functionality

**Key Functions:**
- `refresh()` - Update the device list
- `setSelectedDevices(QList<DeviceInfo>)` - Set which devices are selected
- `getSelectedDevices()` - Get currently selected devices

## Using the Enhanced UI

### Adding Effects

1. Click the "Add Effect" button
2. Select an effect from the dialog
3. Configure effect parameters in the effect's tab

### Managing Multiple Effects

- Each effect gets its own tab with independent settings
- Use the play/stop button on each tab to control individual effects
- Close tabs to remove effects

### Device Selection

- Select devices from the list to associate them with the current effect
- The selected devices will respond to the current effect when it's running
- Each effect can have its own set of selected devices

### Using the Preview

1. Use the "2D/3D" toggle to switch between view modes
2. In 2D mode:
   - View a single layer at a time
   - Use arrow keys to navigate between layers
   - Drag to pan the view
3. In 3D mode:
   - View all layers in a 3D perspective
   - Drag to rotate the view
   - Easily see relationships between layers

### Saving and Loading Profiles

1. Configure your effects and device selections
2. Click "Save Profile" and choose a location
3. Load previously saved profiles with "Load Profile"

## Profile Format

Profiles are saved in JSON format with the following structure:

```json
{
  "version": 1.0,
  "effects": [
    {
      "id": "effect_id",
      "settings": {
        // Effect-specific settings
      },
      "devices": [
        {
          "index": 0,
          "type": 0,
          "position": {
            "x": 1,
            "y": 2,
            "z": 0
          }
        }
      ]
    }
  ]
}
```

## Integration with EffectManager

The Enhanced UI integrates with an improved EffectManager that now supports:

1. Multiple simultaneous effects
2. Per-effect device assignments
3. Preview system integration

### Key Enhancements

- `startEffect(effectId, existingEffect)` - Start a specific effect instance
- `stopEffect(effect)` - Stop a specific effect instance
- `setActiveDevicesForEffect(effect, devices)` - Set devices for a specific effect
- `isEffectRunning(effect)` - Check if a specific effect is running

## Future Extensions

The Enhanced UI framework is designed to be extensible for future improvements:

1. **Effect Categories** - Group effects by category in the selection dialog
2. **Effect Layering** - Combine effects with layer controls
3. **Effect Sequences** - Time-based sequences of effects
4. **Advanced Preview Options** - Additional visualization modes and controls

## Troubleshooting

### Effect Not Appearing
- Ensure the effect is properly registered in EffectRegistry
- Check that the effect widget is properly created and initialized

### UI Layout Issues
- Check widget sizes and size policies
- Verify that all components are properly added to layouts

### Preview Rendering Issues
- Reset the view with the reset button
- Switch between 2D and 3D modes to refresh the display
- Verify the effect is properly generating colors for positions

## Migration from Previous UI

The EnhancedLightscapeWidget now includes both the old Setup tab and the new Effects tab, making it easy to transition between the old and new interfaces. The original EffectWidget functionality is now enhanced in the EnhancedEffectWidget.