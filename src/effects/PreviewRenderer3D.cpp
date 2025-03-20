/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewRenderer3D.cpp                                     |
|                                                           |
| 3D Renderer for effect preview                            |
\*---------------------------------------------------------*/

#include "effects/PreviewRenderer3D.h"
#include <algorithm>
#include <cmath>

namespace Lightscape {

PreviewRenderer3D::PreviewRenderer3D()
{
}

void PreviewRenderer3D::draw(QPainter& painter, int width, int height, SpatialGrid* grid, BaseEffect* effect, 
                           float rotationX, float rotationY, float rotationZ, float zoom, 
                           const QPoint& panOffset, bool showGrid, bool deviceOnlyMode, 
                           const QSet<GridPosition>& devicePositions)
{
    if (!grid) return; // Only check for grid, allow preview with no effect
    
    // Get grid dimensions
    GridDimensions dims = grid->GetDimensions();
    
    // Draw mode indicator and effect title
    painter.setPen(QColor(200, 200, 200));
    QRect infoRect(10, 10, width - 20, 20);
    
    QString modeText = "3D View (Stacked Layers)";
    
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
    
    // Calculate cell size based on view dimensions
    float baseSize = std::min((width - 80) / (float)dims.width, 
                             (height - 100) / (float)dims.height);
    float cellSize = baseSize * zoom * 0.7f;
    
    // Calculate center of view
    float centerX = width / 2 + panOffset.x();
    float centerY = height / 2 + panOffset.y();
    
    // Create transformation matrix
    QMatrix4x4 transform;
    transform.translate(0, 0, 0); // Start at origin
    transform.rotate(rotationY * 180.0f / M_PI, 0, 1, 0); // Y-axis rotation (yaw)
    transform.rotate(rotationX * 180.0f / M_PI, 1, 0, 0); // X-axis rotation (pitch)
    transform.rotate(rotationZ * 180.0f / M_PI, 0, 0, 1); // Z-axis rotation (roll)
    
    // The layer spacing in 3D space
    float layerSpacing = cellSize * 1.5f;
    
    // Draw grid outlines if showing grid
    if (showGrid) {
        drawGridOutlines(painter, dims, cellSize, layerSpacing, transform, centerX, centerY, grid);
    }
    
    // Collect all cells
    std::vector<CellInfo> cells;
    
    // Add all cells with their 3D positions
    for (int z = 0; z < dims.depth; z++) {
        // Z position for this layer (centered around origin)
        float zPos = (z - (dims.depth-1)/2.0f) * layerSpacing;
        
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                
                // Get color from effect or use default if no effect is active
                RGBColor rgbColor;
                
                if (!effect || !effect->getEnabled()) {
                    // Use a checkerboard pattern for visibility when no effect or disabled effect
                    bool isOdd = ((pos.x + pos.y + pos.z) % 2 == 1);
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
                            // Use the effect's internal time to match real device animation
                            rgbColor = effect->getColorForPosition(pos, effect->getInternalTime());
                        }
                    } else {
                        rgbColor = effect->getColorForPosition(pos, effect->getInternalTime());
                    }
                }
                QColor cellColor(RGBGetRValue(rgbColor), RGBGetGValue(rgbColor), RGBGetBValue(rgbColor));
                
                // Calculate 3D position (center of cell)
                float xPos = (x - dims.width/2.0f + 0.5f) * cellSize;
                float yPos = (y - dims.height/2.0f + 0.5f) * cellSize;
                QVector3D center(xPos, yPos, zPos);
                
                // Calculate the 8 corners of the cell
                float halfWidth = cellSize * 0.45f;
                float halfHeight = cellSize * 0.45f;
                float halfDepth = cellSize * 0.15f;
                
                QVector<QVector3D> corners;
                // Front face (lower Z)
                corners.append(QVector3D(center.x() - halfWidth, center.y() - halfHeight, center.z() - halfDepth)); // Front top-left
                corners.append(QVector3D(center.x() + halfWidth, center.y() - halfHeight, center.z() - halfDepth)); // Front top-right
                corners.append(QVector3D(center.x() + halfWidth, center.y() + halfHeight, center.z() - halfDepth)); // Front bottom-right
                corners.append(QVector3D(center.x() - halfWidth, center.y() + halfHeight, center.z() - halfDepth)); // Front bottom-left
                // Back face (higher Z)
                corners.append(QVector3D(center.x() - halfWidth, center.y() - halfHeight, center.z() + halfDepth)); // Back top-left
                corners.append(QVector3D(center.x() + halfWidth, center.y() - halfHeight, center.z() + halfDepth)); // Back top-right
                corners.append(QVector3D(center.x() + halfWidth, center.y() + halfHeight, center.z() + halfDepth)); // Back bottom-right
                corners.append(QVector3D(center.x() - halfWidth, center.y() + halfHeight, center.z() + halfDepth)); // Back bottom-left
                
