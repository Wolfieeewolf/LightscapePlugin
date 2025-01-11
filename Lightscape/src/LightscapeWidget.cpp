#include "../include/LightscapeWidget.h"
#include "../include/GridSettingsDialog.h"
#include "ui_LightscapeWidget.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QtGlobal>

LightscapeWidget::LightscapeWidget(ResourceManager* resource_manager, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LightscapeWidget)
    , device_manager(resource_manager)
    , effect_engine(new EffectEngine(&spatial_grid, this))
{
    ui->setupUi(this);
    setupUserPositionControls();
    setupInitialState();
    setupConnections();
    setupEffectControls();
    populateDeviceList();

    // Connect effect engine signals
    connect(effect_engine, &EffectEngine::colorsUpdated, this, &LightscapeWidget::updateDeviceColors);
}

LightscapeWidget::~LightscapeWidget()
{
    effect_engine->stopEffect();
    delete ui;
}

void LightscapeWidget::setupUserPositionControls()
{
    QGroupBox* position_group = new QGroupBox("User Position", this);
    QVBoxLayout* position_layout = new QVBoxLayout(position_group);

    set_user_position_button = new QPushButton("Set User Position", this);
    clear_user_position_button = new QPushButton("Clear User Position", this);
    warning_label = new QLabel(this);
    warning_label->setWordWrap(true);
    warning_label->setStyleSheet("QLabel { color: red; }");

    position_layout->addWidget(set_user_position_button);
    position_layout->addWidget(clear_user_position_button);
    position_layout->addWidget(warning_label);

    ui->mainLayout->addWidget(position_group);
    clear_user_position_button->setEnabled(false);
}

void LightscapeWidget::setupEffectControls()
{
    connect(ui->effectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &LightscapeWidget::on_effectComboChanged);
    connect(ui->speedSlider, &QSlider::valueChanged, 
            this, &LightscapeWidget::on_speedSliderChanged);
    connect(ui->intensitySlider, &QSlider::valueChanged, 
            this, &LightscapeWidget::on_intensitySliderChanged);
}

void LightscapeWidget::setupInitialState()
{
    // Empty now that timer is removed
}

void LightscapeWidget::setupConnections()
{
    connect(ui->configureButton, &QPushButton::clicked, this, &LightscapeWidget::on_configureButton_clicked);
    connect(ui->deviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LightscapeWidget::on_deviceComboChanged);
    connect(ui->assignButton, &QPushButton::clicked, this, &LightscapeWidget::on_assignButtonClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &LightscapeWidget::on_removeButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &LightscapeWidget::on_clearButtonClicked);
    connect(ui->colorButton, &QPushButton::clicked, this, &LightscapeWidget::on_colorButtonClicked);
    connect(set_user_position_button, &QPushButton::clicked, this, &LightscapeWidget::on_setUserPositionClicked);
    connect(clear_user_position_button, &QPushButton::clicked, this, &LightscapeWidget::on_clearUserPositionClicked);

    connect(ui->wholeDeviceRadio, &QRadioButton::toggled, this, &LightscapeWidget::on_assignmentTypeChanged);
    connect(ui->zoneRadio, &QRadioButton::toggled, this, &LightscapeWidget::on_assignmentTypeChanged);
    connect(ui->ledRadio, &QRadioButton::toggled, this, &LightscapeWidget::on_assignmentTypeChanged);
}

void LightscapeWidget::on_effectComboChanged(int index)
{
    if (index == 0) // None
    {
        effect_engine->stopEffect();
    }
    else
    {
        effect_engine->startEffect(static_cast<EffectEngine::Effect>(index));
    }
}

void LightscapeWidget::on_speedSliderChanged(int value)
{
    effect_engine->setSpeed(value);
}

void LightscapeWidget::on_intensitySliderChanged(int value)
{
    effect_engine->setIntensity(value);
}

void LightscapeWidget::populateDeviceList()
{
    ui->deviceCombo->clear();
    for(unsigned int i = 0; i < device_manager.GetDeviceCount(); i++)
    {
        ui->deviceCombo->addItem(device_manager.GetDeviceName(i));
    }
}

void LightscapeWidget::on_configureButton_clicked()
{
    GridSettingsDialog dialog(&spatial_grid, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        refreshGridDisplay();
    }
}

void LightscapeWidget::on_deviceComboChanged(int index)
{
    if(index >= 0)
    {
        updateSelectionCombo();
    }
}

void LightscapeWidget::on_assignmentTypeChanged()
{
    updateSelectionCombo();
}

void LightscapeWidget::updateSelectionCombo()
{
    ui->selectionCombo->clear();
    int device_index = ui->deviceCombo->currentIndex();
    
    if(device_index < 0)
    {
        return;
    }

    if(ui->zoneRadio->isChecked())
    {
        for(unsigned int i = 0; i < device_manager.GetZoneCount(device_index); i++)
        {
            ui->selectionCombo->addItem(device_manager.GetZoneName(device_index, i));
        }
    }
    else if(ui->ledRadio->isChecked())
    {
        for(unsigned int i = 0; i < device_manager.GetLEDCount(device_index); i++)
        {
            ui->selectionCombo->addItem(device_manager.GetLEDName(device_index, i));
        }
    }
    else
    {
        ui->selectionCombo->addItem("Whole Device");
    }
}

