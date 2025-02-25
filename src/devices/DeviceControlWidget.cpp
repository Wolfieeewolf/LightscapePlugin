#include "devices/DeviceControlWidget.h"
#include "devices/CustomDeviceDialog.h"
#include "ui_DeviceControlWidget.h"
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QDebug>

DeviceControlWidget::DeviceControlWidget(DeviceManager* deviceManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DeviceControlWidget)
    , deviceManager(deviceManager)
    , addCustomButton(nullptr)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setupCustomDeviceButton();

    connect(ui->deviceTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DeviceControlWidget::onDeviceTypeChanged);
    connect(ui->deviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DeviceControlWidget::onDeviceComboChanged);
    
    connect(ui->wholeDeviceRadio, &QRadioButton::toggled,
            this, &DeviceControlWidget::onSelectionTypeChanged);
    connect(ui->zoneRadio, &QRadioButton::toggled,
            this, &DeviceControlWidget::onSelectionTypeChanged);
    connect(ui->ledRadio, &QRadioButton::toggled,
            this, &DeviceControlWidget::onSelectionTypeChanged);

    if (deviceManager) {
        connect(deviceManager, &DeviceManager::deviceListChanged,
                this, &DeviceControlWidget::populateDeviceList);
        connect(deviceManager, &DeviceManager::deviceAssignmentChanged,
                this, &DeviceControlWidget::onDeviceAssignmentChanged);
    }

    populateDeviceList();
    updateAddCustomButtonVisibility();
}

DeviceControlWidget::~DeviceControlWidget()
{
    delete ui;
}

void DeviceControlWidget::setupCustomDeviceButton()
{
    addCustomButton = new QPushButton("+ Add Custom Device", this);
    addCustomButton->setVisible(false);
    
    QVBoxLayout* deviceLayout = qobject_cast<QVBoxLayout*>(ui->deviceLayout);
    if (deviceLayout) {
        int index = deviceLayout->indexOf(ui->deviceCombo);
        deviceLayout->insertWidget(index + 1, addCustomButton);
    }

    connect(addCustomButton, &QPushButton::clicked, 
            this, &DeviceControlWidget::onAddCustomDeviceClicked);
}

void DeviceControlWidget::onDeviceTypeChanged(int index)
{
    auto type = index == 0 ? Lightscape::DeviceType::RGB : Lightscape::DeviceType::NonRGB;
    updateRGBControls(type == Lightscape::DeviceType::RGB);
    updateAddCustomButtonVisibility();
    populateDeviceList();
}

void DeviceControlWidget::populateDeviceList()
{
    if (!deviceManager || !deviceManager->resourceManager) return;

    qDebug() << "Populating device list...";

    ui->deviceCombo->clear();
    deviceIndices.clear();
    
    auto type = ui->deviceTypeCombo->currentIndex() == 0 ? 
                Lightscape::DeviceType::RGB : 
                Lightscape::DeviceType::NonRGB;

    // Get the full list of controllers
    auto controllers = deviceManager->resourceManager->GetRGBControllers();
    
    // First, map all device indices to their names
    QMap<int, QString> allDevices;
    for (unsigned int i = 0; i < controllers.size(); i++) {
        if (controllers[i]) {
            QString name = QString::fromStdString(controllers[i]->name);
            allDevices[static_cast<int>(i)] = name;
            qDebug() << "Found device:" << name << "at index:" << i;
        }
    }

    // Now get unassigned devices
    QList<unsigned int> unassignedDevices = deviceManager->GetUnassignedDevices(type);
    qDebug() << "Unassigned devices:" << unassignedDevices;

    // Add only unassigned devices to combo box while preserving their original indices
    for (int deviceIndex : unassignedDevices) {
        if (allDevices.contains(deviceIndex)) {
            QString name = allDevices[deviceIndex];
            int comboIndex = ui->deviceCombo->count();
            ui->deviceCombo->addItem(name);
            deviceIndices[comboIndex] = deviceIndex;
            qDebug() << "Added device to combo:" << name << "combo index:" << comboIndex << "device index:" << deviceIndex;
        }
    }

    updateSelectionCombo();
}

void DeviceControlWidget::updateRGBControls(bool visible)
{
    ui->rgbControls->setVisible(visible);
    if (!visible) {
        ui->wholeDeviceRadio->setChecked(true);
    }
}

void DeviceControlWidget::updateAddCustomButtonVisibility()
{
    bool isNonRGBSelected = getCurrentDeviceType() == Lightscape::DeviceType::NonRGB;
    addCustomButton->setVisible(isNonRGBSelected);
}

