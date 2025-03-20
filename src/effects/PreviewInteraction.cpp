/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewInteraction.cpp                                    |
|                                                           |
| Mouse and keyboard handling for preview renderer          |
\*---------------------------------------------------------*/

#include "effects/PreviewInteraction.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSettings>
#include <QInputDialog>
#include <cmath>

namespace Lightscape {

PreviewInteraction::PreviewInteraction(QWidget* parent)
    : QObject(parent)
    , _parent(parent)
{
    // Nothing to initialize
}

void PreviewInteraction::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        _isDragging = true;
        _prevMousePos = event->pos();
        _parent->setCursor(Qt::ClosedHandCursor);
        _isShiftPressed = event->modifiers() & Qt::ShiftModifier;
        event->accept();
    }
}

void PreviewInteraction::mouseMoveEvent(QMouseEvent* event)
{
    if (_isDragging) {
        QPoint deltaPos = event->pos() - _prevMousePos;
        _prevMousePos = event->pos();
        
        // Update shift state in case user presses/releases shift while dragging
        bool currentShift = event->modifiers() & Qt::ShiftModifier;
        if (currentShift != _isShiftPressed) {
            _isShiftPressed = currentShift;
        }
        
        if (_isShiftPressed) {
            // If Shift is pressed, emit roll signal
            emit rollChanged(deltaPos.x() * 0.005f);
        } else {
            // Normal rotation
            emit rotationChanged(deltaPos.x() * 0.005f, deltaPos.y() * 0.005f);
        }
        
        event->accept();
    }
}

void PreviewInteraction::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        _isDragging = false;
        _parent->setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

void PreviewInteraction::wheelEvent(QWheelEvent* event)
{
    // Handle zooming
    int delta = event->angleDelta().y();
    
    // Calculate zoom multiplier for smoother zoom (exponential)
    float zoomMultiplier = (delta > 0) ? 1.1f : 0.9f;
    
    // Emit zoom change
    emit zoomChanged(zoomMultiplier);
    
    event->accept();
}

void PreviewInteraction::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        // Reset view
        emit viewReset();
        event->accept();
    } else if (event->key() == Qt::Key_Left) {
        // Previous layer
        emit activeLayerChanged(-1);
        event->accept();
    } else if (event->key() == Qt::Key_Right) {
        // Next layer
        emit activeLayerChanged(1);
        event->accept();
    } else if (event->key() == Qt::Key_Shift) {
        // Track shift key state for Z-axis rotation
        _isShiftPressed = true;
        event->accept();
    } else {
        // Let the parent handle other keys
        event->ignore();
    }
}

void PreviewInteraction::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Shift) {
        // Track shift key state for Z-axis rotation
        _isShiftPressed = false;
        event->accept();
    } else {
        // Let the parent handle other keys
        event->ignore();
    }
}

void PreviewInteraction::contextMenuEvent(QContextMenuEvent* event)
{
    // Create a context menu
    QMenu contextMenu(_parent);
    
    // Add a simple action to save the current view
    QAction* saveAction = new QAction("Set as Default View", &contextMenu);
    connect(saveAction, &QAction::triggered, this, [this]() {
        emit viewSaved();
    });
    contextMenu.addAction(saveAction);
    
    // Add action to reset to factory default view
    QAction* resetAction = new QAction("Reset to Factory Default", &contextMenu);
    connect(resetAction, &QAction::triggered, this, [this]() {
        // Clear saved settings
        QSettings settings("OpenRGB", "Lightscape");
        settings.beginGroup("PreviewSettings");
        settings.remove(""); // Remove all keys in this group
        settings.endGroup();
        
        // Reset view
        emit viewReset();
        
        QMessageBox::information(_parent, "View Reset", 
                               "The view has been reset to factory defaults.");
    });
    contextMenu.addAction(resetAction);
    
    // Execute the menu
    contextMenu.exec(event->globalPos());
}

void PreviewInteraction::saveCurrentViewAsDefault(float rotationX, float rotationY, float rotationZ, float zoom)
{
    // This function allows users to save the current view orientation as the default
    QSettings settings("OpenRGB", "Lightscape");
    settings.beginGroup("PreviewSettings");
    settings.setValue("DefaultRotationX", rotationX);
    settings.setValue("DefaultRotationY", rotationY);
    settings.setValue("DefaultRotationZ", rotationZ);
    settings.setValue("DefaultZoom", zoom);
    settings.endGroup();
    
    // Show a message box to confirm the settings were saved
    QMessageBox::information(_parent, "Default View Saved", 
                           "The current view has been saved as the default orientation.");
}

void PreviewInteraction::resetView()
{
    emit viewReset();
}

} // namespace Lightscape