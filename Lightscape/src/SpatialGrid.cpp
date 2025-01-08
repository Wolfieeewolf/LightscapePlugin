#include "../include/SpatialGrid.h"
#include <QDebug>

SpatialGrid::SpatialGrid(QObject* parent)
    : QObject(parent)
    , dimensions(3, 3, 3)  // Default 3x3x3 grid
    , selected_button(nullptr)
{
    SetDefaultLabels();
}

void SpatialGrid::SetupGrid(const QVector<QGridLayout*>& layouts)
{
    if (layouts.size() != dimensions.depth)
        return;
        
    ClearGrids();
    
    for (int z = 0; z < dimensions.depth; z++)
    {
        if (!layouts[z])
            continue;
            
        for (int y = 0; y < dimensions.height; y++)
        {
            for (int x = 0; x < dimensions.width; x++)
            {
                CreateGridButton(layouts[z], x, y, z);
            }
        }
    }
}

QString SpatialGrid::GetDefaultPositionLabel(const GridPosition& pos) const
{
    if (!ValidatePosition(pos))
        return QString("P0");
    
    int position_number = pos.y * dimensions.width + pos.x + 1;
    return QString("P%1").arg(position_number);
}

void SpatialGrid::SetDefaultLabels()
{
    static const QString layerLabels[3] = {"Top", "Middle", "Bottom"};
    
    for (int z = 0; z < dimensions.depth; z++)
    {
        // Set layer label
        if (z < 3)
            layer_labels[z] = layerLabels[z];
        else
            layer_labels[z] = QString("Layer %1").arg(z + 1);
            
        // Set position labels with P1, P2, P3 format
        for (int y = 0; y < dimensions.height; y++)
        {
            for (int x = 0; x < dimensions.width; x++)
            {
                GridPosition pos(x, y, z);
                position_labels[pos] = GetDefaultPositionLabel(pos);
            }
        }
    }
}

QString SpatialGrid::GetPositionLabel(const GridPosition& pos) const
{
    return position_labels.value(pos, GetDefaultPositionLabel(pos));
}

void SpatialGrid::SetDimensions(const GridDimensions& dims)
{
    if (dims.width <= 0 || dims.height <= 0 || dims.depth <= 0)
        return;
        
    dimensions = dims;
    emit gridUpdated();
}

void SpatialGrid::CreateGridButton(QGridLayout* grid, int x, int y, int z)
{
    QPushButton* btn = new QPushButton(GetPositionLabel(GridPosition(x, y, z)));
    btn->setMinimumSize(60, 60);
    
    grid->addWidget(btn, y, x);
    button_positions[btn] = GridPosition(x, y, z);
    
    connect(btn, &QPushButton::clicked, this, [this, btn]() {
        selected_button = btn;
        selected_position = button_positions[btn];
        UpdateButtonStyles();
        emit positionSelected(button_positions[btn]);
        emit selectionChanged(btn);
    });
}

void SpatialGrid::SetPositionLabel(const GridPosition& pos, const QString& label)
{
    if (!ValidatePosition(pos))
        return;
        
    position_labels[pos] = label;
    
    if (QPushButton* btn = GetButtonAtPosition(pos))
        btn->setText(label);
}

void SpatialGrid::SetLayerLabel(int layer, const QString& label)
{
    if (layer >= 0 && layer < dimensions.depth)
        layer_labels[layer] = label;
}

QString SpatialGrid::GetLayerLabel(int layer) const
{
    return layer_labels.value(layer, QString("Layer %1").arg(layer + 1));
}

void SpatialGrid::ClearGrids()
{
    for (auto btn : button_positions.keys())
    {
        if (btn == selected_button)
            selected_button = nullptr;
        btn->deleteLater();
    }
    button_positions.clear();
}

void SpatialGrid::UpdateButtonStyles()
{
    // Clear all button styles
    for (auto btn : button_positions.keys())
    {
        btn->setStyleSheet("");
    }
    
    // Highlight selected button
    if (selected_button)
    {
        selected_button->setStyleSheet("QPushButton { background-color: lightblue; }");
    }
}

bool SpatialGrid::ValidatePosition(const GridPosition& pos) const
{
    return pos.x >= 0 && pos.x < dimensions.width &&
           pos.y >= 0 && pos.y < dimensions.height &&
           pos.z >= 0 && pos.z < dimensions.depth;
}

QPushButton* SpatialGrid::GetButtonAtPosition(const GridPosition& pos) const
{
    for (auto it = button_positions.begin(); it != button_positions.end(); ++it)
    {
        if (it.value() == pos)
            return it.key();
    }
    return nullptr;
}

bool SpatialGrid::HasAssignments(const GridPosition& pos) const
{
    return assignments.contains(pos) && !assignments[pos].isEmpty();
}

QList<DeviceAssignment> SpatialGrid::GetAssignments(const GridPosition& pos) const
{
    return assignments.value(pos);
}

void SpatialGrid::AddAssignment(const GridPosition& pos, const DeviceAssignment& assignment)
{
    if (!ValidatePosition(pos))
        return;
        
    if (!assignments.contains(pos))
        assignments[pos] = QList<DeviceAssignment>();
        
    assignments[pos].append(assignment);
    emit assignmentsChanged(pos);
}

void SpatialGrid::RemoveAssignment(const GridPosition& pos, int assignment_index)
{
    if (!ValidatePosition(pos) || !assignments.contains(pos))
        return;
        
    if (assignment_index >= 0 && assignment_index < assignments[pos].size())
    {
        assignments[pos].removeAt(assignment_index);
        emit assignmentsChanged(pos);
    }
}

void SpatialGrid::ClearAssignments(const GridPosition& pos)
{
    if (assignments.remove(pos) > 0)
        emit assignmentsChanged(pos);
}

void SpatialGrid::ClearAllAssignments()
{
    QList<GridPosition> positions = assignments.keys();
    assignments.clear();
    
    for (const auto& pos : positions)
        emit assignmentsChanged(pos);
}