/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewRenderer.cpp                                       |
|                                                           |
| Class for rendering effect previews                       |
\*---------------------------------------------------------*/

#include "effects/PreviewRenderer.h"
#include "effects/PreviewRenderer2D.h"
#include "effects/PreviewRenderer3D.h"
#include "effects/PreviewInteraction.h"
#include "effects/PreviewSettings.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <cmath>

namespace Lightscape {

PreviewRenderer::PreviewRenderer(QWidget *parent)
    : QWidget(parent)
    , _updateTimer(new QTimer(this))
    , _interaction(new PreviewInteraction(this))
    , _settings(new PreviewSettings(this))
{
    // Set minimum size
    setMinimumSize(400, 300);
    
    // Set background
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(40, 40, 40));
    setPalette(pal);
    
    // Configure timer for animation - start immediately
    _updateTimer->setInterval(33); // ~30 FPS
    connect(_updateTimer, &QTimer::timeout, this, &PreviewRenderer::updatePreview);
    _updateTimer->start(); // Always start animation regardless of _animated
    
    // Enable keyboard focus for key events
    setFocusPolicy(Qt::StrongFocus);
    
    // Set tooltip with usage instructions
    setToolTip("Effect Preview\n"
              "• Drag to rotate in 3D mode\n"
              "• Drag to pan in 2D mode\n"
              "• Scroll to zoom\n"
              "• Arrow keys to change layer");
    
    // Load saved view settings
    _settings->loadSettings(_rotationX, _rotationY, _rotationZ, _zoom);
    
    // Connect interaction signals
    connect(_interaction, &PreviewInteraction::rotationChanged, this, &PreviewRenderer::onRotationChanged);
    connect(_interaction, &PreviewInteraction::rollChanged, this, &PreviewRenderer::onRollChanged);
    connect(_interaction, &PreviewInteraction::panChanged, this, &PreviewRenderer::onPanChanged);
    connect(_interaction, &PreviewInteraction::zoomChanged, this, &PreviewRenderer::onZoomChanged);
    connect(_interaction, &PreviewInteraction::viewReset, this, &PreviewRenderer::onViewReset);
    connect(_interaction, &PreviewInteraction::activeLayerChanged, this, &PreviewRenderer::onActiveLayerChanged);
    connect(_interaction, &PreviewInteraction::viewSaved, this, &PreviewRenderer::onViewSaved);
    connect(_interaction, &PreviewInteraction::updateRequested, this, [this](){ update(); });
}

PreviewRenderer::~PreviewRenderer()
{
    if (_updateTimer) {
        _updateTimer->stop();
        delete _updateTimer;
        _updateTimer = nullptr;
    }
    
    if (_interaction) {
        delete _interaction;
        _interaction = nullptr;
    }
    
    if (_settings) {
        delete _settings;
        _settings = nullptr;
    }
}

void PreviewRenderer::setGrid(SpatialGrid* grid)
{
    _grid = grid;
    printf("[Lightscape] Preview: Grid set to %p\n", (void*)grid);
    update();
}

SpatialGrid* PreviewRenderer::getGrid() const
{
    return _grid;
}

void PreviewRenderer::setEffect(BaseEffect* effect)
{
    _effect = effect;
    update();
}

BaseEffect* PreviewRenderer::getEffect() const
{
    return _effect;
}

void PreviewRenderer::resetView()
{
    _settings->loadSettings(_rotationX, _rotationY, _rotationZ, _zoom);
    _panOffset = QPoint(0, 0);
    update();
}

void PreviewRenderer::setShowGrid(bool show)
{
    _showGrid = show;
    update();
}

void PreviewRenderer::toggleViewMode()
{
    _is3DMode = !_is3DMode;
    update();
}

bool PreviewRenderer::isIn3DMode() const
{
    return _is3DMode;
}

void PreviewRenderer::toggleDeviceOnlyMode()
{
    _deviceOnlyMode = !_deviceOnlyMode;
    printf("[Lightscape] Device-only mode %s. %d device positions tracked.\n", 
           _deviceOnlyMode ? "enabled" : "disabled", _devicePositions.size());
    update();
}

bool PreviewRenderer::isInDeviceOnlyMode() const
{
    return _deviceOnlyMode;
}

void PreviewRenderer::setDevicePositions(const QSet<GridPosition>& devicePositions)
{
    _devicePositions = devicePositions;
    printf("[Lightscape] Set %d device positions in the preview.\n", devicePositions.size());
    update();
}

void PreviewRenderer::saveCurrentViewAsDefault()
{
    _settings->saveSettings(_rotationX, _rotationY, _rotationZ, _zoom);
}

void PreviewRenderer::setActiveLayer(int layer)
{
    if (_grid) {
        GridDimensions dims = _grid->GetDimensions();
        if (layer >= 0 && layer < dims.depth) {
            _activeLayer = layer;
            emit activeLayerChanged(layer);
            update();
        }
    }
}

int PreviewRenderer::getActiveLayer() const
{
    return _activeLayer;
}

