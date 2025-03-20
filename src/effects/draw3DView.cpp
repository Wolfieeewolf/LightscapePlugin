// This is a temporary file containing the fixed draw3DView function
// Copy this function into PreviewRenderer.cpp to replace the existing one

void PreviewRenderer::draw3DView(QPainter& painter)
{
    if (!_grid || !_effect) return;
    
    // Get grid dimensions
    GridDimensions dims = _grid->GetDimensions();
    
    // Draw mode indicator
    painter.setPen(QColor(200, 200, 200));
    QRect infoRect(10, 10, width() - 20, 20);
    painter.drawText(infoRect, Qt::AlignLeft, "3D View (Stacked Layers)");
    
    // Calculate the base size for cells similar to 2D view
    float baseSize = std::min((width() - 40) / (float)dims.width, 
                             (height() - 60) / (float)dims.height);
    float cellSize = baseSize * _zoom * 0.7f; // Scale down slightly to fit all layers
    
    // Calculate center of view for rotation
    float centerX = width() / 2 + _panOffset.x();
    float centerY = height() / 2 + _panOffset.y();
    
    // Calculate size of a single layer
    float layerWidth = dims.width * cellSize;
    float layerHeight = dims.height * cellSize;
    
    // Calculate the layer offset (how much to shift each layer)
    float layerOffsetX = 0.0f; // No horizontal shift - stack vertically
    float layerOffsetY = 0.0f; // No diagonal shift 
    float layerOffsetZ = cellSize * 1.5f; // Vertical spacing between layers (increased for more separation)
    
    // Create a rotation matrix with perspective effect
    QTransform rotationMatrix;
    rotationMatrix.translate(centerX, centerY); // Move to center
    
    // Calculate the scale factor based on z-rotation for perspective effect
    float zScale = 1.0f - _rotationX * 0.15f; // Subtle perspective effect
    
    // Apply rotation and scaling
    rotationMatrix.rotate(_rotationY * 180.0f / M_PI); // Yaw (horizontal rotation)
    rotationMatrix.rotate(_rotationX * 180.0f / M_PI); // Pitch (vertical rotation)
    rotationMatrix.scale(zScale, zScale); // Apply perspective scaling
    
    // Use reverse painter order to draw back-to-front based on rotation
    int startZ, endZ, stepZ;
    if (sin(_rotationY) > 0) {
        // Draw front to back (layer 0 first)
        startZ = 0;
        endZ = dims.depth;
        stepZ = 1;
    } else {
        // Draw back to front (highest layer first)
        startZ = dims.depth - 1;
        endZ = -1;
        stepZ = -1;
    }
    
    // Draw layers in the appropriate order
    for (int z = startZ; z != endZ; z += stepZ) {
        // Calculate the unrotated layer position (centered around origin)
        // Use z-axis for layer stacking (cube-like arrangement)
        float layerCenterX = -(layerWidth / 2);
        float layerCenterY = -(layerHeight / 2);
        float layerCenterZ = (z - dims.depth/2.0f) * layerOffsetZ; // Center around the middle layer
        
        // Create points for the four corners of the layer with z-offset for 3D effect
        // Convert the 3D points to 2D with perspective
        float z3D = layerCenterZ / 500.0f; // Scaled for subtle perspective
        float perspectiveFactor = 1.0f / (1.0f + z3D);
        
        // Calculate 3D coordinates for the corners
        QPointF topLeft3D(layerCenterX, layerCenterY);
        QPointF topRight3D(layerCenterX + layerWidth, layerCenterY);
        QPointF bottomRight3D(layerCenterX + layerWidth, layerCenterY + layerHeight);
        QPointF bottomLeft3D(layerCenterX, layerCenterY + layerHeight);
        
        // Apply the rotation transform to get 2D projected points
        QPointF topLeft = rotationMatrix.map(topLeft3D) * perspectiveFactor;
        QPointF topRight = rotationMatrix.map(topRight3D) * perspectiveFactor;
        QPointF bottomRight = rotationMatrix.map(bottomRight3D) * perspectiveFactor;
        QPointF bottomLeft = rotationMatrix.map(bottomLeft3D) * perspectiveFactor;
        
        // Use z-coordinate for layer visibility
        // Get the rotated z value for this layer (simplified calculation)
        float rotatedZ = sin(_rotationY) * layerCenterX + sin(_rotationX) * layerCenterY + cos(_rotationY) * layerCenterZ;
        
        // Apply rotation to get the actual drawing position for the layer label
        QPointF layerOrigin = rotationMatrix.map(QPointF(layerCenterX, layerCenterY)) * perspectiveFactor;
        
        // Draw layer label
        painter.setPen(QColor(200, 200, 200));
        QString layerLabel = "Layer " + QString::number(z + 1);
        if (_grid) {
            QString gridLayerLabel = _grid->GetLayerLabel(z);
            if (!gridLayerLabel.isEmpty()) {
                layerLabel = gridLayerLabel;
            }
        }
        painter.drawText(layerOrigin.x(), layerOrigin.y() - 10, layerLabel);
        
        // Draw grid lines if enabled
        if (_showGrid) {
            // Draw horizontal and vertical grid lines
            painter.setPen(QPen(QColor(70, 70, 70)));
            
            // Draw horizontal lines
            for (int y = 0; y <= dims.height; y++) {
                float linePos = static_cast<float>(y) / dims.height;
                QPointF left = topLeft + linePos * (bottomLeft - topLeft);
                QPointF right = topRight + linePos * (bottomRight - topRight);
                painter.drawLine(left, right);
            }
            
            // Draw vertical lines
            for (int x = 0; x <= dims.width; x++) {
                float linePos = static_cast<float>(x) / dims.width;
                QPointF top = topLeft + linePos * (topRight - topLeft);
                QPointF bottom = bottomLeft + linePos * (bottomRight - bottomLeft);
                painter.drawLine(top, bottom);
            }
            
            // Draw layer outline with a stronger color
            painter.setPen(QPen(QColor(100, 100, 140), 2));
            QPolygonF outlinePolygon;
            outlinePolygon << topLeft << topRight << bottomRight << bottomLeft;
            painter.drawPolygon(outlinePolygon);
        }
        
        // Draw grid cells with colors from the effect
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                
                // Get color from effect
                RGBColor rgbColor = _effect->getColorForPosition(pos, _time);
                
                // Convert to QColor
                QColor cellColor = QColor(
                    RGBGetRValue(rgbColor),
                    RGBGetGValue(rgbColor),
                    RGBGetBValue(rgbColor)
                );
                
                // Calculate the cell's position within the layer
                float cellLeft = static_cast<float>(x) / dims.width;
                float cellTop = static_cast<float>(y) / dims.height;
                float cellRight = static_cast<float>(x + 1) / dims.width;
                float cellBottom = static_cast<float>(y + 1) / dims.height;
                
                // Apply interpolation to get the corners of the cell
                QPointF cellTopLeft = topLeft + cellLeft * (topRight - topLeft) + cellTop * (bottomLeft - topLeft);
                QPointF cellTopRight = topLeft + cellRight * (topRight - topLeft) + cellTop * (bottomLeft - topLeft);
                QPointF cellBottomRight = topLeft + cellRight * (topRight - topLeft) + cellBottom * (bottomLeft - topLeft);
                QPointF cellBottomLeft = topLeft + cellLeft * (topRight - topLeft) + cellBottom * (bottomLeft - topLeft);
                
                // Create a polygon for the cell
                QPolygonF cellPoly;
                cellPoly << cellTopLeft << cellTopRight << cellBottomRight << cellBottomLeft;
                
                // Draw cell
                painter.setPen(Qt::black);
                painter.setBrush(cellColor);
                painter.drawPolygon(cellPoly);
                
                // Draw position label for small grids
                if (dims.width <= 8 && dims.height <= 8 && cellSize >= 30) {
                    painter.setPen(Qt::black);
                    
                    // Use position labels like P1, P2, P3 instead of coordinates
                    QString label = "P" + QString::number(x + y * dims.width + 1);
                    
                    // If grid has position labels, use those instead
                    if (_grid) {
                        QString gridLabel = _grid->GetPositionLabel(pos);
                        if (!gridLabel.isEmpty()) {
                            label = gridLabel;
                        }
                    }
                    
                    // Calculate the center of the cell for text
                    QPointF cellCenter = (cellTopLeft + cellBottomRight) / 2.0f;
                    
                    // Create a rectangle for text centering
                    QRectF textRect(cellCenter.x() - cellSize/3, cellCenter.y() - cellSize/3, 
                                   cellSize*2/3, cellSize*2/3);
                    painter.drawText(textRect, Qt::AlignCenter, label);
                }
            }
        }
    }
    
    // Draw controls hint
    QRect helpRect(10, height() - 25, width() - 20, 20);
    painter.setPen(QColor(180, 180, 180));
    painter.drawText(helpRect, Qt::AlignRight, 
                   "Drag to rotate | Scroll to zoom | Space to reset view");
}