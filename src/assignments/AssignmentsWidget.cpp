/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| AssignmentsWidget.cpp                                     |
|                                                           |
| Implementation of device assignments management UI        |
\*---------------------------------------------------------*/

#include "assignments/AssignmentsWidget.h"
#include "ui_AssignmentsWidget.h"
#include "devices/DeviceControlWidget.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QDebug>

AssignmentsWidget::AssignmentsWidget(DeviceManager* deviceManager, 
                                   SpatialGrid* spatialGrid, 
                                   QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AssignmentsWidget)
    , deviceManager(deviceManager)
    , spatialGrid(spatialGrid)
    , deviceControlWidget(nullptr)
{
    printf("[Lightscape][AssignmentsWidget] Creating assignments widget.\n");
    
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    ui->assignmentList->setMinimumHeight(100);

    connect(ui->assignButton, &QPushButton::clicked, this, &AssignmentsWidget::onAssignButtonClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &AssignmentsWidget::onRemoveButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &AssignmentsWidget::onClearButtonClicked);
    connect(ui->colorButton, &QPushButton::clicked, this, &AssignmentsWidget::onColorButtonClicked);

    if (deviceManager) {
        connect(deviceManager, &DeviceManager::deviceAssignmentChanged,
                this, &AssignmentsWidget::onDeviceAssignmentChanged);
    }

    if (layout()) {
        layout()->setSizeConstraint(QLayout::SetMinimumSize);
    }
}

AssignmentsWidget::~AssignmentsWidget()
{
    printf("[Lightscape][AssignmentsWidget] Destroying assignments widget.\n");
    delete ui;
}

void AssignmentsWidget::onColorButtonClicked()
{
    if (!spatialGrid || !spatialGrid->GetSelectedButton()) {
        QMessageBox::warning(this, "Error", "Please select a grid position first.");
        return;
    }

    QListWidgetItem* currentItem = ui->assignmentList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Error", "Please select an assignment to change its color.");
        return;
    }

    QColor current_color = currentItem->background().color();
    QColor color = QColorDialog::getColor(current_color, this, "Select Color");
    
    if (color.isValid()) {
        // Update UI
        currentItem->setBackground(color);

        // Convert QColor to RGBColor (OpenRGB format)
        // OpenRGB format uses 0x00RRGGBB format
        RGBColor rgbColor = ToRGBColor(color.red(), color.green(), color.blue());

        printf("[Lightscape][AssignmentsWidget] Setting color: R=%d, G=%d, B=%d (0x%06X)\n", 
               color.red(), color.green(), color.blue(), rgbColor);

        // Update color in spatial grid
        GridPosition pos = spatialGrid->GetSelectedPosition();
        int index = ui->assignmentList->row(currentItem);
        if (index >= 0) {
            // Get the assignment to find device details
            const auto& assignments = spatialGrid->GetAssignments(pos);
            if (index < assignments.size()) {
                const auto& assignment = assignments[index];
                
                if (assignment.device_type == Lightscape::DeviceType::RGB) {
                    bool success = false;
                    
                    // Handle RGB device color updates through device manager
                    if (assignment.zone_index >= 0) {
                        success = deviceManager->SetZoneColor(assignment.device_index, assignment.zone_index, rgbColor);
                    }
                    else if (assignment.led_index >= 0) {
                        success = deviceManager->SetLEDColor(assignment.device_index, assignment.led_index, rgbColor);
                    }
                    else {
                        success = deviceManager->SetDeviceColor(assignment.device_index, rgbColor);
                    }
                    
                    if (success) {
                        deviceManager->UpdateDevice(assignment.device_index);
                        spatialGrid->UpdateAssignmentColor(pos, index, rgbColor);
                        emit colorChangeRequested(rgbColor);
                    }
                }
            }
        }
    }
}

