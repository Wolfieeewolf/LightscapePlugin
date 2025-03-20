/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectsPreviewPanel.h                                     |
|                                                           |
| Panel for effect preview                                  |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QSet>
#include "effects/BaseEffect.h"
#include "effects/PreviewRenderer.h"
#include "grid/SpatialGrid.h"

namespace Ui {
class EffectsPreviewPanel;
}

namespace Lightscape {

class EffectsPreviewPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectsPreviewPanel(QWidget *parent = nullptr);
    ~EffectsPreviewPanel();
    
    // Initialize with grid
    void setGrid(::SpatialGrid* grid);
    
    // Effect management
    void setEffect(BaseEffect* effect);
    void clearEffect();
    
    // View control
    void resetView();
    void toggleViewMode();
    void setActiveLayer(int layer);
    void showLayerDialog();
    
    // Expose the renderer
    PreviewRenderer* getRenderer() const;
    
    // Device-only mode
    void toggleDeviceOnlyMode();
    void setDevicePositions(const QSet<GridPosition>& devicePositions);
    
    signals:
    void resetViewClicked();
    void toggleViewClicked();
    void changeLayerClicked();
    void deviceOnlyModeToggled(bool enabled);
    
private:
    Ui::EffectsPreviewPanel *ui;
    PreviewRenderer* _previewRenderer;
    
    void setupUI();
};

} // namespace Lightscape