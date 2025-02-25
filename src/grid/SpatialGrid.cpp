#include "grid/SpatialGrid.h"
#include <QDebug>

const QString SpatialGrid::USER_POSITION_STYLE = 
    "QPushButton { background-color: #4CAF50; color: white; border: 2px solid #45a049; border-radius: 4px; }"
    "QPushButton:hover { background-color: #45a049; }";

const QString SpatialGrid::SELECTED_STYLE = 
    "QPushButton { background-color: #2196F3; color: white; border: 2px solid #1976D2; border-radius: 4px; }";

const QString SpatialGrid::ASSIGNMENT_STYLE = 
    "QPushButton { background-color: #FFA726; border: 2px solid #FB8C00; border-radius: 4px; }";

const QString SpatialGrid::DEFAULT_STYLE = "";

SpatialGrid::SpatialGrid(QObject* parent)
    : QObject(parent)
    , dimensions(3, 3, 3)
    , selected_button(nullptr)
    , requires_user_position(false)
{
}

void SpatialGrid::SetDimensions(const GridDimensions& dims)
{
    dimensions = dims;
    emit gridUpdated();
}

void SpatialGrid::SetupGrid(const QVector<QGridLayout*>& layouts)
{
    ClearGrids();

    for (int z = 0; z < dimensions.depth && z < layouts.size(); z++) {
        QGridLayout* grid = layouts[z];
        if (!grid) continue;

        // Clear existing items
        while (grid->count() > 0) {
            QLayoutItem* item = grid->takeAt(0);
            delete item->widget();
            delete item;
        }

        // Create new grid buttons
        for (int y = 0; y < dimensions.height; y++) {
            for (int x = 0; x < dimensions.width; x++) {
                CreateGridButton(grid, x, y, z);
            }
        }
    }

    emit gridUpdated();
}

void SpatialGrid::CreateGridButton(QGridLayout* grid, int x, int y, int z)
{
    QPushButton* button = new QPushButton();
    button->setMinimumSize(40, 40);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    GridPosition pos(x, y, z);
    button_positions[button] = pos;
    
    // Set button text/label
    QString label = GetPositionLabel(pos);
    if (!label.isEmpty()) {
        button->setText(label);
    }
    
    connect(button, &QPushButton::clicked, [this, button, pos]() {
        QPushButton* previousButton = selected_button;
        GridPosition previousPos = selected_position;

        // If clicking the same button, clear selection
        if (selected_button == button) {
            ClearSelection();
            return;
        }

        // Clear the style of the previous button
        if (previousButton) {
            selected_button = nullptr;  // Temporarily clear selection
            UpdateButtonStyle(previousButton, previousPos);
        }

        // Set new selection
        selected_button = button;
        selected_position = pos;
        UpdateButtonStyle(button, pos);
        
        emit positionSelected(pos);
        emit selectionChanged(button);
    });
    
    grid->addWidget(button, y, x);
    UpdateButtonStyle(button, pos);
}

void SpatialGrid::ClearGrids()
{
    for (auto button : button_positions.keys()) {
        if (button == selected_button) {
            selected_button = nullptr;
        }
        button->deleteLater();
    }
    button_positions.clear();
}

void SpatialGrid::SetPositionLabel(const GridPosition& pos, const QString& label)
{
    if (!ValidatePosition(pos)) return;
    
    position_labels[pos] = label;
    
    // Update button text if it exists
    QPushButton* button = GetButtonAtPosition(pos);
    if (button) {
        button->setText(label);
    }
}

void SpatialGrid::SetDefaultLabels()
{
    for (int z = 0; z < dimensions.depth; z++) {
        for (int y = 0; y < dimensions.height; y++) {
            for (int x = 0; x < dimensions.width; x++) {
                GridPosition pos(x, y, z);
                SetPositionLabel(pos, GetDefaultPositionLabel(pos));
            }
        }
    }
}

QString SpatialGrid::GetPositionLabel(const GridPosition& pos) const
{
    if (!ValidatePosition(pos)) return QString();
    return position_labels.value(pos, GetDefaultPositionLabel(pos));
}

QString SpatialGrid::GetDefaultPositionLabel(const GridPosition& pos) const
{
    if (!ValidatePosition(pos)) return QString();
    
    // Calculate 1-based position number
    int posNumber = pos.y * dimensions.width + pos.x + 1;
    return QString("P%1").arg(posNumber);
}

void SpatialGrid::SetLayerLabel(int layer, const QString& label)
{
    if (layer < 0 || layer >= dimensions.depth) return;
    
    layer_labels[layer] = label;
    emit layerLabelChanged(layer, label);
}

QString SpatialGrid::GetLayerLabel(int layer) const
{
    if (layer < 0 || layer >= dimensions.depth) return QString();
    return layer_labels.value(layer, QString("Layer %1").arg(layer + 1));
}

void SpatialGrid::ClearSelection()
{
    if (selected_button) {
        QPushButton* button = selected_button;
        GridPosition pos = selected_position;
        selected_button = nullptr;
        selected_position = GridPosition();
        UpdateButtonStyle(button, pos);
        emit selectionChanged(nullptr);
    }
}

