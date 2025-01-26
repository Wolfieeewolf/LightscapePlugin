#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include "SpatialGrid.h"

class GridPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GridPanel(SpatialGrid* grid, QWidget* parent = nullptr);
    ~GridPanel();

    void updateDisplay();

signals:
    void gridPositionSelected(const GridPosition& pos);

private slots:
    void onPositionSelected(const GridPosition& pos);

private:
    SpatialGrid* grid;
    QVBoxLayout* mainLayout;
    QTabWidget* layerTabs;
    QList<QTableWidget*> gridTables;

    void createGridTables();
};