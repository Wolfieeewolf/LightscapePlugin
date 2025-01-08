#include "../include/GridSettingsDialog.h"
#include "ui_GridSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QMessageBox>
#include <QGroupBox>
#include <QLineEdit>

static bool dimensions_changing = false;

GridSettingsDialog::GridSettingsDialog(SpatialGrid* spatial_grid, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::GridSettingsDialog)
    , grid(spatial_grid)
    , labelTables()
{
    ui->setupUi(this);
    
    // Connect the buttons
    connect(ui->resetButton, &QPushButton::clicked, this, &GridSettingsDialog::on_resetDefaultsClicked);
    connect(ui->applyButton, &QPushButton::clicked, this, &GridSettingsDialog::on_applyClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // Connect spinboxes
    connect(ui->widthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GridSettingsDialog::on_dimensionsChanged);
    connect(ui->heightSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GridSettingsDialog::on_dimensionsChanged);
    connect(ui->depthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &GridSettingsDialog::on_dimensionsChanged);

    // Set initial values
    dimensions_changing = true;
    ui->widthSpin->setValue(grid->GetDimensions().width);
    ui->heightSpin->setValue(grid->GetDimensions().height);
    ui->depthSpin->setValue(grid->GetDimensions().depth);
    dimensions_changing = false;

    // Create label tables
    createLabelTables();
}

GridSettingsDialog::~GridSettingsDialog()
{
    delete ui;
}

void GridSettingsDialog::on_dimensionsChanged()
{
    if (dimensions_changing) return;

    int width = ui->widthSpin->value();
    int height = ui->heightSpin->value();
    int depth = ui->depthSpin->value();

    // Ask for confirmation if reducing dimensions
    if (width < grid->GetDimensions().width || 
        height < grid->GetDimensions().height || 
        depth < grid->GetDimensions().depth)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
            "Confirm Dimension Change",
            "Reducing grid dimensions will clear existing assignments in the affected areas. Continue?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No)
        {
            dimensions_changing = true;
            ui->widthSpin->setValue(grid->GetDimensions().width);
            ui->heightSpin->setValue(grid->GetDimensions().height);
            ui->depthSpin->setValue(grid->GetDimensions().depth);
            dimensions_changing = false;
            return;
        }
    }

    updateLabels();
}

void GridSettingsDialog::on_labelChanged(int row, int column)
{
    QTableWidget* table = qobject_cast<QTableWidget*>(sender());
    if (!table) return;

    int currentLayer = ui->layerTabs->currentIndex();
    QTableWidgetItem* item = table->item(row, column);
    if (!item) return;

    GridPosition pos(column, row, currentLayer);
    grid->SetPositionLabel(pos, item->text());
}

void GridSettingsDialog::on_layerLabelChanged(int index)
{
    QLineEdit* nameEdit = findChild<QLineEdit*>(QString("layerName_%1").arg(index));
    if (nameEdit)
    {
        grid->SetLayerLabel(index, nameEdit->text());
        ui->layerTabs->setTabText(index, nameEdit->text());
    }
}

void GridSettingsDialog::on_resetDefaultsClicked()
{
    grid->SetDefaultLabels();
    dimensions_changing = true;
    ui->widthSpin->setValue(grid->GetDimensions().width);
    ui->heightSpin->setValue(grid->GetDimensions().height);
    ui->depthSpin->setValue(grid->GetDimensions().depth);
    dimensions_changing = false;
    updateLabels();
}