                // Project corners to 2D
                QVector<QPointF> projectedCorners;
                for (const auto& corner : corners) {
                    projectedCorners.append(project(corner, transform, centerX, centerY));
                }
                
                // Get transformed position for depth sorting
                QVector3D transformedPos = transform * center;
                
                // Create position label
                QString label = "P" + QString::number(x + y * dims.width + 1);
                if (grid) {
                    QString gridLabel = grid->GetPositionLabel(pos);
                    if (!gridLabel.isEmpty()) {
                        label = gridLabel;
                    }
                }
                
                // Add to cells list
                CellInfo cellInfo;
                cellInfo.x = x;
                cellInfo.y = y;
                cellInfo.z = z;
                cellInfo.color = cellColor;
                cellInfo.center = center;
                cellInfo.depth = transformedPos.z();
                cellInfo.label = label;
                cellInfo.projectedCorners = projectedCorners;
                
                cells.push_back(cellInfo);
            }
        }
    }
    
    // Sort cells by depth (back to front)
    std::sort(cells.begin(), cells.end(), [](const CellInfo& a, const CellInfo& b) {
        return a.depth > b.depth; // Draw back to front
    });
    
    // Draw all cells
    drawCells(painter, cells, dims, cellSize);
    
    // Draw controls hint
    drawControls(painter, width, height);
}

void PreviewRenderer3D::drawControls(QPainter& painter, int width, int height)
{
    QRect helpRect(10, height - 25, width - 20, 20);
    painter.setPen(QColor(180, 180, 180));
    painter.drawText(helpRect, Qt::AlignRight, 
                   "Drag to rotate | Shift+drag for roll | Scroll to zoom | Space to reset view");
}

void PreviewRenderer3D::drawGridOutlines(QPainter& painter, const GridDimensions& dims, float cellSize, 
                                       float layerSpacing, QMatrix4x4& transform, 
                                       float centerX, float centerY, SpatialGrid* grid)
{
    painter.setPen(QPen(QColor(100, 100, 140), 1.0f));
    
    // Draw layer outlines
    for (int z = 0; z < dims.depth; z++) {
        // Z position for this layer
        float zPos = (z - (dims.depth-1)/2.0f) * layerSpacing;
        
        // Calculate the four corners of the layer
        float halfWidth = dims.width * cellSize / 2.0f;
        float halfHeight = dims.height * cellSize / 2.0f;
        
        QVector<QVector3D> corners;
        corners.append(QVector3D(-halfWidth, -halfHeight, zPos)); // Top-left
        corners.append(QVector3D(halfWidth, -halfHeight, zPos));  // Top-right
        corners.append(QVector3D(halfWidth, halfHeight, zPos));   // Bottom-right
        corners.append(QVector3D(-halfWidth, halfHeight, zPos));  // Bottom-left
        
        // Project to 2D
        QVector<QPointF> projectedCorners;
        for (const auto& corner : corners) {
            projectedCorners.append(project(corner, transform, centerX, centerY));
        }
        
        // Draw the layer outline
        for (int i = 0; i < 4; i++) {
            painter.drawLine(projectedCorners[i], projectedCorners[(i+1)%4]);
        }
        
        // Draw the layer label
        QPointF labelPos = (projectedCorners[0] + projectedCorners[1]) / 2.0f;
        labelPos.ry() -= 15;
        
        painter.setPen(QColor(200, 200, 200));
        QString layerLabel = "Layer " + QString::number(z + 1);
        if (grid) {
            QString gridLabel = grid->GetLayerLabel(z);
            if (!gridLabel.isEmpty()) {
                layerLabel = gridLabel;
            }
        }
        painter.drawText(labelPos, layerLabel);
        
        // Draw grid lines (if not too many cells)
        if (dims.width <= 10 && dims.height <= 10) {
            painter.setPen(QPen(QColor(80, 80, 100), 0.5f));
            
            // Horizontal lines
            for (int y = 1; y < dims.height; y++) {
                float yPos = (y - dims.height/2.0f) * cellSize;
                QPointF p1 = project(QVector3D(-halfWidth, yPos, zPos), transform, centerX, centerY);
                QPointF p2 = project(QVector3D(halfWidth, yPos, zPos), transform, centerX, centerY);
                painter.drawLine(p1, p2);
            }
            
            // Vertical lines
            for (int x = 1; x < dims.width; x++) {
                float xPos = (x - dims.width/2.0f) * cellSize;
                QPointF p1 = project(QVector3D(xPos, -halfHeight, zPos), transform, centerX, centerY);
                QPointF p2 = project(QVector3D(xPos, halfHeight, zPos), transform, centerX, centerY);
                painter.drawLine(p1, p2);
            }
        }
    }
}

