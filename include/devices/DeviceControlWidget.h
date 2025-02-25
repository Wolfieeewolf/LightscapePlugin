#pragma once

#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QMap>
#include "devices/NonRGBDeviceManager.h"
#include "core/DeviceManager.h"

namespace Ui {
class DeviceControlWidget;
}

class DeviceControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceControlWidget(DeviceManager* deviceManager = nullptr, 
                               QWidget *parent = nullptr);
    ~DeviceControlWidget();

    // Current selections
    int getCurrentDeviceIndex() const;
    QString getCurrentDeviceName() const;
    QString getCurrentSelectionName() const;
    Lightscape::DeviceType getCurrentDeviceType() const;
    int getSelectionComboIndex() const;

    // UI element access
    QRadioButton* getWholeDeviceRadioButton() const;
    QRadioButton* getZoneRadioButton() const;
    QRadioButton* getLEDRadioButton() const;

    // Size management
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void deviceSelectionChanged(int deviceIndex, Lightscape::DeviceType type);
    void selectionTypeChanged();
    void customDeviceAdded();

public slots:
    void populateDeviceList();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onDeviceTypeChanged(int index);
    void onDeviceComboChanged(int index);
    void onSelectionTypeChanged();
    void onAddCustomDeviceClicked();
    void onDeviceAssignmentChanged(unsigned int index, 
                                 Lightscape::DeviceType type,
                                 bool isAssigned);

private:
    void setupCustomDeviceButton();
    void updateRGBControls(bool visible);
    void updateSelectionCombo();
    void updateAddCustomButtonVisibility();
    QSize calculateNeededSize() const;

    Ui::DeviceControlWidget *ui;
    DeviceManager *deviceManager;
    QPushButton *addCustomButton;
    QMap<int, int> deviceIndices;  // Maps combo index to actual device index
};