void AssignmentsWidget::onAssignButtonClicked()
{
    if (!deviceManager || !spatialGrid || !deviceControlWidget) {
        QMessageBox::warning(this, "Error", "Device manager, spatial grid, or device control not initialized.");
        return;
    }

    if (!spatialGrid->GetSelectedButton()) {
        QMessageBox::warning(this, "Error", "Please select a position first.");
        return;
    }

    DeviceControlWidget* control = qobject_cast<DeviceControlWidget*>(deviceControlWidget);
    if (!control) {
        QMessageBox::warning(this, "Error", "Invalid device control widget.");
        return;
    }

    int deviceIndex = control->getCurrentDeviceIndex();
    if (deviceIndex < 0) {
        QMessageBox::warning(this, "Error", "No device selected.");
        return;
    }

    Lightscape::DeviceType deviceType = control->getCurrentDeviceType();
    QString selectionName = control->getCurrentSelectionName();
    GridPosition pos = spatialGrid->GetSelectedPosition();

    printf("[Lightscape][AssignmentsWidget] Assigning device - Index: %d, Type: %s\n", 
           deviceIndex, (deviceType == Lightscape::DeviceType::RGB ? "RGB" : "Non-RGB"));

    // Create assignment
    DeviceAssignment assignment(deviceIndex, deviceType);
    
    // Set zone or LED index if it's an RGB device
    if (deviceType == Lightscape::DeviceType::RGB) {
        if (control->getZoneRadioButton()->isChecked()) {
            assignment.zone_index = control->getSelectionComboIndex();
            assignment.led_index = -1;
        } else if (control->getLEDRadioButton()->isChecked()) {
            assignment.zone_index = -1;
            assignment.led_index = control->getSelectionComboIndex();
        } else {
            assignment.zone_index = -1;
            assignment.led_index = -1;
        }
    }

    // Add to grid
    spatialGrid->AddAssignment(pos, assignment);

    // Update device manager
    QString layerLabel = spatialGrid->GetLayerLabel(pos.z);
    deviceManager->SetDeviceAssignment(deviceIndex, deviceType, layerLabel, pos);

    emit assignRequested(deviceIndex, deviceType, selectionName, pos);
    updateAssignmentsList();
}

void AssignmentsWidget::onRemoveButtonClicked()
{
    printf("[Lightscape][AssignmentsWidget] Remove button clicked\n");
    
    QListWidgetItem* currentItem = ui->assignmentList->currentItem();
    if (!currentItem) {
        printf("[Lightscape][AssignmentsWidget] No item selected\n");
        return;
    }

    if (!spatialGrid || !spatialGrid->GetSelectedButton()) {
        printf("[Lightscape][AssignmentsWidget] No grid position selected\n");
        return;
    }

    GridPosition pos = spatialGrid->GetSelectedPosition();
    const auto& assignments = spatialGrid->GetAssignments(pos);
    int index = ui->assignmentList->row(currentItem);
    
    printf("[Lightscape][AssignmentsWidget] Removing assignment at index: %d out of %d assignments\n", 
           index, static_cast<int>(assignments.size()));
    
    if (index >= 0 && index < assignments.size()) {
        const auto& assignment = assignments[index];
        printf("[Lightscape][AssignmentsWidget] Removing device: %s\n", 
               deviceManager->GetDeviceName(assignment.device_index, assignment.device_type).toStdString().c_str());
        
        // First remove from grid
        spatialGrid->RemoveAssignment(pos, index);

        // Then remove from device manager
        deviceManager->RemoveDeviceAssignment(assignment.device_index, assignment.device_type);

        // Finally remove the UI item
        delete ui->assignmentList->takeItem(index);

        emit removeRequested(currentItem);
        
        printf("[Lightscape][AssignmentsWidget] Device removed successfully\n");
    } else {
        printf("[Lightscape][AssignmentsWidget] Invalid index for removal\n");
    }
}

