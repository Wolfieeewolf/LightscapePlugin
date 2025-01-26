#pragma once

#include <QWidget>
#include <QComboBox>
#include <QRadioButton>
#include "core/DeviceManager.h"

namespace Ui {
class DeviceControlWidget;
}

class DeviceControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceControlWidget(DeviceManager* deviceManager = nullptr, QWidget *parent = nullptr);
    ~DeviceControlWidget();

    void populateDeviceList();
    void updateSelectionCombo();
    int getCurrentDeviceIndex() const;
    QString getCurrentDeviceName() const;
    QString getCurrentSelectionName() const;
    
    // Accessors for AssignmentsWidget
    int getSelectionComboIndex() const;
    QRadioButton* getZoneRadioButton() const;
    QRadioButton* getLEDRadioButton() const;
    QRadioButton* getWholeDeviceRadioButton() const;

signals:
    void deviceSelectionChanged(int index);
    void selectionTypeChanged();

private slots:
    void onDeviceComboChanged(int index);
    void onSelectionTypeChanged();

private:
    Ui::DeviceControlWidget *ui;
    DeviceManager *deviceManager;
};