void GridSettingsDialog::on_applyClicked()
{
    // Get new dimensions
    GridDimensions dims;
    dims.width = ui->widthSpin->value();
    dims.height = ui->heightSpin->value();
    dims.depth = ui->depthSpin->value();

    // Validate dimensions
    if (dims.width <= 0 || dims.height <= 0 || dims.depth <= 0 ||
        dims.width > 10 || dims.height > 10 || dims.depth > 5)
    {
        QMessageBox::warning(this, "Invalid Dimensions", 
            "Grid dimensions must be between 1-10 for width/height and 1-5 for depth.");
        return;
    }

    // Clear existing assignments if dimensions are changing
    if (dims.width != grid->GetDimensions().width ||
        dims.height != grid->GetDimensions().height ||
        dims.depth != grid->GetDimensions().depth)
    {
        grid->ClearAllAssignments();
    }

    // Update grid dimensions
    dimensions_changing = true;
    grid->SetDimensions(dims);

    // Update labels
    for (int z = 0; z < dims.depth; z++)
    {
        // Set layer label
        QLineEdit* nameEdit = findChild<QLineEdit*>(QString("layerName_%1").arg(z));
        if (nameEdit)
        {
            grid->SetLayerLabel(z, nameEdit->text());
        }

        // Set position labels
        if (z < labelTables.size())
        {
            QTableWidget* table = labelTables[z];
            for (int y = 0; y < dims.height && y < table->rowCount(); y++)
            {
                for (int x = 0; x < dims.width && x < table->columnCount(); x++)
                {
                    QTableWidgetItem* item = table->item(y, x);
                    if (item)
                    {
                        grid->SetPositionLabel(GridPosition(x, y, z), item->text());
                    }
                }
            }
        }
    }
    
    dimensions_changing = false;
    accept();
}

void GridSettingsDialog::createLabelTables()
{
    // Clear existing tabs
    ui->layerTabs->clear();
    labelTables.clear();
    
    // Create tabs for each layer
    for (int z = 0; z < grid->GetDimensions().depth; z++)
    {
        QWidget* layerWidget = new QWidget();
        QVBoxLayout* layerLayout = new QVBoxLayout(layerWidget);

        // Layer name edit
        QHBoxLayout* nameLayout = new QHBoxLayout();
        QLabel* nameLabel = new QLabel("Layer Name:", layerWidget);
        QLineEdit* nameEdit = new QLineEdit(grid->GetLayerLabel(z), layerWidget);
        nameEdit->setObjectName(QString("layerName_%1").arg(z));
        nameLayout->addWidget(nameLabel);
        nameLayout->addWidget(nameEdit);
        layerLayout->addLayout(nameLayout);

        connect(nameEdit, &QLineEdit::textChanged, [this, z]() {
            on_layerLabelChanged(z);
        });

        // Grid labels table
        QTableWidget* labelTable = new QTableWidget(grid->GetDimensions().height,
                                                  grid->GetDimensions().width,
                                                  layerWidget);
        labelTable->setObjectName(QString("labelTable_%1").arg(z));
        
        // Populate table with current labels
        for (int y = 0; y < grid->GetDimensions().height; y++)
        {
            for (int x = 0; x < grid->GetDimensions().width; x++)
            {
                GridPosition pos(x, y, z);
                QString label = grid->GetPositionLabel(pos);
                QTableWidgetItem* item = new QTableWidgetItem(label);
                labelTable->setItem(y, x, item);
            }
        }

        connect(labelTable, &QTableWidget::cellChanged, this, &GridSettingsDialog::on_labelChanged);

        labelTable->resizeColumnsToContents();
        labelTable->resizeRowsToContents();
        labelTables.append(labelTable);
        layerLayout->addWidget(labelTable);

        ui->layerTabs->addTab(layerWidget, grid->GetLayerLabel(z));
    }
}

void GridSettingsDialog::updateLabels()
{
    createLabelTables();
}

void GridSettingsDialog::populateCurrentSettings()
{
    // Update spinboxes
    dimensions_changing = true;
    ui->widthSpin->setValue(grid->GetDimensions().width);
    ui->heightSpin->setValue(grid->GetDimensions().height);
    ui->depthSpin->setValue(grid->GetDimensions().depth);
    dimensions_changing = false;

    // Update tables
    updateLabels();
}