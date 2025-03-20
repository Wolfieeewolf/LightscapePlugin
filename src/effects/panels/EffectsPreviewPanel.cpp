/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectsPreviewPanel.cpp                                   |
|                                                           |
| Panel for effect preview                                  |
\*---------------------------------------------------------*/

#include "effects/panels/EffectsPreviewPanel.h"
#include "ui_EffectsPreviewPanel.h"

namespace Lightscape {

EffectsPreviewPanel::EffectsPreviewPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EffectsPreviewPanel)
    , _previewRenderer(nullptr)
{
    ui->setupUi(this);
    setupUI();
}

EffectsPreviewPanel::~EffectsPreviewPanel()
{
    delete ui;
}

void EffectsPreviewPanel::setupUI()
{
    // Limit the title label height to make it consistent with other headers
    if (ui->titleLabel) {
        ui->titleLabel->setMaximumHeight(25);
    }
    
    // Create the preview renderer
    _previewRenderer = new PreviewRenderer(this);
    _previewRenderer->setMinimumSize(300, 200); // Ensure it has a reasonable minimum size
    _previewRenderer->setVisible(true); // Make sure it's visible
    if (ui->previewContainerLayout) {
        ui->previewContainerLayout->addWidget(_previewRenderer);
    }
    
    // Connect the view control buttons
    if (ui->toggleViewButton) {
        connect(ui->toggleViewButton, &QPushButton::clicked, this, [this]() {
            toggleViewMode();
            emit toggleViewClicked();
        });
    }
    
    if (ui->deviceOnlyButton) {
        connect(ui->deviceOnlyButton, &QPushButton::toggled, this, [this](bool checked) {
            if (_previewRenderer) {
                _previewRenderer->toggleDeviceOnlyMode();
                emit deviceOnlyModeToggled(checked);
            }
        });
    }
    
    if (ui->changeLayerButton) {
        connect(ui->changeLayerButton, &QPushButton::clicked, this, [this]() {
            showLayerDialog();
            emit changeLayerClicked();
        });
    }
    
    if (ui->resetViewButton) {
        connect(ui->resetViewButton, &QPushButton::clicked, this, [this]() {
            resetView();
            emit resetViewClicked();
        });
    }
}

void EffectsPreviewPanel::setGrid(::SpatialGrid* grid)
{
    if (_previewRenderer) {
        printf("[Lightscape] EffectsPreviewPanel: Setting grid %p to renderer\n", (void*)grid);
        _previewRenderer->setGrid(grid);
    } else {
        printf("[Lightscape] ERROR: EffectsPreviewPanel has no renderer!\n");
    }
}

void EffectsPreviewPanel::setEffect(BaseEffect* effect)
{
    if (_previewRenderer) {
        _previewRenderer->setEffect(effect);
    }
}

void EffectsPreviewPanel::clearEffect()
{
    if (_previewRenderer) {
        _previewRenderer->setEffect(nullptr);
    }
}

void EffectsPreviewPanel::resetView()
{
    if (_previewRenderer) {
        _previewRenderer->resetView();
    }
}

void EffectsPreviewPanel::toggleViewMode()
{
    if (_previewRenderer) {
        _previewRenderer->toggleViewMode();
    }
}

void EffectsPreviewPanel::setActiveLayer(int layer)
{
    if (_previewRenderer) {
        _previewRenderer->setActiveLayer(layer);
    }
}

void EffectsPreviewPanel::showLayerDialog()
{
    if (_previewRenderer) {
        _previewRenderer->showLayerDialog();
    }
}

PreviewRenderer* EffectsPreviewPanel::getRenderer() const
{
    return _previewRenderer;
}

void Lightscape::EffectsPreviewPanel::toggleDeviceOnlyMode()
{
    if (_previewRenderer) {
        _previewRenderer->toggleDeviceOnlyMode();
        
        // Update button state if it exists
        if (ui->deviceOnlyButton) {
            ui->deviceOnlyButton->setChecked(_previewRenderer->isInDeviceOnlyMode());
        }
    }
}

void Lightscape::EffectsPreviewPanel::setDevicePositions(const QSet<GridPosition>& devicePositions)
{
    if (_previewRenderer) {
        _previewRenderer->setDevicePositions(devicePositions);
    }
}

} // namespace Lightscape