#include "devices/DeviceAssignmentWidget.h"
#include "ui_DeviceAssignmentWidget.h"

DeviceAssignmentWidget::DeviceAssignmentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceAssignmentWidget)
{
    ui->setupUi(this);

    populateDeviceComboBox();
    setupAssignmentTable();

    connect(ui->assignButton, &QPushButton::clicked, this, &DeviceAssignmentWidget::onAssignButtonClicked);
    connect(ui->deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &DeviceAssignmentWidget::onDeviceSelectionChanged);
}

DeviceAssignmentWidget::~DeviceAssignmentWidget()
{
    delete ui;
}

void DeviceAssignmentWidget::populateDeviceComboBox()
{
    // TODO: Implement device population logic
    ui->deviceComboBox->clear();
    // Example placeholder
    ui->deviceComboBox->addItem("Device 1");
    ui->deviceComboBox->addItem("Device 2");
}

void DeviceAssignmentWidget::setupAssignmentTable()
{
    ui->assignmentTable->setColumnCount(3);
    ui->assignmentTable->setHorizontalHeaderLabels({"Device", "Grid Zone", "Status"});
}

void DeviceAssignmentWidget::onAssignButtonClicked()
{
    // TODO: Implement device assignment logic
}

void DeviceAssignmentWidget::onDeviceSelectionChanged(int index)
{
    // TODO: Update assignment table based on selected device
}