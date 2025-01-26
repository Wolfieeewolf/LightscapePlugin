#include "grid/GridPanel.h"
#include <QTabWidget>
#include <QPushButton>

GridPanel::GridPanel(SpatialGrid* spatial_grid, QWidget* parent)
    : QWidget(parent)
    , grid(spatial_grid)
    , mainLayout(new QVBoxLayout(this))
    , layerTabs(new QTabWidget(this))
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setContentsMargins(0, 0, 0, 0);
    
    layerTabs->setContentsMargins(0, 0, 0, 0);
    layerTabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(layerTabs);
    
    // Connect signal to update when layer names change
    connect(grid, &SpatialGrid::layerLabelChanged, this, &GridPanel::updateDisplay);
    connect(grid, &SpatialGrid::positionSelected, this, &GridPanel::onPositionSelected);
    
    createGridTables();
}

GridPanel::~GridPanel()
{
}

void GridPanel::onPositionSelected(const GridPosition& pos)
{
    emit gridPositionSelected(pos);
}

void GridPanel::createGridTables()
{
    layerTabs->clear();
    gridTables.clear();

    GridDimensions dims = grid->GetDimensions();
    QVector<QGridLayout*> layouts;

    for (int z = 0; z < dims.depth; z++)
    {
        QWidget* layerWidget = new QWidget();
        layerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layerWidget->setContentsMargins(0, 0, 0, 0);
        
        QGridLayout* gridLayout = new QGridLayout(layerWidget);
        gridLayout->setSpacing(8);
        gridLayout->setContentsMargins(8, 8, 8, 8);
        
        layouts.append(gridLayout);
        layerTabs->addTab(layerWidget, grid->GetLayerLabel(z));
    }

    // Let SpatialGrid create and manage the buttons
    grid->SetupGrid(layouts);
}

void GridPanel::updateDisplay()
{
    createGridTables();
}