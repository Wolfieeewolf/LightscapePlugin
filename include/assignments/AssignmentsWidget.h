#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QMap>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "grid/GridTypes.h"
#include "core/Types.h"

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
    void assignRequested(int deviceIndex, Lightscape::DeviceType type, QString selectionName, GridPosition pos);
    void removeRequested(QListWidgetItem* item);
    void colorChangeRequested(RGBColor color);

private slots:
    void onAssignButtonClicked();
    void onRemoveButtonClicked();
    void onClearButtonClicked();
    void onColorButtonClicked();
    void onDeviceAssignmentChanged(unsigned int index, Lightscape::DeviceType type, bool isAssigned);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::AssignmentsWidget *ui;
    DeviceManager *deviceManager;
    SpatialGrid *spatialGrid;
    QWidget *deviceControlWidget;
    QMap<QPair<unsigned int, Lightscape::DeviceType>, QPair<QString, GridPosition>> assignmentInfo;

    QSize calculateNeededSize() const;
    QString formatDeviceInfo(const unsigned int index, 
                           const Lightscape::DeviceType type,
                           const QString& selectionName) const;
    QString getDeviceTypeString(const Lightscape::DeviceType type) const;
    QString getAssignmentDetails(const unsigned int index,
                               const Lightscape::DeviceType type) const;
};