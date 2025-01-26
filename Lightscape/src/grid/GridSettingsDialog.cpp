#include "grid/GridSettingsDialog.h"
#include "ui_GridSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QMessageBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QHeaderView>

static bool dimensions_changing = false;

GridSettingsDialog::GridSettingsDialog(SpatialGrid* spatial_grid, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::GridSettingsDialog)
    , grid(spatial_grid)
    , labelTables()
{
    ui->setupUi(this);
    
    // Set warning label color to yellow
    ui->warningLabel->setStyleSheet("color: #FFD700;"); // Golden yellow
    
    // Connect the buttons
    connect(ui->resetButton, &QPushButton::clicked, this, &GridSettingsDialog::on_resetDefaultsClicked);
    connect(ui->applyButton, &QPushButton::clicked, this, &GridSettingsDialog::on_applyClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->setUserPositionButton, &QPushButton::clicked, this, &GridSettingsDialog::on_setUserPositionButton_clicked);
    connect(ui->clearUserPositionButton, &QPushButton::clicked, this, &GridSettingsDialog::on_clearUserPositionButton_clicked);
    
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

    // Set window flags to enable close button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
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

    if (width < grid->GetDimensions().width || 
        height < grid->GetDimensions().height || 
        depth < grid->GetDimensions().depth)
    {
        ui->warningLabel->setText("Warning: Reducing grid dimensions will clear existing assignments in the affected areas.");
    }
    else
    {
        ui->warningLabel->clear();
    }

    updateLabels();
}

void GridSettingsDialog::updateCellColors()
{
    // Get current user position from grid
    auto userPosOpt = grid->GetUserPosition();

    // Update colors for all tables
    for (int z = 0; z < labelTables.size(); z++)
    {
        QTableWidget* table = labelTables[z];
        for (int y = 0; y < table->rowCount(); y++)
        {
            for (int x = 0; x < table->columnCount(); x++)
            {
                QTableWidgetItem* item = table->item(y, x);
                if (!item) continue;

                // Clear any background color (use theme default)
                item->setBackground(QBrush());

                // Only set green if we have a valid user position and this is it
                if (userPosOpt.has_value())
                {
                    GridPosition userPos = userPosOpt.value();
                    if (userPos.x == x && userPos.y == y && userPos.z == z)
                    {
                        item->setBackground(Qt::green);
                    }
                }
            }
        }
    }
}

void GridSettingsDialog::on_setUserPositionButton_clicked()
{
    // Get current table
    int currentLayer = ui->layerTabs->currentIndex();
    if (currentLayer < 0 || currentLayer >= labelTables.size()) return;

    QTableWidget* table = labelTables[currentLayer];
    if (!table) return;

    // Get currently selected cell
    QTableWidgetItem* selectedItem = table->currentItem();
    if (!selectedItem) {
        ui->warningLabel->setText("Please select a cell first");
        return;
    }

    // Get position of selected cell
    int row = table->row(selectedItem);
    int col = table->column(selectedItem);

    // Create and emit the new position
    GridPosition newPos(col, row, currentLayer);
    grid->SetUserPosition(newPos);  // Set the position directly
    emit userPositionChanged(newPos);  // Notify any listeners
    
    // Update colors to reflect the change
    updateCellColors();
    ui->warningLabel->setText("User position set");
}

void GridSettingsDialog::on_clearUserPositionButton_clicked()
{
    grid->ClearUserPosition();  // Use the direct clear method
    updateCellColors();         // Update the UI to remove the green highlight
    ui->warningLabel->setText("User position cleared");
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
    if (!nameEdit) return;

    QString newLabel = nameEdit->text();
    grid->SetLayerLabel(index, newLabel);
    ui->layerTabs->setTabText(index, newLabel);
    emit gridUpdated();
}

