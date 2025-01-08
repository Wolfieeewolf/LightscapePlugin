#include "../include/LightscapeWidget.h"
#include "../include/GridSettingsDialog.h"
#include "ui_LightscapeWidget.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QtGlobal>

LightscapeWidget::LightscapeWidget(ResourceManager* resource_manager, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LightscapeWidget)
    , device_manager(resource_manager)
    , spatial_grid(this)
{
    ui->setupUi(this);
    setupConnections();
    setupInitialState();
}

LightscapeWidget::~LightscapeWidget()
{
    delete ui;
}

void LightscapeWidget::setupInitialState()
{
    // Set up initial grid
    QVector<QGridLayout*> layouts = {
        ui->topGrid,
        ui->middleGrid,
        ui->bottomGrid
    };
    
    spatial_grid.SetupGrid(layouts);

    // Start update timer
    update_timer.start(100);

    // Populate device list
    populateDeviceList();

    // Initial UI state
    ui->removeButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->selectionCombo->setEnabled(false);
    updateSelectionCombo();
}

void LightscapeWidget::refreshGridDisplay()
{
    // First, check if we need to create or remove any layer tabs
    while (ui->layerTabs->count() > spatial_grid.GetDimensions().depth)
    {
        // Remove extra tabs
        ui->layerTabs->removeTab(ui->layerTabs->count() - 1);
    }
    
    while (ui->layerTabs->count() < spatial_grid.GetDimensions().depth)
    {
        // Add new tabs as needed
        QWidget* newPage = new QWidget();
        QGridLayout* newGrid = new QGridLayout(newPage);
        ui->layerTabs->addTab(newPage, spatial_grid.GetLayerLabel(ui->layerTabs->count()));
    }

    // Update all layer tab titles
    for (int i = 0; i < spatial_grid.GetDimensions().depth; i++)
    {
        QString layerTitle = spatial_grid.GetLayerLabel(i);
        ui->layerTabs->setTabText(i, layerTitle);
    }

    // Clear existing grid layouts and rebuild them
    QVector<QGridLayout*> layouts;
    for (int i = 0; i < spatial_grid.GetDimensions().depth; i++)
    {
        QWidget* page = ui->layerTabs->widget(i);
        QGridLayout* grid = qobject_cast<QGridLayout*>(page->layout());
        
        // Clear existing grid
        if (grid)
        {
            while (grid->count() > 0)
            {
                QLayoutItem* item = grid->takeAt(0);
                if (item->widget())
                {
                    item->widget()->deleteLater();
                }
                delete item;
            }
        }
        else
        {
            // Create new grid layout if needed
            grid = new QGridLayout(page);
        }
        
        layouts.append(grid);
    }
    
    spatial_grid.ClearGrids(); // Clear old buttons first
    spatial_grid.SetupGrid(layouts);
}

void LightscapeWidget::setupConnections()
{
    connect(&update_timer, &QTimer::timeout, 
            this, &LightscapeWidget::on_updateColors);

    connect(&spatial_grid, &SpatialGrid::positionSelected,
            this, &LightscapeWidget::on_gridPositionSelected);

    connect(ui->configureButton, &QPushButton::clicked,
            this, &LightscapeWidget::on_configureButton_clicked);

    connect(ui->deviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LightscapeWidget::on_deviceComboChanged);
            
    connect(ui->wholeDeviceRadio, &QRadioButton::toggled,
            this, &LightscapeWidget::on_assignmentTypeChanged);
            
    connect(ui->zoneRadio, &QRadioButton::toggled,
            this, &LightscapeWidget::on_assignmentTypeChanged);
            
    connect(ui->ledRadio, &QRadioButton::toggled,
            this, &LightscapeWidget::on_assignmentTypeChanged);
            
    connect(ui->assignButton, &QPushButton::clicked,
            this, &LightscapeWidget::on_assignButtonClicked);
            
    connect(ui->removeButton, &QPushButton::clicked,
            this, &LightscapeWidget::on_removeButtonClicked);
            
    connect(ui->clearButton, &QPushButton::clicked,
            this, &LightscapeWidget::on_clearButtonClicked);
            
    connect(ui->colorButton, &QPushButton::clicked,
            this, &LightscapeWidget::on_colorButtonClicked);
}

