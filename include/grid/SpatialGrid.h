#pragma once

#include <QObject>
#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include <QString>
#include <QVector>
#include <optional>
#include "RGBController.h"
#include "grid/GridTypes.h"
#include "core/Types.h"

// Forward declarations
class QLabel;

struct GridDimensions {
    int width;
    int height;
    int depth;
    
    GridDimensions(int w = 3, int h = 3, int d = 3) 
        : width(w), height(h), depth(d) {}
};

class DeviceAssignment {
public:
    unsigned int device_index;
    Lightscape::DeviceType device_type;
    int zone_index;
    int led_index;
    RGBColor color;
    
    DeviceAssignment() 
        : device_index(0)
        , device_type(Lightscape::DeviceType::RGB)
        , zone_index(-1)
        , led_index(-1)
        , color(ToRGBColor(0, 0, 0)) {}
        
    DeviceAssignment(unsigned int dev, 
                    Lightscape::DeviceType type = Lightscape::DeviceType::RGB,
                    int zone = -1, 
                    int led = -1) 
        : device_index(dev)
        , device_type(type)
        , zone_index(zone)
        , led_index(led)
        , color(ToRGBColor(0, 0, 0)) {}
};

class SpatialGrid : public QObject {
    Q_OBJECT

public:
    explicit SpatialGrid(QObject* parent = nullptr);
    
    // Grid Setup
    void SetDimensions(const GridDimensions& dims);
    GridDimensions GetDimensions() const { return dimensions; }
    
    // Layout Management
    void SetupGrid(const QVector<QGridLayout*>& layouts);
    void ClearGrids();
    
    // Position Labels
    void SetPositionLabel(const GridPosition& pos, const QString& label);
    void SetDefaultLabels(); // Sets compass-style labels
    QString GetPositionLabel(const GridPosition& pos) const;
    QString GetDefaultPositionLabel(const GridPosition& pos) const;
    
    // Layer Labels
    void SetLayerLabel(int layer, const QString& label);
    QString GetLayerLabel(int layer) const;
    
    // Selection Management
    QPushButton* GetSelectedButton() const { return selected_button; }
    GridPosition GetSelectedPosition() const { return selected_position; }
    void ClearSelection();
    
    // Assignment Management
    bool HasAssignments(const GridPosition& pos) const;
    QList<DeviceAssignment> GetAssignments(const GridPosition& pos) const;
    void AddAssignment(const GridPosition& pos, const DeviceAssignment& assignment);
    void RemoveAssignment(const GridPosition& pos, int assignment_index);
    void ClearAssignments(const GridPosition& pos);
    void ClearAllAssignments();
    bool UpdateAssignmentColor(const GridPosition& pos, int index, const RGBColor& color);

    // User Position Management
    bool SetUserPosition(const GridPosition& pos);
    void ClearUserPosition();
    bool HasUserPosition() const;
    std::optional<GridPosition> GetUserPosition() const;
    bool IsUserPosition(const GridPosition& pos) const;
    bool RequiresUserPosition() const { return requires_user_position; }
    void SetRequireUserPosition(bool required);
    QString GetUserPositionWarning() const;
    void UpdateUserPositionWarning();
    
    // Get position for a device
    std::optional<GridPosition> GetDevicePosition(const Lightscape::DeviceInfo& device) const;

signals:
    void positionSelected(const GridPosition& pos);
    void selectionChanged(QPushButton* button);
    void gridUpdated();
    void assignmentsChanged(const GridPosition& pos);
    void userPositionChanged(const GridPosition& pos);
    void userPositionRequired(const QString& warning);
    void layerLabelChanged(int layer, const QString& newLabel);

private:
    GridDimensions dimensions;
    QPushButton* selected_button;
    GridPosition selected_position;
    QMap<GridPosition, QString> position_labels;
    QMap<int, QString> layer_labels;
    QMap<GridPosition, QList<DeviceAssignment>> assignments;
    QMap<QPushButton*, GridPosition> button_positions;
    std::optional<GridPosition> user_position;
    bool requires_user_position;
    QString user_position_warning;
    
    // Button styles
    static const QString USER_POSITION_STYLE;
    static const QString SELECTED_STYLE;
    static const QString ASSIGNMENT_STYLE;
    static const QString DEFAULT_STYLE;
    
    void CreateGridButton(QGridLayout* grid, int x, int y, int z);
    void UpdateButtonStyles();
    bool ValidatePosition(const GridPosition& pos) const;
    QPushButton* GetButtonAtPosition(const GridPosition& pos) const;
    void UpdateButtonStyle(QPushButton* button, const GridPosition& pos);
};