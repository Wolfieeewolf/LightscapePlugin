/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewRenderer2D.h                                       |
|                                                           |
| 2D Renderer for effect preview                            |
\*---------------------------------------------------------*/

#pragma once

#include <QPainter>
#include <QColor>
#include "grid/GridTypes.h"
#include "grid/SpatialGrid.h"
#include "effects/BaseEffect.h"

namespace Lightscape {

class PreviewRenderer2D {
public:
    PreviewRenderer2D();
    ~PreviewRenderer2D() = default;

    void draw(QPainter& painter, int width, int height, SpatialGrid* grid, BaseEffect* effect, 
              int activeLayer, float zoom, const QPoint& panOffset, bool showGrid, 
              bool deviceOnlyMode, const QSet<GridPosition>& devicePositions);

private:
    // Helper functions
    void drawControls(QPainter& painter, int width, int height);
    void drawGrid(QPainter& painter, const GridDimensions& dims, float cellSize, 
                 float offsetX, float offsetY);
    void drawCells(QPainter& painter, const GridDimensions& dims, float cellSize, 
                  float offsetX, float offsetY, SpatialGrid* grid, BaseEffect* effect, 
                  int activeLayer, bool deviceOnlyMode, const QSet<GridPosition>& devicePositions);
};

} // namespace Lightscape