void LightscapeWidget::on_configureButton_clicked()
{
    GridSettingsDialog dialog(&spatial_grid, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        refreshGridDisplay();
    }
}

void LightscapeWidget::on_deviceComboChanged(int index)
{
    Q_UNUSED(index);
    updateSelectionCombo();
}

void LightscapeWidget::on_assignmentTypeChanged()
{
    updateSelectionCombo();
    ui->selectionCombo->setEnabled(!ui->wholeDeviceRadio->isChecked());
}

void LightscapeWidget::on_assignButtonClicked()
{
    if (!spatial_grid.GetSelectedButton())
    {
        QMessageBox::warning(this, "Error", "Please select a position first.");
        return;
    }

    DeviceAssignment assignment;
    assignment.device_index = ui->deviceCombo->currentData().toUInt();
    
    if (ui->wholeDeviceRadio->isChecked())
    {
        assignment.zone_index = -1;
        assignment.led_index = -1;
    }
    else if (ui->zoneRadio->isChecked())
    {
        assignment.zone_index = ui->selectionCombo->currentData().toInt();
        assignment.led_index = -1;
    }
    else if (ui->ledRadio->isChecked())
    {
        assignment.zone_index = -1;
        assignment.led_index = ui->selectionCombo->currentData().toInt();
    }

    spatial_grid.AddAssignment(spatial_grid.GetSelectedPosition(), assignment);
    updateAssignmentsList();
}

void LightscapeWidget::on_removeButtonClicked()
{
    if (!spatial_grid.GetSelectedButton())
        return;

    int current_row = ui->assignmentList->currentRow();
    if (current_row >= 0)
    {
        GridPosition pos = spatial_grid.GetSelectedPosition();
        spatial_grid.RemoveAssignment(pos, current_row);
        updateAssignmentsList();
        
        bool has_assignments = spatial_grid.HasAssignments(pos);
        ui->removeButton->setEnabled(has_assignments);
        ui->clearButton->setEnabled(has_assignments);
    }
}

void LightscapeWidget::on_clearButtonClicked()
{
    if (!spatial_grid.GetSelectedButton())
        return;

    GridPosition pos = spatial_grid.GetSelectedPosition();
    spatial_grid.ClearAssignments(pos);
    updateAssignmentsList();
    ui->removeButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
}

void LightscapeWidget::on_colorButtonClicked()
{
    if (!spatial_grid.GetSelectedButton() || ui->assignmentList->currentRow() < 0)
    {
        QMessageBox::warning(this, "Error", "Please select an assignment first.");
        return;
    }

    QColorDialog dialog(this);
    dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    
    if (dialog.exec() == QDialog::Accepted)
    {
        QColor color = dialog.selectedColor();
        GridPosition pos = spatial_grid.GetSelectedPosition();
        int current_row = ui->assignmentList->currentRow();
        
        QList<DeviceAssignment> assignments = spatial_grid.GetAssignments(pos);
        if (current_row < assignments.size())
        {
            DeviceAssignment assignment = assignments[current_row];
            assignment.color = ToRGBColor(color.red(), color.green(), color.blue());
            
            spatial_grid.RemoveAssignment(pos, current_row);
            spatial_grid.AddAssignment(pos, assignment);
            
            updateAssignmentsList();
        }
    }
}

void LightscapeWidget::on_gridPositionSelected(const GridPosition& pos)
{
    bool has_assignments = spatial_grid.HasAssignments(pos);
    ui->removeButton->setEnabled(has_assignments);
    ui->clearButton->setEnabled(has_assignments);
    updateAssignmentsList();
}

