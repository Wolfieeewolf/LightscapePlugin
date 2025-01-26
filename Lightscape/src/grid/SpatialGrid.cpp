#include "grid/SpatialGrid.h"
#include <QDebug>

const QString SpatialGrid::USER_POSITION_STYLE = "QPushButton { background-color: #4CAF50; color: white; border: 2px solid #45a049; }";

SpatialGrid::SpatialGrid(QObject* parent)
    : QObject(parent)
    , dimensions(3, 3, 3)
    , selected_button(nullptr)
    , requires_user_position(true)
    , user_position_warning("User position must be set for spatial effects")
{
    SetDefaultLabels();
    SetUserPosition(GridPosition(1, 1, 1));
}

void SpatialGrid::SetDefaultLabels()
{
    static const QString defaultLayerLabels[3] = {"Top", "Middle", "Bottom"};
    
    // Always clear existing labels when setting defaults
    layer_labels.clear();
    position_labels.clear();
    
    for (int z = 0; z < dimensions.depth; z++)
    {
        // Set layer label
        layer_labels[z] = z < 3 ? defaultLayerLabels[z] : QString("Layer %1").arg(z + 1);
        
        // Set position labels for this layer
        for (int y = 0; y < dimensions.height; y++)
        {
            for (int x = 0; x < dimensions.width; x++)
            {
                GridPosition pos(x, y, z);
                // Calculate position number (P1, P2, etc.) for any size grid
                int position_number = (y * dimensions.width + x + 1);
                position_labels[pos] = QString("P%1").arg(position_number);
            }
        }
    }
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
    
    UpdateButtonStyle(btn, GridPosition(x, y, z));
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

void SpatialGrid::UpdateButtonStyle(QPushButton* button, const GridPosition& pos)
{
    if (!button) return;
    
    if (user_position && pos == *user_position)
    {
        button->setStyleSheet(USER_POSITION_STYLE);
    }
    else if (button == selected_button)
    {
        button->setStyleSheet("QPushButton { background-color: lightblue; }");
    }
    else
    {
        button->setStyleSheet("");
    }
}

void SpatialGrid::UpdateButtonStyles()
{
    for (auto btn : button_positions.keys())
    {
        UpdateButtonStyle(btn, button_positions[btn]);
    }
}

void SpatialGrid::UpdateUserPositionWarning()
{
    if (requires_user_position && !HasUserPosition())
    {
        emit userPositionRequired(user_position_warning);
    }
}

QString SpatialGrid::GetDefaultPositionLabel(const GridPosition& pos) const
{
    if (!ValidatePosition(pos))
        return QString("P0");
    
    int position_number = pos.y * dimensions.width + pos.x + 1;
    return QString("P%1").arg(position_number);
}

QString SpatialGrid::GetPositionLabel(const GridPosition& pos) const
{
    return position_labels.value(pos, GetDefaultPositionLabel(pos));
}

void SpatialGrid::SetPositionLabel(const GridPosition& pos, const QString& label)
{
    if (!ValidatePosition(pos))
        return;
        
    position_labels[pos] = label;
    
    if (QPushButton* btn = GetButtonAtPosition(pos))
        btn->setText(label);
}

QString SpatialGrid::GetLayerLabel(int layer) const
{
    return layer_labels.value(layer, QString("Layer %1").arg(layer + 1));
}

void SpatialGrid::SetLayerLabel(int layer, const QString& label)
{
    if (layer >= 0 && layer < dimensions.depth)
    {
        layer_labels[layer] = label;
        emit layerLabelChanged(layer, label);
        emit gridUpdated();
    }
}

void SpatialGrid::SetDimensions(const GridDimensions& dims)
{
    if (dims.width <= 0 || dims.height <= 0 || dims.depth <= 0)
        return;
        
    dimensions = dims;
    SetDefaultLabels(); // Reset labels when dimensions change
    emit gridUpdated();
}

bool SpatialGrid::HasUserPosition() const
{
    return user_position.has_value();
}

std::optional<GridPosition> SpatialGrid::GetUserPosition() const
{
    return user_position;
}

bool SpatialGrid::IsUserPosition(const GridPosition& pos) const
{
    return user_position && pos == *user_position;
}

bool SpatialGrid::SetUserPosition(const GridPosition& pos)
{
    if (!ValidatePosition(pos))
        return false;

    if (user_position)
    {
        QPushButton* old_button = GetButtonAtPosition(*user_position);
        if (old_button)
        {
            UpdateButtonStyle(old_button, *user_position);
        }
    }

    user_position = pos;
    QPushButton* button = GetButtonAtPosition(pos);
    if (button)
    {
        UpdateButtonStyle(button, pos);
        emit userPositionChanged(pos);
        UpdateUserPositionWarning();
        return true;
    }
    
    return false;
}

void SpatialGrid::ClearUserPosition()
{
    if (user_position)
    {
        QPushButton* button = GetButtonAtPosition(*user_position);
        if (button)
        {
            UpdateButtonStyle(button, *user_position);
        }
        user_position.reset();
        emit userPositionChanged(GridPosition(-1, -1, -1));
        UpdateUserPositionWarning();
    }
}

void SpatialGrid::SetRequireUserPosition(bool required)
{
    requires_user_position = required;
    UpdateUserPositionWarning();
}

QString SpatialGrid::GetUserPositionWarning() const
{
    return user_position_warning;
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

bool SpatialGrid::UpdateAssignmentColor(const GridPosition& pos, int index, const RGBColor& color)
{
    if (!ValidatePosition(pos) || !assignments.contains(pos))
        return false;
        
    if (index >= 0 && index < assignments[pos].size())
    {
        assignments[pos][index].color = color;
        emit assignmentsChanged(pos);
        return true;
    }
    return false;
}
