#include "assignments/AssignmentsWidget.h"
#include "ui_AssignmentsWidget.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QVBoxLayout>

AssignmentsWidget::AssignmentsWidget(DeviceManager* deviceManager, 
                                   SpatialGrid* spatialGrid, 
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssignmentsWidget),
    deviceManager(deviceManager),
    spatialGrid(spatialGrid),
    deviceControlWidget(nullptr)
{
    ui->setupUi(this);

    // Set size policy
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Set minimum sizes for internal widgets
    ui->assignmentList->setMinimumHeight(100);  // Ensure list has space for at least a few items

    connect(ui->assignButton, &QPushButton::clicked, 
            this, &AssignmentsWidget::onAssignButtonClicked);
    connect(ui->removeButton, &QPushButton::clicked, 
            this, &AssignmentsWidget::onRemoveButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, 
            this, &AssignmentsWidget::onClearButtonClicked);
    connect(ui->colorButton, &QPushButton::clicked, 
            this, &AssignmentsWidget::onColorButtonClicked);

    // Set layout size constraint
    if (layout()) {
        layout()->setSizeConstraint(QLayout::SetMinimumSize);
    }
}

AssignmentsWidget::~AssignmentsWidget()
{
    delete ui;
}

QSize AssignmentsWidget::calculateNeededSize() const
{
    int minWidth = 250;  // Base minimum width
    int minHeight = 0;

    if (layout()) {
        // Get layout's minimum size
        QSize layoutSize = layout()->minimumSize();
        minWidth = std::max(minWidth, layoutSize.width());
        minHeight = std::max(minHeight, layoutSize.height());
    }

    // Add margins
    QMargins margins = contentsMargins();
    minWidth += margins.left() + margins.right();
    minHeight += margins.top() + margins.bottom();

    // Ensure space for list
    minHeight = std::max(minHeight, 200);  // Minimum height to show a few assignments

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
    
    // If new size is smaller than needed, resize back to minimum
    QSize needed = calculateNeededSize();
    if (event->size().width() < needed.width() || 
        event->size().height() < needed.height()) {
        resize(
            std::max(event->size().width(), needed.width()),
            std::max(event->size().height(), needed.height())
        );
    }
}

void AssignmentsWidget::onAssignButtonClicked()
{
    if (!deviceManager || !spatialGrid)
    {
        QMessageBox::warning(this, "Error", "Device manager or spatial grid not initialized.");
        return;
    }

    if (!spatialGrid->GetSelectedButton())
    {
        QMessageBox::warning(this, "Error", "Please select a position first.");
        return;
    }

    DeviceAssignment assignment;
    assignment.device_index = deviceManager->GetCurrentDeviceIndex();
    assignment.color = 0x000000; // Black by default

    QString selectionName = deviceManager->GetCurrentSelectionName();
    if (selectionName == "Entire Device")
    {
        assignment.zone_index = -1;
        assignment.led_index = -1;
    }
    else
    {
        bool isZone = selectionName.contains("Zone");
        if (isZone)
        {
            assignment.zone_index = deviceManager->GetCurrentDeviceIndex();
            assignment.led_index = -1;
        }
        else
        {
            assignment.zone_index = -1;
            assignment.led_index = deviceManager->GetCurrentDeviceIndex();
        }
    }

    spatialGrid->AddAssignment(spatialGrid->GetSelectedPosition(), assignment);
    updateAssignmentsList();
}

void AssignmentsWidget::onRemoveButtonClicked()
{
    QListWidgetItem* currentItem = ui->assignmentList->currentItem();
    if (currentItem)
    {
        int index = ui->assignmentList->row(currentItem);
        spatialGrid->RemoveAssignment(spatialGrid->GetSelectedPosition(), index);
        delete currentItem;
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
        spatialGrid->ClearAssignments(spatialGrid->GetSelectedPosition());
        ui->assignmentList->clear();
    }
}

void AssignmentsWidget::onColorButtonClicked()
{
    if (!spatialGrid || !spatialGrid->GetSelectedButton())
    {
        QMessageBox::warning(this, "Error", "Please select a grid position first.");
        return;
    }

    QListWidgetItem* currentItem = ui->assignmentList->currentItem();
    if (!currentItem)
    {
        QMessageBox::warning(this, "Error", "Please select an assignment to change its color.");
        return;
    }

    QColor current_color = currentItem->background().color();
    QColor color = QColorDialog::getColor(current_color, this, "Select Color");
    
    if (color.isValid())
    {
        // Convert QColor to RGBColor
        unsigned int rgb_color = ((color.blue() & 0xFF) << 16) | 
                                ((color.green() & 0xFF) << 8) | 
                                color.red();
        
        int index = ui->assignmentList->row(currentItem);
        
        if (spatialGrid->UpdateAssignmentColor(spatialGrid->GetSelectedPosition(), index, rgb_color))
        {
            currentItem->setBackground(color);
            emit colorChangeRequested(color);
        }
    }
}

void AssignmentsWidget::updateAssignmentsList()
{
    ui->assignmentList->clear();
    
    if (!deviceManager || !spatialGrid || !spatialGrid->GetSelectedButton())
        return;

    const auto& assignments = spatialGrid->GetAssignments(spatialGrid->GetSelectedPosition());
    for (const auto& assignment : assignments)
    {
        QString text = deviceManager->GetDeviceName(assignment.device_index) + ": ";
        
        if (assignment.zone_index >= 0)
        {
            text += deviceManager->GetZoneName(assignment.device_index, assignment.zone_index);
        }
        else if (assignment.led_index >= 0)
        {
            text += deviceManager->GetLEDName(assignment.device_index, assignment.led_index);
        }
        else
        {
            text += "Whole Device";
        }

        QListWidgetItem* item = new QListWidgetItem(text);
        
        // Convert OpenRGB color to QColor
        unsigned int color = assignment.color;
        QColor qcolor((color >> 16) & 0xFF,    // Red
                     (color >> 8) & 0xFF,     // Green
                     color & 0xFF);           // Blue
        
        item->setBackground(qcolor);
        ui->assignmentList->addItem(item);
    }
}

void AssignmentsWidget::clearAssignments()
{
    ui->assignmentList->clear();
}