#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QList>
#include "SpatialGrid.h"
#include "ui_GridSettingsDialog.h"

namespace Ui {
class GridSettingsDialog;
}

class GridSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GridSettingsDialog(SpatialGrid* grid, QWidget* parent = nullptr);
    ~GridSettingsDialog();

signals:
    void gridUpdated();
    void userPositionRequired(const QString& warning);
    void userPositionChanged(const GridPosition& pos);

private slots:
    void on_dimensionsChanged();
    void on_labelChanged(int row, int column);
    void on_layerLabelChanged(int index);
    void on_applyClicked();
    void on_resetDefaultsClicked();
    void on_setUserPositionButton_clicked();
    void on_clearUserPositionButton_clicked();

private:
    Ui::GridSettingsDialog* ui;
    SpatialGrid* grid;
    QList<QTableWidget*> labelTables;

    void setupUi();
    void updateLabels();
    void createLabelTables();
    void populateCurrentSettings();
    void updateCellColors();
};