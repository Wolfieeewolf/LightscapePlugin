#include "core/TestGridVisualizer.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Lightscape {

TestGridVisualizer::TestGridVisualizer(SpatialGrid* grid, QWidget* parent)
    : QWidget(parent)
    , spatialGrid(grid)
    , activeLayer(0)
    , showCoordinates(true)
    , isInteractive(false)
    , isMultiSelectMode(false)
    , selectedPosition(-1, -1, -1)
    , hoveredPosition(-1, -1, -1)
{
    // Set size policy for consistent sizing across tabs
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(250, 250);  // Standard minimum size
    
    // Simple background color
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(240, 240, 240));
    setPalette(pal);
    
    // Enable mouse tracking for position selection
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setCursor(isInteractive ? Qt::PointingHandCursor : Qt::ArrowCursor);
}

void TestGridVisualizer::setActiveLayer(int layer)
{
    if (!spatialGrid || layer < 0 || layer >= spatialGrid->GetDimensions().depth) {
        return;
    }
    
    activeLayer = layer;
    update();
}

void TestGridVisualizer::setPositionColor(const GridPosition& pos, const RGBColor& color)
{
    if (pos.z == activeLayer || pos.z == -1) {
        GridPosition layerPos(pos.x, pos.y, activeLayer);
        positionColors[layerPos] = color;
        update();
    }
}

void TestGridVisualizer::clearColors()
{
    positionColors.clear();
    update();
}

void TestGridVisualizer::toggleCoordinateDisplay(bool show)
{
    showCoordinates = show;
    update();
}

GridPosition TestGridVisualizer::mouseToGridPosition(const QPoint& mousePos) const
{
    if (!spatialGrid) return GridPosition(-1, -1, -1);
    
    GridDimensions dims = spatialGrid->GetDimensions();
    if (dims.width <= 0 || dims.height <= 0) return GridPosition(-1, -1, -1);
    
    // Calculate grid layout
    int cellSize = calculateCellSize();
    int offsetX = (this->width() - (dims.width * cellSize)) / 2;
    int offsetY = (this->height() - (dims.height * cellSize)) / 2;
    
    // Convert mouse position to grid position
    int gridX = (mousePos.x() - offsetX) / cellSize;
    int gridY = (mousePos.y() - offsetY) / cellSize;
    
    // Check if within grid bounds
    if (gridX >= 0 && gridX < dims.width && gridY >= 0 && gridY < dims.height) {
        return GridPosition(gridX, gridY, activeLayer);
    }
    
    return GridPosition(-1, -1, -1);
}

void TestGridVisualizer::mouseMoveEvent(QMouseEvent* event)
{
    if (!spatialGrid) {
        QWidget::mouseMoveEvent(event);
        return;
    }
    
    // Just update the hovered position
    hoveredPosition = mouseToGridPosition(event->pos());
    update(); // Refresh to show hover highlight
    
    QWidget::mouseMoveEvent(event);
}

void TestGridVisualizer::mousePressEvent(QMouseEvent* event)
{
    if (!isInteractive || !spatialGrid || !event) {
        QWidget::mousePressEvent(event);
        return;
    }
    
    if (event->button() == Qt::LeftButton) {
        GridPosition pos = mouseToGridPosition(event->pos());
        
        // Check if position is valid and has assignments
        if (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && spatialGrid->HasAssignments(pos)) {
            selectedPosition = pos;
            
            // Handle multi-select mode - simplified
            if (isMultiSelectMode) {
                if (selectedPositions.contains(pos)) {
                    selectedPositions.removeAll(pos);
                } else {
                    selectedPositions.append(pos);
                }
            } else {
                selectedPositions.clear();
                selectedPositions.append(pos);
            }
            
            update();
            emit positionClicked(pos);
        }
    }
    
    QWidget::mousePressEvent(event);
}

void TestGridVisualizer::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        isMultiSelectMode = true;
        setCursor(Qt::DragCopyCursor);
    }
    QWidget::keyPressEvent(event);
}

void TestGridVisualizer::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control) {
        isMultiSelectMode = false;
        setCursor(isInteractive ? Qt::PointingHandCursor : Qt::ArrowCursor);
    }
    QWidget::keyReleaseEvent(event);
}

