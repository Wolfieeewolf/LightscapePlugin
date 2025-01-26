#include "devices/DeviceControlWidget.h"
#include "ui_DeviceControlWidget.h"

DeviceControlWidget::DeviceControlWidget(DeviceManager* deviceManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceControlWidget),
    deviceManager(deviceManager)
{
    ui->setupUi(this);

    connect(ui->deviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &DeviceControlWidget::onDeviceComboChanged);
    
    connect(ui->wholeDeviceRadio, &QRadioButton::clicked, 
            this, &DeviceControlWidget::onSelectionTypeChanged);
    connect(ui->zoneRadio, &QRadioButton::clicked, 
            this, &DeviceControlWidget::onSelectionTypeChanged);
    connect(ui->ledRadio, &QRadioButton::clicked, 
            this, &DeviceControlWidget::onSelectionTypeChanged);

    ui->wholeDeviceRadio->setChecked(true);
    populateDeviceList();
}

DeviceControlWidget::~DeviceControlWidget()
{
    delete ui;
}

void DeviceControlWidget::populateDeviceList()
{
    if (!deviceManager) return;
    
    ui->deviceCombo->clear();
    for (unsigned int i = 0; i < deviceManager->GetDeviceCount(); i++)
    {
        ui->deviceCombo->addItem(deviceManager->GetDeviceName(i));
    }
    
    if (ui->deviceCombo->count() > 0) {
        ui->deviceCombo->setCurrentIndex(0);
        deviceManager->SetCurrentDeviceIndex(0);  // Set initial device
    }
}

void DeviceControlWidget::updateSelectionCombo()
{
    if (!deviceManager) return;
    
    ui->selectionCombo->clear();
    int deviceIndex = ui->deviceCombo->currentIndex();
    if (deviceIndex < 0) return;
    
    if (ui->wholeDeviceRadio->isChecked())
    {
        ui->selectionCombo->addItem("Entire Device");
    }
    else if (ui->zoneRadio->isChecked())
    {
        size_t zoneCount = deviceManager->GetZoneCount(deviceIndex);
        for (size_t i = 0; i < zoneCount && i <= INT_MAX; i++)
        {
            ui->selectionCombo->addItem(deviceManager->GetZoneName(deviceIndex, static_cast<int>(i)));
        }
    }
    else if (ui->ledRadio->isChecked())
    {
        size_t ledCount = deviceManager->GetLEDCount(deviceIndex);
        for (size_t i = 0; i < ledCount && i <= INT_MAX; i++)
        {
            ui->selectionCombo->addItem(deviceManager->GetLEDName(deviceIndex, static_cast<int>(i)));
        }
    }
    
    if (ui->selectionCombo->count() > 0) {
        ui->selectionCombo->setCurrentIndex(0);
        deviceManager->SetCurrentSelectionName(ui->selectionCombo->currentText());  // Set initial selection
    }
}

void DeviceControlWidget::onDeviceComboChanged(int index)
{
    if (deviceManager) {
        deviceManager->SetCurrentDeviceIndex(index);  // Update device manager
    }
    emit deviceSelectionChanged(index);
    updateSelectionCombo();
}

void DeviceControlWidget::onSelectionTypeChanged()
{
    updateSelectionCombo();
    emit selectionTypeChanged();
}

int DeviceControlWidget::getCurrentDeviceIndex() const
{
    return ui->deviceCombo->currentIndex();
}

QString DeviceControlWidget::getCurrentDeviceName() const
{
    return ui->deviceCombo->currentText();
}

QString DeviceControlWidget::getCurrentSelectionName() const
{
    return ui->selectionCombo->currentText();
}

int DeviceControlWidget::getSelectionComboIndex() const
{
    return ui->selectionCombo->currentIndex();
}

QRadioButton* DeviceControlWidget::getZoneRadioButton() const
{
    return ui->zoneRadio;
}

QRadioButton* DeviceControlWidget::getLEDRadioButton() const
{
    return ui->ledRadio;
}

QRadioButton* DeviceControlWidget::getWholeDeviceRadioButton() const
{
    return ui->wholeDeviceRadio;
}