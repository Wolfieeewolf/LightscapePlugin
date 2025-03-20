/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewRenderer2D.cpp                                     |
|                                                           |
| 2D Renderer for effect preview                            |
\*---------------------------------------------------------*/

#include "effects/PreviewRenderer2D.h"
#include <algorithm>

namespace Lightscape {

PreviewRenderer2D::PreviewRenderer2D()
{
}

void PreviewRenderer2D::draw(QPainter& painter, int width, int height, SpatialGrid* grid, BaseEffect* effect, 
                           int activeLayer, float zoom, const QPoint& panOffset, bool showGrid, 
                           bool deviceOnlyMode, const QSet<GridPosition>& devicePositions)
{
    if (!grid) return; // Only check for grid, allow preview with no effect
    
    // Get grid dimensions
    GridDimensions dims = grid->GetDimensions();
    
    // Draw mode indicator and effect title
    painter.setPen(QColor(200, 200, 200));
    QRect infoRect(10, 10, width - 20, 20);
    
    QString modeText = QString("2D View - Layer %1 of %2").arg(activeLayer + 1).arg(dims.depth);
    
    // Show effect status
    if (effect) {
        if (effect->getEnabled()) {
            modeText += " (" + effect->GetStaticInfo().name + " - Running)";
        } else {
            modeText += " (" + effect->GetStaticInfo().name + " - Press play to start)";
        }
    } else {
        modeText += " (No effect selected)";
    }
    
    painter.drawText(infoRect, Qt::AlignLeft, modeText);
    
    // Calculate cell size based on grid dimensions, view size, and zoom
    float baseSize = std::min((width - 40) / (float)dims.width, 
                             (height - 60) / (float)dims.height);
    float cellSize = baseSize * zoom;
    
    // Calculate offset to center the grid in the view
    float offsetX = (width - cellSize * dims.width) / 2 + panOffset.x();
    float offsetY = 30 + (height - 30 - cellSize * dims.height) / 2 + panOffset.y();
    
    // Draw grid background (lines)
    if (showGrid) {
        drawGrid(painter, dims, cellSize, offsetX, offsetY);
    }
    
    // Draw grid cells with colors from the effect
    drawCells(painter, dims, cellSize, offsetX, offsetY, grid, effect, activeLayer, 
             deviceOnlyMode, devicePositions);
    
    // Draw controls hint
    drawControls(painter, width, height);
}

void PreviewRenderer2D::drawControls(QPainter& painter, int width, int height)
{
    QRect helpRect(10, height - 25, width - 20, 20);
    painter.setPen(QColor(180, 180, 180));
    painter.drawText(helpRect, Qt::AlignRight, 
                   "Drag to pan | Scroll to zoom | ←→ to change layer");
}

void PreviewRenderer2D::drawGrid(QPainter& painter, const GridDimensions& dims, float cellSize, 
                               float offsetX, float offsetY)
{
    painter.setPen(QPen(QColor(70, 70, 70)));
    
    // Draw horizontal lines
    for (int y = 0; y <= dims.height; y++) {
        float yPos = offsetY + y * cellSize;
        painter.drawLine(offsetX, yPos, offsetX + dims.width * cellSize, yPos);
    }
    
    // Draw vertical lines
    for (int x = 0; x <= dims.width; x++) {
        float xPos = offsetX + x * cellSize;
        painter.drawLine(xPos, offsetY, xPos, offsetY + dims.height * cellSize);
    }
}

void PreviewRenderer2D::drawCells(QPainter& painter, const GridDimensions& dims, float cellSize, 
                                float offsetX, float offsetY, SpatialGrid* grid, BaseEffect* effect, 
                                int activeLayer, bool deviceOnlyMode, const QSet<GridPosition>& devicePositions)
{
    for (int y = 0; y < dims.height; y++) {
        for (int x = 0; x < dims.width; x++) {
            GridPosition pos(x, y, activeLayer);
            
            // Get color from effect or use default if no effect is active
            RGBColor rgbColor;
            
            if (!effect || !effect->getEnabled()) {
                // Use a checkerboard pattern for visibility when no effect or disabled effect
                bool isOdd = ((pos.x + pos.y) % 2 == 1);
                rgbColor = isOdd ? ToRGBColor(60, 60, 70) : ToRGBColor(50, 50, 60);
                
                // Highlight device positions if available
                if (deviceOnlyMode && devicePositions.contains(pos)) {
                    rgbColor = ToRGBColor(80, 80, 100);  // Slightly brighter for devices
                }
            } else {
                // In device-only mode, use default color for non-device positions
                if (deviceOnlyMode) {
                    // Use direct set lookup instead of manual looping
                    bool isDevicePosition = devicePositions.contains(pos);
                    
                    if (!isDevicePosition) {
                        // Use a default grey for non-device positions
                        rgbColor = ToRGBColor(100, 100, 100); // Medium grey
                    } else {
                        rgbColor = effect->getColorForPosition(pos, effect->getInternalTime());
                    }
                } else {
                    // Use the effect's internal time to match real device animation
                    rgbColor = effect->getColorForPosition(pos, effect->getInternalTime());
                }
            }
            
            // Convert to QColor
            QColor cellColor = QColor(
                RGBGetRValue(rgbColor),
                RGBGetGValue(rgbColor),
                RGBGetBValue(rgbColor)
            );
            
            // Draw cell
            QRectF cellRect(
                offsetX + x * cellSize + 1,
                offsetY + y * cellSize + 1,
                cellSize - 2,
                cellSize - 2
            );
            
            painter.setPen(Qt::black);
            painter.setBrush(cellColor);
            painter.drawRect(cellRect);
            
            // Draw position label for small grids
            if (dims.width <= 8 && dims.height <= 8 && cellSize >= 30) {
                painter.setPen(Qt::black);
                
                // Use position labels like P1, P2, P3 instead of coordinates
                QString label = "P" + QString::number(x + y * dims.width + 1);
                
                // If grid has position labels, use those instead
                if (grid) {
                    QString gridLabel = grid->GetPositionLabel(pos);
                    if (!gridLabel.isEmpty()) {
                        label = gridLabel;
                    }
                }
                
                painter.drawText(cellRect, Qt::AlignCenter, label);
            }
        }
    }
}

} // namespace Lightscape