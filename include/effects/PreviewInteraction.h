/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewInteraction.h                                      |
|                                                           |
| Mouse and keyboard handling for preview renderer          |
\*---------------------------------------------------------*/

#pragma once

#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include "grid/SpatialGrid.h"

namespace Lightscape {

class PreviewInteraction : public QObject {
    Q_OBJECT

public:
    PreviewInteraction(QWidget* parent);
    ~PreviewInteraction() = default;

    // Event handlers
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);

    // Settings
    void saveCurrentViewAsDefault(float rotationX, float rotationY, float rotationZ, float zoom);
    void resetView();

    // Getters
    bool isDragging() const { return _isDragging; }
    bool isShiftPressed() const { return _isShiftPressed; }
    QPoint getPrevMousePos() const { return _prevMousePos; }

signals:
    void rotationChanged(float deltaX, float deltaY);
    void rollChanged(float delta);
    void panChanged(const QPoint& delta);
    void zoomChanged(float factor);
    void viewReset();
    void activeLayerChanged(int delta);
    void viewSaved();
    void updateRequested();

private:
    QWidget* _parent;
    bool _isDragging = false;
    bool _isShiftPressed = false;
    QPoint _prevMousePos;
};

} // namespace Lightscape