int TestGridVisualizer::calculateCellSize() const
{
    if (!spatialGrid) return 0;
    
    GridDimensions dims = spatialGrid->GetDimensions();
    if (dims.width <= 0 || dims.height <= 0) return 0;
    
    // Calculate the cell size to fit in the available space
    int availableWidth = this->width() - 40;  // Allow for margins
    int availableHeight = this->height() - 60; // Allow for header and margins
    
    int cellWidth = availableWidth / dims.width;
    int cellHeight = availableHeight / dims.height;
    
    return qMin(cellWidth, cellHeight);
}

void TestGridVisualizer::paintEvent(QPaintEvent* /*event*/)
{
    if (!spatialGrid) return;
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    GridDimensions dims = spatialGrid->GetDimensions();
    if (dims.width <= 0 || dims.height <= 0) return;
    
    // Calculate grid layout
    int cellSize = calculateCellSize();
    int offsetX = (width() - (dims.width * cellSize)) / 2;
    int offsetY = (height() - (dims.height * cellSize)) / 2;
    
    // Draw layer header
    QString layerName = spatialGrid->GetLayerLabel(activeLayer);
    if (layerName.isEmpty()) {
        layerName = QString("Layer %1").arg(activeLayer);
    }
    
    QString layerHeader = QString("%1 (Z=%2)").arg(layerName).arg(activeLayer);
    
    QFont headerFont = painter.font();
    headerFont.setPointSize(10);
    headerFont.setBold(true);
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    
    QRect headerRect(0, 10, this->width(), 20);
    painter.drawText(headerRect, Qt::AlignCenter, layerHeader);
    
    // Draw grid
    for (int y = 0; y < dims.height; y++) {
        for (int x = 0; x < dims.width; x++) {
            GridPosition pos(x, y, activeLayer);
            QRect cellRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize);
            
            // Draw cell background
            QColor bgColor = QColor(Qt::white);
            bool hasAssignment = spatialGrid->HasAssignments(pos);
            
            // Color cells based on position colors or assignment
            if (positionColors.contains(pos)) {
                RGBColor rgbColor = positionColors[pos];
                bgColor = QColor(RGBGetRValue(rgbColor), RGBGetGValue(rgbColor), RGBGetBValue(rgbColor));
            } else if (hasAssignment) {
                bgColor = QColor(255, 170, 0, 150); // Orange for assigned positions
            }
            
            painter.fillRect(cellRect, bgColor);
            
            // Draw cell border
            painter.setPen(QColor(100, 100, 100));
            painter.drawRect(cellRect);
            
            // Highlight user position with green border
            if (spatialGrid->HasUserPosition() && spatialGrid->GetUserPosition().value() == pos) {
                painter.setPen(QPen(QColor(0, 150, 0), 2));
                painter.drawRect(cellRect.adjusted(2, 2, -2, -2));
            }
            
            // Highlight selected position with blue border
            if (selectedPositions.contains(pos)) {
                painter.setPen(QPen(QColor(0, 100, 200), 2));
                painter.drawRect(cellRect.adjusted(2, 2, -2, -2));
            }
            
            // Highlight hovered position only if interactive
            if (isInteractive && hoveredPosition == pos && hasAssignment) {
                painter.setPen(QPen(QColor(150, 0, 150, 150), 1));
                painter.drawRect(cellRect.adjusted(1, 1, -1, -1));
            }
            
            // Draw position label and coordinates
            QFont labelFont = painter.font();
            labelFont.setPointSize(8);
            painter.setFont(labelFont);
            painter.setPen(Qt::black);
            
            // Show position label at top
            QString label = spatialGrid->GetPositionLabel(pos);
            if (label.isEmpty()) {
                label = QString("P(%1,%2)").arg(x).arg(y);
            }
            
            QRect labelRect = cellRect;
            labelRect.setHeight(cellRect.height() / 2);
            painter.drawText(labelRect, Qt::AlignCenter, label);
            
            // Show coordinates at bottom if enabled
            if (showCoordinates) {
                QString coordText = QString("(%1,%2)").arg(x).arg(y);
                QRect coordRect = cellRect;
                coordRect.setTop(cellRect.top() + cellRect.height() / 2);
                painter.drawText(coordRect, Qt::AlignCenter, coordText);
            }
            
            // Gray out non-interactive positions
            if (isInteractive && !hasAssignment) {
                QColor overlayColor(200, 200, 200, 120);
                painter.fillRect(cellRect, overlayColor);
            }
        }
    }
}

} // namespace Lightscape