void DeviceControlWidget::onDeviceComboChanged(int index)
{
    if (!deviceManager || index < 0) return;

    auto type = ui->deviceTypeCombo->currentIndex() == 0 ? 
                Lightscape::DeviceType::RGB : 
                Lightscape::DeviceType::NonRGB;

    int deviceIndex = deviceIndices.value(index, -1);
    if (deviceIndex >= 0) {
        deviceManager->SetCurrentDeviceIndex(deviceIndex);
        deviceManager->SetCurrentDeviceType(type);
        updateSelectionCombo();
        
        emit deviceSelectionChanged(deviceIndex, type);
        
        qDebug() << "Device selected - combo index:" << index 
                 << "device index:" << deviceIndex 
                 << "name:" << deviceManager->GetDeviceName(deviceIndex, type);
    }
}

void DeviceControlWidget::onSelectionTypeChanged()
{
    updateSelectionCombo();
    emit selectionTypeChanged();
}

void DeviceControlWidget::onAddCustomDeviceClicked()
{
    CustomDeviceDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString name = dialog.getDeviceName();
        QString typeStr = dialog.getDeviceType();
        
        NonRGBDeviceType type = NonRGBDeviceType::Custom;
        if (typeStr == "Monitor") type = NonRGBDeviceType::Monitor;
        else if (typeStr == "Speaker") type = NonRGBDeviceType::Speaker;
        else if (typeStr == "Desk") type = NonRGBDeviceType::Desk;
        else if (typeStr == "PC Case") type = NonRGBDeviceType::PCCase;
        
        auto device = new NonRGBDevice(name, type);
        deviceManager->AddNonRGBDevice(device);
        populateDeviceList();
    }
}

void DeviceControlWidget::updateSelectionCombo()
{
    ui->selectionCombo->clear();

    if (!deviceManager || ui->deviceCombo->currentIndex() < 0) return;

    auto type = ui->deviceTypeCombo->currentIndex() == 0 ? 
                Lightscape::DeviceType::RGB : 
                Lightscape::DeviceType::NonRGB;

    if (type == Lightscape::DeviceType::RGB) {
        int deviceIndex = getCurrentDeviceIndex();
        
        if (deviceIndex >= 0) {
            if (ui->wholeDeviceRadio->isChecked()) {
                ui->selectionCombo->addItem("Entire Device");
            }
            else if (ui->zoneRadio->isChecked()) {
                size_t zoneCount = deviceManager->GetZoneCount(deviceIndex);
                for (size_t i = 0; i < zoneCount; i++) {
                    ui->selectionCombo->addItem(deviceManager->GetZoneName(deviceIndex, static_cast<int>(i)));
                }
            }
            else if (ui->ledRadio->isChecked()) {
                size_t ledCount = deviceManager->GetLEDCount(deviceIndex);
                for (size_t i = 0; i < ledCount; i++) {
                    ui->selectionCombo->addItem(deviceManager->GetLEDName(deviceIndex, static_cast<int>(i)));
                }
            }
        }
    }
}

void DeviceControlWidget::onDeviceAssignmentChanged(unsigned int index, 
                                                   Lightscape::DeviceType type,
                                                   bool isAssigned)
{
    qDebug() << "Device assignment changed - index:" << index << "type:" << (type == Lightscape::DeviceType::RGB ? "RGB" : "NonRGB")
             << "assigned:" << isAssigned;
    populateDeviceList();
}

int DeviceControlWidget::getCurrentDeviceIndex() const
{
    int comboIndex = ui->deviceCombo->currentIndex();
    return deviceIndices.value(comboIndex, -1);
}

QString DeviceControlWidget::getCurrentDeviceName() const
{
    int deviceIndex = getCurrentDeviceIndex();
    if (deviceIndex >= 0) {
        return deviceManager->GetDeviceName(deviceIndex, getCurrentDeviceType());
    }
    return QString();
}

QString DeviceControlWidget::getCurrentSelectionName() const
{
    return ui->selectionCombo->currentText();
}

Lightscape::DeviceType DeviceControlWidget::getCurrentDeviceType() const
{
    return ui->deviceTypeCombo->currentIndex() == 0 ? 
           Lightscape::DeviceType::RGB : 
           Lightscape::DeviceType::NonRGB;
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

QSize DeviceControlWidget::calculateNeededSize() const
{
    int minWidth = 200;
    int minHeight = 0;

    if (layout()) {
        QSize layoutSize = layout()->minimumSize();
        minWidth = std::max(minWidth, layoutSize.width());
        minHeight = std::max(minHeight, layoutSize.height());
    }

    QMargins margins = contentsMargins();
    minWidth += margins.left() + margins.right();
    minHeight += margins.top() + margins.bottom();

    return QSize(minWidth, minHeight);
}

QSize DeviceControlWidget::sizeHint() const
{
    return calculateNeededSize();
}

QSize DeviceControlWidget::minimumSizeHint() const
{
    return calculateNeededSize();
}

void DeviceControlWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    
    QSize needed = calculateNeededSize();
    if (event->size().width() < needed.width() || 
        event->size().height() < needed.height()) {
        resize(
            std::max(event->size().width(), needed.width()),
            std::max(event->size().height(), needed.height())
        );
    }
}