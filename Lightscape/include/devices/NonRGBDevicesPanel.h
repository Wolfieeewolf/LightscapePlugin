#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "devices/NonRGBDeviceManager.h"
#include "grid/NonRGBGridManager.h"
#include "devices/NonRGBDeviceWidget.h"

class NonRGBDevicesPanel : public QWidget
{
    Q_OBJECT

public:
    explicit NonRGBDevicesPanel(NonRGBDeviceManager* deviceManager, 
                               NonRGBGridManager* gridManager,
                               QWidget* parent = nullptr);

private slots:
    void onAddDeviceClicked();
    void onDeviceModified(NonRGBDevice* device);
    void onDeviceRemoveRequested(const QString& deviceName);
    void onDeviceListChanged();

private:
    NonRGBDeviceManager* deviceManager;
    NonRGBGridManager* gridManager;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* devicesLayout;

    void setupUI();
    void updateDevicesList();
    void addDeviceWidget(NonRGBDevice* device);
    QString generateUniqueName(const QString& baseName) const;
};