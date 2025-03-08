# Lightscape Effect System - Technical Specification

*Project Repository: [https://github.com/Wolfieeewolf/LightscapePlugin](https://github.com/Wolfieeewolf/LightscapePlugin)*

## Overview

This document provides technical details for implementing the enhanced effect system in Lightscape, integrating concepts from OpenRGBEffectsPlugin while preserving Lightscape's spatial capabilities.

## Core Components

### 1. Enhanced BaseEffect Class

```cpp
class BaseEffect : public QWidget
{
    Q_OBJECT

public:
    explicit BaseEffect(QWidget* parent = nullptr);
    virtual ~BaseEffect() = default;
    
    // Core spatial interface (existing)
    virtual void initialize(DeviceManager* device_manager, SpatialGrid* grid);
    virtual RGBColor getColorForPosition(const GridPosition& pos, float time) = 0;
    
    // OpenRGBEffectsPlugin-style interface (new)
    virtual void StepEffect(std::vector<ControllerZone*> zones);
    virtual void OnControllerZonesListChanged(std::vector<ControllerZone*> zones);
    
    // Settings & state management (existing with enhancements)
    virtual void loadSettings(const QJsonObject& json);
    virtual QJsonObject saveSettings() const;
    
    // Common effect properties
    void setEnabled(bool enabled);
    bool isEnabled() const;
    
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
    
    // Each effect must provide static info
    static EffectInfo GetStaticInfo();
    
signals:
    void effectUpdated();
    void settingsChanged();
    
protected:
    // Shared resources
    DeviceManager* _device_manager = nullptr;
    SpatialGrid* _spatial_grid = nullptr;
    
    // State
    bool _is_enabled = false;
    float _time = 0.0f;
    
    // Common properties
    int _speed = 50;
    int _brightness = 100;
    GridPosition _reference_point{0, 0, 0};
    QList<RGBColor> _user_colors;
    bool _random_colors_enabled = false;
    unsigned int _fps = 60;
    
    // Helper methods
    float calculateDistance(const GridPosition& pos1, const GridPosition& pos2) const;
    RGBColor applyBrightness(const RGBColor& color, float brightness_factor) const;
    
    // Default implementation for non-spatial devices
    void processSpatialZones(std::vector<ControllerZone*> zones, float time);
    void processNonSpatialZones(std::vector<ControllerZone*> zones, float time);
};
```

### 2. Spatial Controller Zone Adapter

```cpp
class SpatialControllerZone
{
public:
    SpatialControllerZone(ControllerZone* zone, const GridPosition& position);
    ~SpatialControllerZone() = default;
    
    // Core data
    ControllerZone* zone;
    GridPosition position;
    
    // Spatial operations
    float getDistanceFrom(const GridPosition& reference) const;
    float getAngleFrom(const GridPosition& reference, int axis) const;
    
    // Color operations
    void setLED(int led_idx, const RGBColor& color, int brightness = 100, int temperature = 0, int tint = 0);
    void setAllLEDs(const RGBColor& color, int brightness = 100, int temperature = 0, int tint = 0);
    
    // Zone information
    zone_type getZoneType() const;
    unsigned int getLEDCount() const;
    
    // Matrix operations (if applicable)
    bool isMatrix() const;
    unsigned int getMatrixWidth() const;
    unsigned int getMatrixHeight() const;
    
    // Serialization
    QJsonObject toJson() const;
    static SpatialControllerZone* fromJson(const QJsonObject& json, DeviceManager* device_manager);
    
    // Factory method to convert from existing zones
    static SpatialControllerZone* fromControllerZone(ControllerZone* zone, const GridPosition& position);
};
```

### 3. Enhanced Effect Manager

```cpp
class EffectManager : public QObject
{
    Q_OBJECT
    
public:
    static EffectManager& getInstance();
    
    // Initialization
    void initialize(DeviceManager* device_manager, SpatialGrid* grid);
    
    // Effect control
    bool startEffect(const QString& effect_id);
    void stopEffect();
    bool isEffectRunning() const;
    QString getCurrentEffectId() const;
    BaseEffect* getCurrentEffect() const;
    
    // Zone management
    void setActiveZones(const QList<ControllerZone*>& zones);
    QList<ControllerZone*> getActiveZones() const;
    
    void setSpatialZones(const QList<SpatialControllerZone*>& zones);
    QList<SpatialControllerZone*> getSpatialZones() const;
    
    // Preview system
    void setPreviewEnabled(bool enabled);
    bool isPreviewEnabled() const;
    void addPreview(BaseEffect* effect, ControllerZone* preview);
    void removePreview(BaseEffect* effect);
    
    // Performance settings
    void setReducedFps(bool reduced);
    
signals:
    void effectStarted(const QString& effect_id);
    void effectStopped();
    void previewUpdated();
    
private slots:
    void updateEffect();
    
private:
    EffectManager();
    ~EffectManager();
    
    // Private methods
    void effectThreadFunction(BaseEffect* effect);
    void updateDevicePositions();
    void notifySelectionChanged(BaseEffect* effect);
    
    // Member variables
    QTimer* _update_timer;
    DeviceManager* _device_manager = nullptr;
    SpatialGrid* _spatial_grid = nullptr;
    BaseEffect* _active_effect = nullptr;
    QString _current_effect_id;
    
    // Zones tracking
    QList<ControllerZone*> _active_zones;
    QMap<BaseEffect*, QList<ControllerZone*>> _effect_zones;
    QMap<BaseEffect*, QList<SpatialControllerZone*>> _effect_spatial_zones;
    QMap<BaseEffect*, ControllerZone*> _previews;
    
    // State
    bool _is_running = false;
    bool _preview_enabled = true;
    int _update_interval = 33; // ~30 FPS
    QElapsedTimer _frame_timer;
    
    // Thread management
    std::map<BaseEffect*, std::thread*> _effect_threads;
    std::mutex _lock;
};
```

### 4. Enhanced Effect Registry

```cpp
class EffectRegistry
{
public:
    static EffectRegistry& getInstance();
    
    // Registration
    void registerEffect(const EffectInfo& info, std::function<BaseEffect*()> creator, const QString& category = "");
    void unregisterEffect(const QString& effectId);
    
    // Effect creation
    BaseEffect* createEffect(const QString& effectId);
    bool hasEffect(const QString& effectId) const;
    
    // Categorization
    QList<QString> getCategories() const;
    QList<EffectInfo> getEffectsInCategory(const QString& category) const;
    QMap<QString, QList<EffectInfo>> getCategorizedEffects() const;
    
private:
    EffectRegistry();
    ~EffectRegistry();
    
    struct RegistryEntry {
        EffectInfo info;
        std::function<BaseEffect*()> creator;
        QString category;
    };
    
    QMap<QString, RegistryEntry> registry;
    QMap<QString, QList<EffectInfo>> categorized_effects;
};
```

### 5. Registration Macro

```cpp
#define REGISTER_EFFECT(_info, _class, _category) \
    namespace { \
        static bool _effect_registered_##_class = \
            Lightscape::EffectRegistry::getInstance().registerEffect( \
                _info, \
                []() { return new _class(); }, \
                _category \
            ); \
    }
```

## User Interface Components

### 1. Effect Tab Widget

```cpp
class EffectTabWidget : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit EffectTabWidget(QWidget* parent = nullptr);
    ~EffectTabWidget();
    
    // Tab management
    void addEffectTab(BaseEffect* effect);
    void closeEffectTab(int index);
    BaseEffect* getEffectAtTab(int index) const;
    
    // Current effect
    BaseEffect* getCurrentEffect() const;
    
signals:
    void effectChanged(BaseEffect* effect);
    void effectClosed(BaseEffect* effect);
    
private slots:
    void onTabCloseRequested(int index);
    void onCurrentChanged(int index);
    
private:
    QList<BaseEffect*> _effects;
};
```

### 2. Effect Tab Header

```cpp
class EffectTabHeader : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectTabHeader(QWidget* parent = nullptr, BaseEffect* effect = nullptr);
    ~EffectTabHeader();
    
    void toggleRunningIndicator(bool running);
    
signals:
    void closeRequested();
    void startStopRequested();
    void renamed(const QString& new_name);
    
private slots:
    void onCloseClicked();
    void onStartStopClicked();
    void onRenameClicked();
    
private:
    Ui::EffectTabHeader* ui;
    BaseEffect* _effect;
};
```

### 3. Device List Widget

```cpp
class DeviceListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceListWidget(QWidget* parent = nullptr);
    ~DeviceListWidget();
    
    // Device initialization
    void init(DeviceManager* device_manager);
    void refresh();
    
    // Selection
    QList<ControllerZone*> getSelectedZones() const;
    void setSelectedZones(const QList<ControllerZone*>& zones);
    
    // Spatial positioning
    void showPositionControls(bool show);
    bool arePositionControlsVisible() const;
    
signals:
    void selectionChanged(const QList<ControllerZone*>& zones);
    void devicePositionChanged(ControllerZone* zone, const GridPosition& position);
    
private slots:
    void onItemSelectionChanged();
    void onPositionChanged(const GridPosition& position);
    
private:
    Ui::DeviceListWidget* ui;
    DeviceManager* _device_manager = nullptr;
    QMap<QListWidgetItem*, ControllerZone*> _item_to_zone;
};
```

### 4. Grid Visualization Widget

```cpp
class GridVisualizationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridVisualizationWidget(QWidget* parent = nullptr);
    ~GridVisualizationWidget();
    
    // Grid setup
    void setGrid(SpatialGrid* grid);
    SpatialGrid* getGrid() const;
    
    // Reference point
    void setReferencePoint(const GridPosition& point);
    GridPosition getReferencePoint() const;
    
    // View control
    void setViewAngle(float yaw, float pitch);
    void resetView();
    void zoomIn();
    void zoomOut();
    
signals:
    void referencePointChanged(const GridPosition& point);
    void deviceSelected(ControllerZone* zone);
    void deviceMoved(ControllerZone* zone, const GridPosition& position);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    
private:
    SpatialGrid* _grid = nullptr;
    GridPosition _reference_point{0, 0, 0};
    
    // View parameters
    float _yaw = 45.0f;
    float _pitch = 30.0f;
    float _zoom = 1.0f;
    QPoint _last_mouse_pos;
    
    // Rendering helpers
    void drawGrid(QPainter& painter);
    void drawDevices(QPainter& painter);
    void drawReferencePoint(QPainter& painter);
    
    // Conversion between screen and grid coordinates
    QPoint gridToScreen(const GridPosition& pos) const;
    GridPosition screenToGrid(const QPoint& pos) const;
};
```

## Effect Implementation Pattern

Each effect should follow this pattern:

```cpp
// SpatialRainbow.h
class SpatialRainbow : public BaseEffect
{
    Q_OBJECT

public:
    explicit SpatialRainbow(QWidget* parent = nullptr);
    ~SpatialRainbow() = default;
    
    // Required spatial implementation
    RGBColor getColorForPosition(const GridPosition& pos, float time) override;
    
    // Standard effect implementation
    void StepEffect(std::vector<ControllerZone*> zones) override;
    
    // Settings
    void loadSettings(const QJsonObject& json) override;
    QJsonObject saveSettings() const override;
    
    // Static info
    static EffectInfo GetStaticInfo();
    
private slots:
    void onWaveSizeChanged(int value);
    void onDirectionChanged(int index);
    
private:
    Ui::SpatialRainbow* ui;
    
    // Effect parameters
    float _wave_size = 10.0f;
    int _direction = 0; // 0: X, 1: Y, 2: Z, 3: Distance
};

// SpatialRainbow.cpp
SpatialRainbow::SpatialRainbow(QWidget* parent) 
    : BaseEffect(parent)
    , ui(new Ui::SpatialRainbow)
{
    ui->setupUi(this);
    
    // Connect UI signals
    connect(ui->waveSizeSlider, &QSlider::valueChanged,
            this, &SpatialRainbow::onWaveSizeChanged);
    
    connect(ui->directionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SpatialRainbow::onDirectionChanged);
    
    // Set defaults
    ui->waveSizeSlider->setValue(_wave_size);
    ui->directionComboBox->setCurrentIndex(_direction);
}

EffectInfo SpatialRainbow::GetStaticInfo()
{
    EffectInfo info;
    info.name = QString("Spatial Rainbow");
    info.id = QString("spatial_rainbow");
    info.description = QString("Rainbow effect based on spatial position");
    info.category = EffectCategory::Spatial;
    info.requiresReferencePoint = true;
    info.supportsPreview = true;
    return info;
}

RGBColor SpatialRainbow::getColorForPosition(const GridPosition& pos, float time)
{
    // Use the selected direction for the rainbow wave
    float position_value;
    switch(_direction)
    {
        case 0: position_value = pos.x; break; // X direction
        case 1: position_value = pos.y; break; // Y direction
        case 2: position_value = pos.z; break; // Z direction
        case 3: // Distance from reference point
            position_value = calculateDistance(pos, _reference_point);
            break;
    }
    
    // Calculate hue based on position and time
    float hue = fmodf((position_value / _wave_size) + time, 1.0f);
    
    // Convert HSV to RGB
    return ColorUtils::HSVtoRGB(hue * 360.0f, 1.0f, 1.0f);
}

void SpatialRainbow::StepEffect(std::vector<ControllerZone*> zones)
{
    // Let the base class handle spatial vs. non-spatial zones
    BaseEffect::StepEffect(zones);
}

// Registration
REGISTER_EFFECT(SpatialRainbow::GetStaticInfo(), SpatialRainbow, "Rainbow")
```

## Setup and Testing System

### 1. Setup Wizard

The setup wizard should guide users through:

1. **Grid Configuration**
   - Setting dimensions (width, height, depth)
   - Setting unit scale (real-world size to grid units)
   - Orientation (e.g., which way is front/back)

2. **Device Placement**
   - Position each device in the grid
   - Set device orientation
   - Group devices by location (e.g., desk, wall, ceiling)

3. **Reference Points**
   - Set default reference points 
   - Create named reference locations

4. **Calibration**
   - Visual verification of device positions
   - Display test patterns

### 2. Test System

The test system should include:

1. **Sequential Test**
   - Light each device in sequence
   - Show device name and position during test

2. **Axis Test**
   - Light all devices on X plane, then Y, then Z
   - Show colored visualization of current plane

3. **Distance Test**
   - Light devices based on distance from reference point
   - Create expanding ring/wave effect

4. **Performance Test**
   - Run increasingly complex effects to test system performance
   - Report frame rates and responsiveness

## Implementation Plan

### Phase 1: Core Architecture (Weeks 1-2)

1. Extend BaseEffect with OpenRGBEffectsPlugin-style interfaces
2. Create SpatialControllerZone adapter
3. Enhance EffectManager with spatial/non-spatial handling
4. Update effect registration system with categories

### Phase 2: UI Enhancement (Weeks 3-4)

1. Create tabbed interface for effects
2. Enhance device list with spatial positioning
3. Improve grid visualization
4. Implement effect preview system

### Phase 3: Effect Implementation (Weeks 5-6)

1. Create template effects that use both interfaces
2. Port/adapt existing effects to new system
3. Implement new spatial effects
4. Add categories and organization

### Phase 4: Setup & Testing (Weeks 7-8)

1. Design and implement setup wizard
2. Create test patterns and verification system
3. Add performance monitoring 
4. Build diagnostics tools

## Testing Strategy

1. **Unit Testing**
   - Test each component in isolation
   - Verify spatial calculations
   - Test color generation and application

2. **Integration Testing**
   - Test effect system with mock devices
   - Verify threading and performance
   - Test UI responsiveness

3. **User Acceptance Testing**
   - Test with real hardware configurations
   - Verify setup wizard usability
   - Ensure effect quality and performance