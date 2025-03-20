/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewRenderer.h                                         |
|                                                           |
| Class for rendering effect previews                       |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include <QSet>
#include <QPainter>
#include "grid/SpatialGrid.h"
#include "effects/BaseEffect.h"
#include "effects/PreviewRenderer2D.h"
#include "effects/PreviewRenderer3D.h"
#include "effects/PreviewInteraction.h"
#include "effects/PreviewSettings.h"

namespace Lightscape {

class PreviewRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewRenderer(QWidget *parent = nullptr);
    ~PreviewRenderer();
    
    // Setup
    void setGrid(SpatialGrid* grid);
    SpatialGrid* getGrid() const;
    
    // Effect preview
    void setEffect(BaseEffect* effect);
    BaseEffect* getEffect() const;
    
    // View controls
    void resetView();
    void setShowGrid(bool show);
    
    // View mode
    void toggleViewMode();
    bool isIn3DMode() const;
    
    // Device-only mode (only shows colors at device positions)
    void toggleDeviceOnlyMode();
    bool isInDeviceOnlyMode() const;
    void setDevicePositions(const QSet<GridPosition>& devicePositions);
    
    // Save current view as default
    void saveCurrentViewAsDefault();
    
    // Layer control
    void setActiveLayer(int layer);
    int getActiveLayer() const;
    bool showLayerDialog();
    
    // Animation
    void setAnimated(bool animated);
    
public slots:
    void updatePreview();
    
signals:
    void activeLayerChanged(int layer);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    
private slots:
    void onRotationChanged(float deltaX, float deltaY);
    void onRollChanged(float delta);
    void onPanChanged(const QPoint& delta);
    void onZoomChanged(float factor);
    void onViewReset();
    void onActiveLayerChanged(int delta);
    void onViewSaved();
    
private:
    // Rendering components
    PreviewRenderer2D _renderer2D;
    PreviewRenderer3D _renderer3D;
    PreviewInteraction* _interaction;
    PreviewSettings* _settings;
    
    // Grid and effect
    SpatialGrid* _grid = nullptr;
    BaseEffect* _effect = nullptr;
    
    // View rotations
    float _rotationX = -1.16f;  // X rotation (pitch)
    float _rotationY = -0.61f;  // Y rotation (yaw)
    float _rotationZ = 0.0f;    // Z rotation (roll)
    
    // Display settings
    int _activeLayer = 0;
    bool _showGrid = true;
    bool _animated = true;
    bool _is3DMode = true;
    bool _deviceOnlyMode = false;
    float _zoom = 1.0f;
    QPoint _panOffset = QPoint(0, 0);
    
    // Device positions
    QSet<GridPosition> _devicePositions;
    
    // Animation timer
    QTimer* _updateTimer = nullptr;
};

} // namespace Lightscape