void LightscapeWidget::on_assignButtonClicked()
{
    if(!spatial_grid.GetSelectedButton())
    {
        QMessageBox::warning(this, "Error", "Please select a position first.");
        return;
    }

    DeviceAssignment assignment;
    assignment.device_index = ui->deviceCombo->currentIndex();
    assignment.color = ToRGBColor(0, 0, 0);

    if(ui->zoneRadio->isChecked())
    {
        assignment.zone_index = ui->selectionCombo->currentIndex();
        assignment.led_index = -1;
    }
    else if(ui->ledRadio->isChecked())
    {
        assignment.zone_index = -1;
        assignment.led_index = ui->selectionCombo->currentIndex();
    }
    else
    {
        assignment.zone_index = -1;
        assignment.led_index = -1;
    }

    spatial_grid.AddAssignment(spatial_grid.GetSelectedPosition(), assignment);
    updateAssignmentsList();
    updateDeviceColors();
}

void LightscapeWidget::on_removeButtonClicked()
{
    QModelIndexList selection = ui->assignmentList->selectionModel()->selectedIndexes();
    if(!selection.isEmpty())
    {
        spatial_grid.RemoveAssignment(spatial_grid.GetSelectedPosition(), selection.first().row());
        updateAssignmentsList();
        updateDeviceColors();
    }
}

void LightscapeWidget::on_clearButtonClicked()
{
    if(spatial_grid.GetSelectedButton())
    {
        spatial_grid.ClearAssignments(spatial_grid.GetSelectedPosition());
        updateAssignmentsList();
        updateDeviceColors();
    }
}

void LightscapeWidget::on_colorButtonClicked()
{
    QColor current_color = ui->colorButton->palette().button().color();
    QColor color = QColorDialog::getColor(current_color, this, "Select Color", 
                                        QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
    if(color.isValid())
    {
        QPalette pal = ui->colorButton->palette();
        pal.setColor(QPalette::Button, color);
        ui->colorButton->setPalette(pal);
        ui->colorButton->update();

        RGBColor rgb_color = ToRGBColor(color.red(), color.green(), color.blue());
        
        if(spatial_grid.GetSelectedButton())
        {
            QModelIndexList selection = ui->assignmentList->selectionModel()->selectedIndexes();
            if(!selection.isEmpty())
            {
                spatial_grid.UpdateAssignmentColor(spatial_grid.GetSelectedPosition(), 
                                                 selection.first().row(), 
                                                 rgb_color);
                updateDeviceColors();
            }
            else
            {
                QMessageBox::warning(this, "Error", "Please select an assignment to change its color.");
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Please select a grid position first.");
        }
    }
}

void LightscapeWidget::on_gridPositionSelected(const GridPosition& pos)
{
    updateAssignmentsList();
}

void LightscapeWidget::updateAssignmentsList()
{
    ui->assignmentList->clear();
    if(!spatial_grid.GetSelectedButton())
    {
        return;
    }

    const auto& assignments = spatial_grid.GetAssignments(spatial_grid.GetSelectedPosition());
    for(const auto& assignment : assignments)
    {
        QString text;
        text += device_manager.GetDeviceName(assignment.device_index) + ": ";
        
        if(assignment.zone_index >= 0)
        {
            text += device_manager.GetZoneName(assignment.device_index, assignment.zone_index);
        }
        else if(assignment.led_index >= 0)
        {
            text += device_manager.GetLEDName(assignment.device_index, assignment.led_index);
        }
        else
        {
            text += "Whole Device";
        }

        ui->assignmentList->addItem(text);
    }
}

void LightscapeWidget::updateDeviceColors()
{
    const GridDimensions dims = spatial_grid.GetDimensions();
    
    for (int z = 0; z < dims.depth; z++)
    {
        for (int y = 0; y < dims.height; y++)
        {
            for (int x = 0; x < dims.width; x++)
            {
                GridPosition pos(x, y, z);
                if (spatial_grid.HasAssignments(pos))
                {
                    const QList<DeviceAssignment>& assignments = spatial_grid.GetAssignments(pos);
                    for (const DeviceAssignment& assignment : assignments)
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

void LightscapeWidget::refreshGridDisplay()
{
    spatial_grid.SetupGrid({
        ui->topGrid,
        ui->middleGrid,
        ui->bottomGrid
    });
}

void LightscapeWidget::updateWarningLabel(const QString& warning)
{
    warning_label->setText(warning);
    warning_label->setVisible(!warning.isEmpty());
}

void LightscapeWidget::on_setUserPositionClicked()
{
    if (!spatial_grid.GetSelectedButton())
    {
        QMessageBox::warning(this, "Error", "Please select a position first.");
        return;
    }

    if (spatial_grid.SetUserPosition(spatial_grid.GetSelectedPosition()))
    {
        set_user_position_button->setEnabled(false);
        clear_user_position_button->setEnabled(true);
    }
}

void LightscapeWidget::on_clearUserPositionClicked()
{
    spatial_grid.ClearUserPosition();
    set_user_position_button->setEnabled(true);
    clear_user_position_button->setEnabled(false);
}

void LightscapeWidget::on_userPositionChanged(const GridPosition& pos)
{
    set_user_position_button->setEnabled(!spatial_grid.HasUserPosition());
    clear_user_position_button->setEnabled(spatial_grid.HasUserPosition());
    updateWarningLabel(spatial_grid.HasUserPosition() ? "" : spatial_grid.GetUserPositionWarning());
}

void LightscapeWidget::on_userPositionRequired(const QString& warning)
{
    updateWarningLabel(warning);
}