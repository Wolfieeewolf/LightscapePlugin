# Lightscape Phase 3: UI Framework Implementation

*Project Repository: [https://github.com/Wolfieeewolf/LightscapePlugin](https://github.com/Wolfieeewolf/LightscapePlugin)*

This document outlines the technical implementation details for Phase 3 of the Lightscape development roadmap: UI Framework Updates. The implementation adds support for multiple simultaneous effects, enhanced previewing, and improved device management.

## Key Components Implemented

### 1. EffectTabWidget

A specialized tab widget for managing multiple effect instances with custom tab headers.

**Files:**
- `include/effects/EffectTabWidget.h`
- `src/effects/EffectTabWidget.cpp`
- `include/effects/EffectTabHeader.h`
- `src/effects/EffectTabHeader.cpp`
- `ui/effects/headers/EffectTabHeader.ui`

**Features:**
- Custom tab headers with play/stop buttons
- Effect renaming support
- Tab close handling with proper cleanup

**Usage Example:**
```cpp
// Create and add to layout
EffectTabWidget* tabWidget = new EffectTabWidget(parent);
layout->addWidget(tabWidget);

// Add an effect tab
BaseEffect* effect = registry.createEffect(effectId);
tabWidget->addEffectTab(effect);

// Connect signals
connect(tabWidget, &EffectTabWidget::effectChanged, 
        this, &MyClass::handleEffectChanged);
```

### 2. PreviewRenderer

A unified visualization widget that supports both 2D and 3D views of effects.

**Files:**
- `include/effects/PreviewRenderer.h`
- `src/effects/PreviewRenderer.cpp`

**Features:**
- Togglable 2D/3D visualization modes
- Real-time effect previewing with color changes
- Interactive camera with rotation and zoom
- Layer navigation
- Flat, clear representation of devices and effects

**Implementation Notes:**
- Uses QPainter for rendering
- Implements isometric projection for 3D visualization
- Supports different interaction modes for 2D and 3D views
- Optimized for clarity and usability

**Key Features:**
- Simple, clear 3D visualization with flat, easy-to-distinguish cells
- 2D layer view for precise examination of individual layers
- Easy navigation between layers
- Consistent interaction patterns across modes

### 3. DeviceListWidget

An enhanced device selection widget with spatial positioning controls.

**Files:**
- `include/effects/DeviceListWidget.h`
- `src/effects/DeviceListWidget.cpp`
- `ui/effects/DeviceListWidget.ui`

**Features:**
- Device list with checkboxes
- Position editing controls
- Select all/clear all functionality

**Implementation Details:**
- Uses QListWidget for device display
- Integrates with SpatialGrid for device positioning
- Uses DeviceManager for device information

### 4. EnhancedEffectWidget

The main container widget that brings together all UI components in a streamlined layout.

**Files:**
- `include/effects/EnhancedEffectWidget.h`
- `src/effects/EnhancedEffectWidget.cpp`
- `ui/effects/EnhancedEffectWidget.ui`

**Features:**
- Space-efficient layout with logical grouping of controls
- Unified preview area with mode toggle
- Profile saving and loading
- Effect management

**Layout Design:**
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

**Integration Points:**
- Integrates with EffectManager for effect control
- Connects with SpatialGrid for device positioning
- Manages effect-specific device assignments

### 5. EffectSelectorDialog

A dialog for selecting effects to add to the UI.

**Files:**
- `include/effects/EffectSelectorDialog.h`
- `src/effects/EffectSelectorDialog.cpp`
- `ui/effects/EffectSelectorDialog.ui`

**Features:**
- Categories effects by type
- Shows effect descriptions
- Supports effect filtering

### 6. Enhanced EffectManager

Improvements to the EffectManager to support multiple effect instances.

**Files:**
- `include/effects/EffectManager.h`
- `src/effects/EffectManager.cpp`

**Enhancements:**
- Support for multiple simultaneously active effects
- Per-effect device assignments
- Preview system integration
- Enhanced profile handling

## Implementation Details

### Preview Renderer Implementation

The unified PreviewRenderer implements both 2D and 3D viewing modes:

```cpp
class PreviewRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewRenderer(QWidget *parent = nullptr);
    
    // Core methods
    void setEffect(BaseEffect* effect);
    void setGrid(SpatialGrid* grid);
    
    // View mode controls
    void setViewMode(bool use2DMode);
    void setActiveLayer(int layer);
    
    // View controls
    void resetView();
    void setShowGrid(bool show);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    // Drawing methods
    void draw2DView(QPainter& painter);
    void draw3DView(QPainter& painter);
    
    // View state
    bool _is2DMode = false;
    int _activeLayer = 0;
    float _rotationX = 0.5f;
    float _rotationY = 0.5f;
    float _zoom = 1.0f;
};
```

The implementation provides:
1. **Mode Switching**: Toggle between 2D and 3D views
2. **Layer Navigation**: Select which layer to view in 2D mode
3. **Rotation/Zoom**: Interactive camera controls in 3D mode
4. **Unified Approach**: Consistent interaction pattern regardless of mode

### File Structure

The implementation follows the established Lightscape file structure:

```
include/effects/
  ├── EffectTabWidget.h
  ├── EffectTabHeader.h
  ├── PreviewRenderer.h
  ├── DeviceListWidget.h
  ├── EnhancedEffectWidget.h
  └── EffectSelectorDialog.h

src/effects/
  ├── EffectTabWidget.cpp
  ├── EffectTabHeader.cpp
  ├── PreviewRenderer.cpp
  ├── DeviceListWidget.cpp
  ├── EnhancedEffectWidget.cpp
  └── EffectSelectorDialog.cpp

ui/effects/
  ├── EnhancedEffectWidget.ui
  ├── DeviceListWidget.ui
  ├── EffectSelectorDialog.ui
  └── headers/
      └── EffectTabHeader.ui
```

### Integration with Existing Codebase

The new UI framework is integrated with the existing codebase through:

1. **EnhancedLightscapeWidget** - A new main widget that includes the old setup tab and the new effects tab
2. **Updated EffectManager** - Enhanced to support multiple effects while maintaining backward compatibility
3. **Core Plugin Changes** - Updated to use the new EnhancedLightscapeWidget

### Key Design Patterns

1. **Composition** - Each UI component is self-contained and composed into larger structures
2. **Observer Pattern** - Signals and slots for communication between components
3. **Factory Pattern** - Effects are created by the EffectRegistry
4. **Singleton** - EffectManager as a singleton for global access
5. **State Pattern** - PreviewRenderer uses different state-specific behaviors for 2D and 3D modes

### Device Positioning Implementation

Device positioning is handled through the SpatialGrid's assignment system:

1. **Finding Current Position**
   - Scan all grid positions to find current device assignments

2. **Updating Position**
   - Remove device from old position
   - Add device to new position using AddAssignment
   - Update UI to reflect changes

```cpp
// Implementation pattern for changing a device's position
void onDevicePositionChanged(const DeviceInfo& device, const GridPosition& newPosition)
{
    // 1. Clear existing assignment
    for (GridPosition pos : allGridPositions) {
        for (int i = 0; i < assignments.size(); i++) {
            if (matchesDevice(assignments[i], device)) {
                spatialGrid->RemoveAssignment(pos, i);
            }
        }
    }
    
    // 2. Add new assignment
    DeviceAssignment assignment(device.index, device.type);
    spatialGrid->AddAssignment(newPosition, assignment);
    
    // 3. Update UI
    updatePreview();
}
```

### Profile Management

Profiles are implemented using JSON to store:

1. Effect IDs and settings
2. Device assignments and positions
3. UI configuration

The implementation uses QJsonDocument, QJsonObject, and QJsonArray for serialization/deserialization.

## Build System Integration

The new files have been added to the qmake project file (`Lightscape.pro`).

Key section additions:

```qmake
HEADERS += \
    include/effects/EffectTabWidget.h \
    include/effects/EffectTabHeader.h \
    include/effects/PreviewRenderer.h \
    include/effects/DeviceListWidget.h \
    include/effects/EnhancedEffectWidget.h \
    include/effects/EffectSelectorDialog.h \
    include/core/HashFunctions.h \

SOURCES += \
    src/effects/EffectTabWidget.cpp \
    src/effects/EffectTabHeader.cpp \
    src/effects/PreviewRenderer.cpp \
    src/effects/DeviceListWidget.cpp \
    src/effects/EnhancedEffectWidget.cpp \
    src/effects/EffectSelectorDialog.cpp \

FORMS += \
    ui/effects/EnhancedEffectWidget.ui \
    ui/effects/DeviceListWidget.ui \
    ui/effects/EffectSelectorDialog.ui \
    ui/effects/headers/EffectTabHeader.ui \
```

## Backward Compatibility

The implementation maintains backward compatibility through:

1. **Dual Interface** - Original widget continues to function
2. **Enhanced EffectManager** - Supports both single and multiple effect modes
3. **Tabbed Approach** - Setup tab preserves original functionality

## Known Issues and Workarounds

1. **QHash Requirements**
   - A custom hash function was needed for QPair<int, DeviceType>
   - Implemented in HashFunctions.h

2. **Device Positioning Method**
   - SpatialGrid doesn't have a direct SetDevicePosition method
   - Workaround: Use AddAssignment after removing from old position

3. **Casting Requirements**
   - EffectRegistry::createEffect returns void*
   - Solution: Explicit static_cast<BaseEffect*> required

4. **PreviewRenderer Performance**
   - Large grids may experience performance issues in 3D mode
   - Solution: Simplified flat representation of cells improves performance

## Technical Debt and Future Improvements

1. **Effect Tab Management**
   - Could benefit from drag-and-drop reordering
   - Tab state persistence needs improvement

2. **Preview Performance**
   - Currently redraws entire grid on updates
   - Could optimize with partial rendering or OpenGL

3. **Device Position Management**
   - Current approach scans entire grid to find devices
   - A more direct lookup method would be more efficient

4. **EffectManager Threading**
   - Thread support is implemented but disabled
   - Future versions should enable proper threading

5. **Preview Enhancements**
   - Additional visualization modes beyond 2D/3D
   - Better highlighting of user position
   - Smoother transitions between layers

## Latest UI Improvements

The UI has been further enhanced with the following improvements:

1. **Improved Control Placement** - Control buttons have been moved to their respective panels:
   - Add/Save/Load buttons are now in the Effects panel
   - View control buttons (2D/3D, Change Layer, Reset View) are now in the Effects Preview panel
   - Select All/Clear All buttons are now in the Effect Devices panel

2. **Removed Bottom Toolbar** - The bottom toolbar has been completely removed, eliminating an unnecessary UI layer

3. **Consistent Header Styling** - All panel headers now have consistent styling and size

4. **Code Cleanup** - Removed the ToolbarPanel component from the codebase

These changes make the UI more intuitive by placing controls directly next to the components they affect.

## Next Steps

With the UI Framework updated and refined, the next steps are:

1. Implement the first spatial effect (SpatialRainbow)
2. Add profile templates and example configurations
3. Add effect categories and organization
4. Enhance the preview system with additional visualization options
5. Add user position visualization in preview