void GridSettingsDialog::on_resetDefaultsClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Reset Grid Settings",
        "This will reset the grid to 3x3x3 and restore default layer names (Top, Middle, Bottom).\nAll current assignments will be cleared.\n\nAre you sure you want to continue?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Set dimensions back to 3x3x3
        dimensions_changing = true;
        ui->widthSpin->setValue(3);
        ui->heightSpin->setValue(3);
        ui->depthSpin->setValue(3);
        dimensions_changing = false;

        // Update grid dimensions
        GridDimensions dims;
        dims.width = 3;
        dims.height = 3;
        dims.depth = 3;
        grid->SetDimensions(dims);

        // Reset labels and refresh the grid display
        grid->SetDefaultLabels();
        updateLabels();

        // Signal that grid has been updated
        emit gridUpdated();
        
        // Update cell colors
        updateCellColors();
    }
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

    // First update grid dimensions and let it set default labels
    dimensions_changing = true;
    grid->SetDimensions(dims);
    dimensions_changing = false;

    // Now go through and update any custom labels that were set
    for (int z = 0; z < dims.depth; z++)
    {
        // Set layer label
        QLineEdit* nameEdit = findChild<QLineEdit*>(QString("layerName_%1").arg(z));
        if (nameEdit)
        {
            QString newLabel = nameEdit->text();
            grid->SetLayerLabel(z, newLabel);
            ui->layerTabs->setTabText(z, newLabel);
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
                    if (item && !item->text().isEmpty())
                    {
                        grid->SetPositionLabel(GridPosition(x, y, z), item->text());
                    }
                }
            }
        }
    }
    
    // Signal that grid has been updated
    emit gridUpdated();
    
    // Update cell colors
    updateCellColors();
}

void GridSettingsDialog::createLabelTables()
{
    // Clear existing tabs
    ui->layerTabs->clear();
    labelTables.clear();
    
    // Create tabs for each layer
    int depth = ui->depthSpin->value();
    int width = ui->widthSpin->value();
    int height = ui->heightSpin->value();
    
    for (int z = 0; z < depth; z++)
    {
        QWidget* layerWidget = new QWidget();
        QVBoxLayout* layerLayout = new QVBoxLayout(layerWidget);

        // Layer name edit
        QHBoxLayout* nameLayout = new QHBoxLayout();
        QLabel* nameLabel = new QLabel("Layer Name:", layerWidget);
        QLineEdit* nameEdit = new QLineEdit(layerWidget);
        nameEdit->setObjectName(QString("layerName_%1").arg(z));
        
        // Set the current layer name
        nameEdit->setText(grid->GetLayerLabel(z));
        
        nameLayout->addWidget(nameLabel);
        nameLayout->addWidget(nameEdit);
        layerLayout->addLayout(nameLayout);

        connect(nameEdit, &QLineEdit::textChanged, [this, z]() {
            on_layerLabelChanged(z);
        });

        // Grid labels table
        QTableWidget* table = new QTableWidget(height, width, layerWidget);
        table->setObjectName(QString("labelTable_%1").arg(z));
        
        // Populate table with labels
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                QString label;
                GridPosition pos(x, y, z);
                
                // For existing positions within current grid dimensions
                if (z < grid->GetDimensions().depth &&
                    x < grid->GetDimensions().width &&
                    y < grid->GetDimensions().height) {
                    label = grid->GetPositionLabel(pos);
                } else {
                    // For new positions, calculate the position number
                    label = QString("P%1").arg(y * width + x + 1);
                }
                
                QTableWidgetItem* item = new QTableWidgetItem(label);
                table->setItem(y, x, item);
            }
        }

        connect(table, &QTableWidget::cellChanged, this, &GridSettingsDialog::on_labelChanged);

        table->resizeColumnsToContents();
        table->resizeRowsToContents();
        labelTables.append(table);
        layerLayout->addWidget(table);

        ui->layerTabs->addTab(layerWidget, grid->GetLayerLabel(z));
    }
    
    // Update cell colors to show current user position
    updateCellColors();
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