bool PreviewRenderer::showLayerDialog()
{
    if (!_grid) return false;
    
    GridDimensions dims = _grid->GetDimensions();
    
    bool ok;
    int layer = QInputDialog::getInt(
        this, 
        "Change Layer", 
        "Select Layer (1-" + QString::number(dims.depth) + "):", 
        _activeLayer + 1, 
        1, 
        dims.depth, 
        1, 
        &ok
    );
    
    if (ok) {
        setActiveLayer(layer - 1); // Convert from 1-based to 0-based
        return true;
    }
    
    return false;
}

void PreviewRenderer::setAnimated(bool animated)
{
    _animated = animated;
    
    if (_animated) {
        _updateTimer->start();
    } else {
        _updateTimer->stop();
    }
}

void PreviewRenderer::updatePreview()
{
    // Just trigger a repaint
    update();
}

void PreviewRenderer::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // Fill background
    painter.fillRect(rect(), QColor(40, 40, 40));
    
    // If no grid, show placeholder
    if (!_grid) {
        painter.setPen(QColor(150, 150, 150));
        painter.drawText(rect(), Qt::AlignCenter, "No spatial grid available");
        return;
    }
    
    // Draw according to current mode
    if (_is3DMode) {
        _renderer3D.draw(painter, width(), height(), _grid, _effect, 
                        _rotationX, _rotationY, _rotationZ, _zoom, 
                        _panOffset, _showGrid, _deviceOnlyMode, _devicePositions);
    } else {
        _renderer2D.draw(painter, width(), height(), _grid, _effect, 
                        _activeLayer, _zoom, _panOffset, _showGrid, 
                        _deviceOnlyMode, _devicePositions);
    }
}

void PreviewRenderer::mousePressEvent(QMouseEvent* event)
{
    _interaction->mousePressEvent(event);
}

void PreviewRenderer::mouseMoveEvent(QMouseEvent* event)
{
    _interaction->mouseMoveEvent(event);
}

void PreviewRenderer::mouseReleaseEvent(QMouseEvent* event)
{
    _interaction->mouseReleaseEvent(event);
}

void PreviewRenderer::wheelEvent(QWheelEvent* event)
{
    _interaction->wheelEvent(event);
}

void PreviewRenderer::keyPressEvent(QKeyEvent* event)
{
    _interaction->keyPressEvent(event);
    if (!event->isAccepted()) {
        QWidget::keyPressEvent(event);
    }
}

void PreviewRenderer::keyReleaseEvent(QKeyEvent* event)
{
    _interaction->keyReleaseEvent(event);
    if (!event->isAccepted()) {
        QWidget::keyReleaseEvent(event);
    }
}

void PreviewRenderer::contextMenuEvent(QContextMenuEvent* event)
{
    _interaction->contextMenuEvent(event);
}

// Interaction slots
void PreviewRenderer::onRotationChanged(float deltaX, float deltaY)
{
    if (_is3DMode) {
        _rotationY += deltaX;
        _rotationX += deltaY;
        
        // Normalize Y rotation for full 360-degree rotation
        while (_rotationY > 2.0f * static_cast<float>(M_PI)) _rotationY -= 2.0f * static_cast<float>(M_PI);
        while (_rotationY < -2.0f * static_cast<float>(M_PI)) _rotationY += 2.0f * static_cast<float>(M_PI);
        
        // For X rotation, limit to slightly less than 90 degrees to avoid gimbal lock
        float maxPitch = static_cast<float>(M_PI/2.0f - 0.1f);
        _rotationX = std::max(-maxPitch, std::min(maxPitch, _rotationX));
    } else {
        // In 2D mode, use as pan instead
        _panOffset += QPoint(deltaX * 200, deltaY * 200);
    }
    
    update();
}

void PreviewRenderer::onRollChanged(float delta)
{
    if (_is3DMode) {
        _rotationZ += delta;
        
        // Normalize Z rotation to keep within reasonable range
        while (_rotationZ > 2.0f * static_cast<float>(M_PI)) _rotationZ -= 2.0f * static_cast<float>(M_PI);
        while (_rotationZ < -2.0f * static_cast<float>(M_PI)) _rotationZ += 2.0f * static_cast<float>(M_PI);
        
        update();
    }
}

void PreviewRenderer::onPanChanged(const QPoint& delta)
{
    _panOffset += delta;
    update();
}

void PreviewRenderer::onZoomChanged(float factor)
{
    _zoom *= factor;
    _zoom = std::max(0.1f, std::min(_zoom, 10.0f)); // Apply limits
    update();
}

void PreviewRenderer::onViewReset()
{
    resetView();
}

void PreviewRenderer::onActiveLayerChanged(int delta)
{
    if (_grid) {
        GridDimensions dims = _grid->GetDimensions();
        int newLayer = _activeLayer + delta;
        if (newLayer >= 0 && newLayer < dims.depth) {
            setActiveLayer(newLayer);
        }
    }
}

void PreviewRenderer::onViewSaved()
{
    saveCurrentViewAsDefault();
}

} // namespace Lightscape