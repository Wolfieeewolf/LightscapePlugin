/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewRenderer3D.h                                       |
|                                                           |
| 3D Renderer for effect preview                            |
\*---------------------------------------------------------*/

#pragma once

#include <QPainter>
#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QPointF>
#include <QMatrix4x4>
#include "grid/GridTypes.h"
#include "grid/SpatialGrid.h"
#include "effects/BaseEffect.h"

namespace Lightscape {

class PreviewRenderer3D {
public:
    PreviewRenderer3D();
    ~PreviewRenderer3D() = default;

    void draw(QPainter& painter, int width, int height, SpatialGrid* grid, BaseEffect* effect, 
              float rotationX, float rotationY, float rotationZ, float zoom, 
              const QPoint& panOffset, bool showGrid, bool deviceOnlyMode, 
              const QSet<GridPosition>& devicePositions);

private:
    // Helper structs and functions
    struct CellInfo {
        int x, y, z;                    // Grid position
        QColor color;                   // Cell color
        QVector3D center;               // 3D position
        float depth;                    // For depth sorting
        QString label;                  // Position label
        QVector<QPointF> projectedCorners; // Projected corners
    };

    void drawControls(QPainter& painter, int width, int height);
    void drawGridOutlines(QPainter& painter, const GridDimensions& dims, float cellSize, 
                         float layerSpacing, QMatrix4x4& transform, 
                         float centerX, float centerY, SpatialGrid* grid);
    void drawCells(QPainter& painter, std::vector<CellInfo>& cells, 
                  const GridDimensions& dims, float cellSize);
    
    QPointF project(const QVector3D& point, QMatrix4x4& transform, float centerX, float centerY);
};

} // namespace Lightscape