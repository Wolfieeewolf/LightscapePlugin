#pragma once

#include <QWidget>
#include <QPainter>
#include <QMap>
#include "grid/SpatialGrid.h"

namespace Lightscape {

/**
 * @brief A mini grid visualization for the test dialog
 * 
 * This widget displays a simplified visualization of the grid
 * to help users understand what's happening during tests.
 */
class TestGridVisualizer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Test Grid Visualizer
     * 
     * @param grid Pointer to the spatial grid
     * @param parent Parent widget
     */
    explicit TestGridVisualizer(SpatialGrid* grid, QWidget* parent = nullptr);
    
    /**
     * @brief Set the active layer to display
     * 
     * @param layer Layer index
     */
    void setActiveLayer(int layer);
    
    /**
     * @brief Update the color of a specific position
     * 
     * @param pos Grid position
     * @param color Color to set
     */
    void setPositionColor(const GridPosition& pos, const RGBColor& color);
    
    /**
     * @brief Clear all visualization colors
     */
    void clearColors();
    
    /**
     * @brief Toggle displaying of coordinates in each cell
     * 
     * @param show Whether to show coordinates
     */
    void toggleCoordinateDisplay(bool show);
    
    /**
     * @brief Set whether the grid is interactive (selectable by clicking)
     * 
     * @param interactive Whether positions can be selected by clicking
     */
    void setInteractive(bool interactive) { 
        isInteractive = interactive; 
        // Update cursor based on interactive state
        setCursor(isInteractive ? (isMultiSelectMode ? Qt::DragCopyCursor : Qt::PointingHandCursor) : Qt::ArrowCursor);
    }
    
    /**
     * @brief Get the currently selected position
     * 
     * @return Selected position or GridPosition(-1,-1,-1) if none
     */
    GridPosition getSelectedPosition() const { return selectedPosition; }
    
    /**
     * @brief Get all selected positions when in multi-select mode
     * 
     * @return List of selected positions
     */
    QList<GridPosition> getSelectedPositions() const { return selectedPositions; }
    
    /**
     * @brief Get whether multi-select mode is active (Ctrl key is pressed)
     * 
     * @return bool True if in multi-select mode
     */
    bool getMultiSelectMode() const { return isMultiSelectMode; }

signals:
    /**
     * @brief Emitted when a position is clicked in interactive mode
     */
    void positionClicked(const GridPosition& pos);

protected:
    /**
     * @brief Paint event handler
     * 
     * @param event Paint event
     */
    void paintEvent(QPaintEvent* event) override;
    
    /**
     * @brief Mouse move event handler for tooltips
     * 
     * @param event Mouse event
     */
    void mouseMoveEvent(QMouseEvent* event) override;
    
    /**
     * @brief Mouse press event handler for selection
     * 
     * @param event Mouse event
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /**
     * @brief Key press event handler for multi-select with Ctrl key
     * 
     * @param event Key event
     */
    void keyPressEvent(QKeyEvent* event) override;
    
    /**
     * @brief Key release event handler for multi-select with Ctrl key
     * 
     * @param event Key event
     */
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    /**
     * @brief Convert mouse coordinates to grid position
     * 
     * @param mousePos Mouse position
     * @return GridPosition or (-1,-1,-1) if outside grid
     */
    GridPosition mouseToGridPosition(const QPoint& mousePos) const;
    
    /**
     * @brief Calculate the cell size based on available space and grid dimensions
     * 
     * @return Cell size in pixels
     */
    int calculateCellSize() const;

    SpatialGrid* spatialGrid;
    int activeLayer;
    QMap<GridPosition, RGBColor> positionColors;
    bool showCoordinates;
    bool isInteractive;
    bool isMultiSelectMode; // Flag for Ctrl key multi-selection
    GridPosition selectedPosition;
    QList<GridPosition> selectedPositions;
    GridPosition hoveredPosition;
};

} // namespace Lightscape