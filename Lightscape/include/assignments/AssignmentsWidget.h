#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include "core/DeviceManager.h"
#include "grid/SpatialGrid.h"

namespace Ui {
class AssignmentsWidget;
}

class AssignmentsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AssignmentsWidget(DeviceManager* deviceManager = nullptr, 
                             SpatialGrid* spatialGrid = nullptr, 
                             QWidget *parent = nullptr);
    ~AssignmentsWidget();

    void updateAssignmentsList();
    void clearAssignments();
    void setDeviceControlWidget(QWidget* widget) { deviceControlWidget = widget; }

    // Size management
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void assignRequested(int deviceIndex, QString selectionName, GridPosition pos);
    void removeRequested(QListWidgetItem* item);
    void colorChangeRequested(QColor color);

private slots:
    void onAssignButtonClicked();
    void onRemoveButtonClicked();
    void onClearButtonClicked();
    void onColorButtonClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::AssignmentsWidget *ui;
    DeviceManager *deviceManager;
    SpatialGrid *spatialGrid;
    QWidget *deviceControlWidget;

    QSize calculateNeededSize() const;
};