void PreviewRenderer3D::drawCells(QPainter& painter, std::vector<CellInfo>& cells, 
                                const GridDimensions& dims, float cellSize)
{
    Q_UNUSED(cellSize); // Mark parameter as unused to avoid compiler warning
    for (const auto& cell : cells) {
        // Apply more distinct shading to different faces for enhanced 3D effect
        QColor baseColor = cell.color;
        QColor topColor = baseColor.lighter(120);     // Lighter top face 
        QColor bottomColor = baseColor.darker(130);   // Darker bottom face
        QColor frontColor = baseColor;                // Base color for front
        QColor backColor = baseColor.darker(140);     // Significantly darker back
        QColor leftColor = baseColor.darker(115);     // Slightly darker left
        QColor rightColor = baseColor.darker(110);    // Slightly darker right
        
        // Draw the cell faces - use solid colors and draw faces in correct order
        painter.setPen(Qt::NoPen); // No pen for solid look
        
        // First draw back faces (those facing away from viewer)
        // Back face
        QPolygonF backFace;
        backFace << cell.projectedCorners[4] << cell.projectedCorners[5] << cell.projectedCorners[6] << cell.projectedCorners[7];
        painter.setBrush(backColor);
        painter.drawPolygon(backFace);
        
        // Bottom face
        QPolygonF bottomFace;
        bottomFace << cell.projectedCorners[3] << cell.projectedCorners[2] << cell.projectedCorners[6] << cell.projectedCorners[7];
        painter.setBrush(bottomColor);
        painter.drawPolygon(bottomFace);
        
        // Right face
        QPolygonF rightFace;
        rightFace << cell.projectedCorners[1] << cell.projectedCorners[2] << cell.projectedCorners[6] << cell.projectedCorners[5];
        painter.setBrush(rightColor);
        painter.drawPolygon(rightFace);
        
        // Then draw front faces
        // Left face
        QPolygonF leftFace;
        leftFace << cell.projectedCorners[0] << cell.projectedCorners[3] << cell.projectedCorners[7] << cell.projectedCorners[4];
        painter.setBrush(leftColor);
        painter.drawPolygon(leftFace);
        
        // Top face
        QPolygonF topFace;
        topFace << cell.projectedCorners[0] << cell.projectedCorners[1] << cell.projectedCorners[5] << cell.projectedCorners[4];
        painter.setBrush(topColor);
        painter.drawPolygon(topFace);
        
        // Front face
        QPolygonF frontFace;
        frontFace << cell.projectedCorners[0] << cell.projectedCorners[1] << cell.projectedCorners[2] << cell.projectedCorners[3];
        painter.setBrush(frontColor);
        painter.drawPolygon(frontFace);
        
        // Add thin dark border to all faces for solid appearance
        QPen borderPen(QColor(30, 30, 30, 80), 0.5f); // Thinner, more transparent border
        painter.setPen(borderPen);
        painter.setBrush(Qt::NoBrush);
        
        // Draw borders for all faces
        painter.drawPolygon(frontFace);
        painter.drawPolygon(topFace);
        painter.drawPolygon(leftFace);
        painter.drawPolygon(rightFace);
        painter.drawPolygon(bottomFace);
        painter.drawPolygon(backFace);
        
        // Draw position label on top face
        if (dims.width <= 8 && dims.height <= 8) {
            // Calculate center of the top face
            QPointF labelPos = (cell.projectedCorners[0] + cell.projectedCorners[1] + 
                               cell.projectedCorners[5] + cell.projectedCorners[4]) / 4.0f;
            
            // Adjust text color based on background brightness
            int brightness = (topColor.red() + topColor.green() + topColor.blue()) / 3;
            painter.setPen(brightness > 128 ? Qt::black : Qt::white);
            
            // Draw the label
            QRectF textRect(labelPos.x() - 12, labelPos.y() - 8, 24, 16);
            painter.drawText(textRect, Qt::AlignCenter, cell.label);
        }
    }
}

QPointF PreviewRenderer3D::project(const QVector3D& point, QMatrix4x4& transform, float centerX, float centerY)
{
    QVector3D transformed = transform * point;
    float perspectiveFactor = 1.0f + transformed.z() / 1000.0f;
    return QPointF(
        centerX + transformed.x() / perspectiveFactor,
        centerY + transformed.y() / perspectiveFactor
    );
}

} // namespace Lightscape