void LightscapeWidget::on_updateColors()
{
    for (int z = 0; z < spatial_grid.GetDimensions().depth; z++)
    {
        for (int y = 0; y < spatial_grid.GetDimensions().height; y++)
        {
            for (int x = 0; x < spatial_grid.GetDimensions().width; x++)
            {
                GridPosition pos(x, y, z);
                if (spatial_grid.HasAssignments(pos))
                {
                    for (const auto& assignment : spatial_grid.GetAssignments(pos))
                    {
                        if (assignment.zone_index >= 0)
                        {
                            device_manager.SetZoneColor(assignment.device_index, 
                                                      assignment.zone_index, 
                                                      assignment.color);
                        }
                        else if (assignment.led_index >= 0)
                        {
                            device_manager.SetLEDColor(assignment.device_index, 
                                                     assignment.led_index, 
                                                     assignment.color);
                        }
                        else
                        {
                            device_manager.SetDeviceColor(assignment.device_index, 
                                                        assignment.color);
                        }
                        device_manager.UpdateDevice(assignment.device_index);
                    }
                }
            }
        }
    }
}

void LightscapeWidget::populateDeviceList()
{
    ui->deviceCombo->clear();
    
    size_t count = device_manager.GetDeviceCount();
    for (size_t i = 0; i < count; i++)
    {
        ui->deviceCombo->addItem(
            device_manager.GetDeviceName(static_cast<unsigned int>(i)), 
            QVariant(static_cast<unsigned int>(i))
        );
    }
}

void LightscapeWidget::updateSelectionCombo()
{
    ui->selectionCombo->clear();
    
    if (ui->deviceCombo->currentIndex() < 0)
        return;
    
    unsigned int device_index = ui->deviceCombo->currentData().toUInt();
    
    if (ui->zoneRadio->isChecked())
    {
        size_t count = device_manager.GetZoneCount(device_index);
        for (size_t i = 0; i < count; i++)
        {
            ui->selectionCombo->addItem(
                device_manager.GetZoneName(device_index, static_cast<int>(i)),
                QVariant(static_cast<int>(i))
            );
        }
    }
    else if (ui->ledRadio->isChecked())
    {
        size_t count = device_manager.GetLEDCount(device_index);
        for (size_t i = 0; i < count; i++)
        {
            ui->selectionCombo->addItem(
                device_manager.GetLEDName(device_index, static_cast<int>(i)),
                QVariant(static_cast<int>(i))
            );
        }
    }
}

void LightscapeWidget::updateAssignmentsList()
{
    ui->assignmentList->clear();
    
    GridPosition pos = spatial_grid.GetSelectedPosition();
    if (!spatial_grid.GetSelectedButton() || !spatial_grid.HasAssignments(pos))
    {
        return;
    }
    
    QList<DeviceAssignment> assignments = spatial_grid.GetAssignments(pos);
    for (const auto& assignment : assignments)
    {
        QString deviceName = device_manager.GetDeviceName(assignment.device_index);
        QString text;
        
        if (assignment.zone_index >= 0)
        {
            QString zoneName = device_manager.GetZoneName(assignment.device_index, assignment.zone_index);
            text = QString("%1 - Zone: %2").arg(deviceName, zoneName);
        }
        else if (assignment.led_index >= 0)
        {
            QString ledName = device_manager.GetLEDName(assignment.device_index, assignment.led_index);
            text = QString("%1 - LED: %2").arg(deviceName, ledName);
        }
        else
        {
            text = QString("%1 - Entire Device").arg(deviceName);
        }
        
        QListWidgetItem* item = new QListWidgetItem(text);
        QColor color(RGBGetRValue(assignment.color),
                    RGBGetGValue(assignment.color),
                    RGBGetBValue(assignment.color));
                    
        item->setBackground(color);
        
        // Set text color for readability
        if ((color.red() * 0.299 + color.green() * 0.587 + color.blue() * 0.114) > 186)
        {
            item->setForeground(Qt::black);
        }
        else
        {
            item->setForeground(Qt::white);
        }
        
        ui->assignmentList->addItem(item);
    }
}