bool SpatialGrid::HasAssignments(const GridPosition& pos) const
{
    if (!ValidatePosition(pos)) return false;
    return assignments.contains(pos) && !assignments[pos].isEmpty();
}

QList<DeviceAssignment> SpatialGrid::GetAssignments(const GridPosition& pos) const
{
    if (!ValidatePosition(pos)) return QList<DeviceAssignment>();
    return assignments.value(pos);
}

void SpatialGrid::AddAssignment(const GridPosition& pos, const DeviceAssignment& assignment)
{
    if (!ValidatePosition(pos)) return;
    
    if (!assignments.contains(pos)) {
        assignments[pos] = QList<DeviceAssignment>();
    }
    assignments[pos].append(assignment);
    
    UpdateButtonStyle(GetButtonAtPosition(pos), pos);
    emit assignmentsChanged(pos);
}

void SpatialGrid::RemoveAssignment(const GridPosition& pos, int index)
{
    if (!ValidatePosition(pos)) return;
    if (!assignments.contains(pos)) return;
    
    auto& assignmentList = assignments[pos];
    if (index >= 0 && index < assignmentList.size()) {
        assignmentList.removeAt(index);
        if (assignmentList.isEmpty()) {
            assignments.remove(pos);
        }
        
        UpdateButtonStyle(GetButtonAtPosition(pos), pos);
        emit assignmentsChanged(pos);
    }
}

void SpatialGrid::ClearAssignments(const GridPosition& pos)
{
    if (!ValidatePosition(pos)) return;
    
    if (assignments.contains(pos)) {
        assignments.remove(pos);
        UpdateButtonStyle(GetButtonAtPosition(pos), pos);
        emit assignmentsChanged(pos);
    }
}

void SpatialGrid::ClearAllAssignments()
{
    QSet<GridPosition> positions = QSet<GridPosition>::fromList(assignments.keys());
    assignments.clear();
    
    for (const GridPosition& pos : positions) {
        UpdateButtonStyle(GetButtonAtPosition(pos), pos);
        emit assignmentsChanged(pos);
    }
}

bool SpatialGrid::UpdateAssignmentColor(const GridPosition& pos, int index, const RGBColor& color)
{
    if (!ValidatePosition(pos)) return false;
    if (!assignments.contains(pos)) return false;
    
    auto& assignmentList = assignments[pos];
    if (index >= assignmentList.size()) return false;
    
    assignmentList[index].color = color;
    emit assignmentsChanged(pos);
    return true;
}

bool SpatialGrid::SetUserPosition(const GridPosition& pos)
{
    if (!ValidatePosition(pos)) return false;
    
    // Clear previous position style if exists
    if (user_position) {
        QPushButton* oldButton = GetButtonAtPosition(user_position.value());
        if (oldButton) {
            UpdateButtonStyle(oldButton, user_position.value());
        }
    }
    
    user_position = pos;
    QPushButton* button = GetButtonAtPosition(pos);
    if (button) {
        UpdateButtonStyle(button, pos);
    }
    
    UpdateUserPositionWarning();
    emit userPositionChanged(pos);
    return true;
}

void SpatialGrid::ClearUserPosition()
{
    if (user_position) {
        QPushButton* oldButton = GetButtonAtPosition(user_position.value());
        if (oldButton) {
            UpdateButtonStyle(oldButton, user_position.value());
        }
        user_position = std::nullopt;
        UpdateUserPositionWarning();
        emit userPositionChanged(GridPosition(-1, -1, -1));
    }
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
    return user_position.has_value() && user_position.value() == pos;
}

void SpatialGrid::SetRequireUserPosition(bool required)
{
    if (requires_user_position != required) {
        requires_user_position = required;
        UpdateUserPositionWarning();
    }
}

QString SpatialGrid::GetUserPositionWarning() const
{
    return user_position_warning;
}

void SpatialGrid::UpdateUserPositionWarning()
{
    QString oldWarning = user_position_warning;
    
    if (requires_user_position && !user_position) {
        user_position_warning = "Please select a reference position for spatial effects";
    } else {
        user_position_warning = QString();
    }
    
    if (oldWarning != user_position_warning) {
        emit userPositionRequired(user_position_warning);
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
    for (auto it = button_positions.begin(); it != button_positions.end(); ++it) {
        if (it.value() == pos) {
            return it.key();
        }
    }
    return nullptr;
}

void SpatialGrid::UpdateButtonStyle(QPushButton* button, const GridPosition& pos)
{
    if (!button) return;
    
    if (button == selected_button) {
        button->setStyleSheet(SELECTED_STYLE);
    }
    else if (IsUserPosition(pos)) {
        button->setStyleSheet(USER_POSITION_STYLE);
    }
    else if (HasAssignments(pos)) {
        button->setStyleSheet(ASSIGNMENT_STYLE);
    }
    else {
        button->setStyleSheet(DEFAULT_STYLE);
    }
}