void AssignmentsWidget::onClearButtonClicked()
{
    if (!spatialGrid || !spatialGrid->GetSelectedButton())
        return;

    if (QMessageBox::question(this, "Clear Assignments", 
                            "Are you sure you want to clear all assignments?", 
                            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        GridPosition pos = spatialGrid->GetSelectedPosition();
        const auto& assignments = spatialGrid->GetAssignments(pos);
        for (const auto& assignment : assignments) {
            deviceManager->RemoveDeviceAssignment(assignment.device_index, assignment.device_type);
        }
        spatialGrid->ClearAssignments(pos);
        clearAssignments();
    }
}

void AssignmentsWidget::onDeviceAssignmentChanged(unsigned int index,
                                                 Lightscape::DeviceType type,
                                                 bool isAssigned)
{
    Q_UNUSED(index);
    Q_UNUSED(type);
    Q_UNUSED(isAssigned);
    updateAssignmentsList();
}

void AssignmentsWidget::updateAssignmentsList()
{
    ui->assignmentList->clear();
    
    if (!deviceManager || !spatialGrid || !spatialGrid->GetSelectedButton())
        return;

    const auto& assignments = spatialGrid->GetAssignments(spatialGrid->GetSelectedPosition());
    for (const auto& assignment : assignments)
    {
        QString text = formatDeviceInfo(assignment.device_index,
                                      assignment.device_type,
                                      assignment.zone_index >= 0 ? 
                                          deviceManager->GetZoneName(assignment.device_index, assignment.zone_index) :
                                          assignment.led_index >= 0 ?
                                              deviceManager->GetLEDName(assignment.device_index, assignment.led_index) :
                                              "Whole Device");

        QListWidgetItem* item = new QListWidgetItem(text);
        item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        
        unsigned int color = assignment.color;
        
        // Extract RGB values correctly from OpenRGB's format (0x00RRGGBB)
        // Get the individual components
        int red = RGBGetRValue(color);
        int green = RGBGetGValue(color);
        int blue = RGBGetBValue(color);
        
        // Create QColor with the correct RGB values
        QColor qcolor(red, green, blue);
        
        printf("[Lightscape][AssignmentsWidget] Setting list item color: R=%d, G=%d, B=%d (0x%06X)\n", 
               red, green, blue, color);
        
        item->setBackground(qcolor);
        ui->assignmentList->addItem(item);
    }
}

void AssignmentsWidget::clearAssignments()
{
    ui->assignmentList->clear();
    if (spatialGrid && spatialGrid->GetSelectedButton()) {
        spatialGrid->ClearAssignments(spatialGrid->GetSelectedPosition());
    }
}

QString AssignmentsWidget::formatDeviceInfo(const unsigned int index,
                                          const Lightscape::DeviceType type,
                                          const QString& selectionName) const
{
    QString deviceName = deviceManager->GetDeviceName(index, type);
    QString deviceTypeStr = type == Lightscape::DeviceType::RGB ? "RGB" : "Non-RGB";
    QString deviceInfo = QString("%1 (%2)").arg(deviceName).arg(deviceTypeStr);

    QString details = getAssignmentDetails(index, type);
    if (!details.isEmpty()) {
        deviceInfo += QString("\n%1").arg(details);
    }

    if (type == Lightscape::DeviceType::RGB && !selectionName.isEmpty()) {
        deviceInfo += QString("\nSelection: %1").arg(selectionName);
    }

    return deviceInfo;
}

QString AssignmentsWidget::getDeviceTypeString(const Lightscape::DeviceType type) const
{
    return type == Lightscape::DeviceType::RGB ? "RGB" : "Non-RGB";
}

QString AssignmentsWidget::getAssignmentDetails(const unsigned int index,
                                              const Lightscape::DeviceType type) const
{
    QString layer;
    GridPosition position;
    if (deviceManager->GetDeviceAssignmentInfo(index, type, layer, position)) {
        // Get position label from grid
        QString posLabel = spatialGrid->GetPositionLabel(position);
        if (posLabel.isEmpty()) {
            // Calculate default position label (P1, P2, etc.)
            int gridWidth = spatialGrid->GetDimensions().width;
            int posNumber = position.y * gridWidth + position.x + 1;
            posLabel = QString("P%1").arg(posNumber);
        }
        
        return QString("Layer: %1, Position: %2")
                .arg(layer)
                .arg(posLabel);
    }
    return QString();
}

QSize AssignmentsWidget::calculateNeededSize() const
{
    int minWidth = 250;
    int minHeight = 0;

    if (layout()) {
        QSize layoutSize = layout()->minimumSize();
        minWidth = std::max(minWidth, layoutSize.width());
        minHeight = std::max(minHeight, layoutSize.height());
    }

    QMargins margins = contentsMargins();
    minWidth += margins.left() + margins.right();
    minHeight += margins.top() + margins.bottom();
    minHeight = std::max(minHeight, 200);

    return QSize(minWidth, minHeight);
}

QSize AssignmentsWidget::sizeHint() const
{
    return calculateNeededSize();
}

QSize AssignmentsWidget::minimumSizeHint() const
{
    return calculateNeededSize();
}

void AssignmentsWidget::resizeEvent(QResizeEvent* event)
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