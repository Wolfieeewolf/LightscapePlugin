#include "core/GridValidator.h"

using namespace Lightscape;

GridValidator::GridValidator(SpatialGrid* grid, QObject* parent)
    : QObject(parent)
    , spatialGrid(grid)
    , warningCount(0)
    , errorCount(0)
{
}

QString GridValidator::validateGrid()
{
    // Reset counters
    warningCount = 0;
    errorCount = 0;
    
    // Create a report as a list of strings
    QStringList report;
    
    // Add header
    report << "# Spatial Grid Validation Report";
    report << "-------------------------\n";
    
    // Get all positions with assignments
    GridDimensions dims = spatialGrid->GetDimensions();
    QList<GridPosition> positions;
    
    // Build list of positions with assignments
    for (int z = 0; z < dims.depth; z++) {
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                if (spatialGrid->HasAssignments(pos)) {
                    positions.append(pos);
                }
            }
        }
    }
    
    // Run validation checks
    validateDimensions(report);
    validateUserPosition(report);
    validateLayerDistribution(report, positions);
    validateAssignments(report, positions);
    
    // Add summary
    generateSummary(report);
    
    // Join the report into a single string
    return report.join("\n");
}

void GridValidator::validateDimensions(QStringList& report)
{
    GridDimensions dims = spatialGrid->GetDimensions();
    
    report << "## Grid Information";
    report << QString("- Grid Dimensions: %1 x %2 x %3")
              .arg(dims.width).arg(dims.height).arg(dims.depth);
    report << QString("- Total Positions: %1")
              .arg(dims.width * dims.height * dims.depth);
    
    // Count positions with assignments
    int assignedCount = 0;
    for (int z = 0; z < dims.depth; z++) {
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                if (spatialGrid->HasAssignments(pos)) {
                    assignedCount++;
                }
            }
        }
    }
    
    report << QString("- Positions with Assignments: %1\n").arg(assignedCount);
    
    // Check for reasonable grid size
    const int MAX_RECOMMENDED_SIZE = 10;
    if (dims.width > MAX_RECOMMENDED_SIZE || 
        dims.height > MAX_RECOMMENDED_SIZE || 
        dims.depth > MAX_RECOMMENDED_SIZE) {
        report << "- ⚠️ Warning: Grid dimensions are very large, which may impact performance";
        warningCount++;
    }
    
    if (assignedCount == 0) {
        report << "- ❌ Error: No positions have device assignments";
        errorCount++;
    }
}

void GridValidator::validateUserPosition(QStringList& report)
{
    report << "## Reference Point (User Position)";
    
    if (spatialGrid->HasUserPosition()) {
        GridPosition userPos = spatialGrid->GetUserPosition().value();
        report << QString("- User Position: (%1, %2, %3)")
                  .arg(userPos.x).arg(userPos.y).arg(userPos.z);
        
        QString userPosLabel = spatialGrid->GetPositionLabel(userPos);
        if (!userPosLabel.isEmpty()) {
            report << QString("- Position Label: %1").arg(userPosLabel);
        }
        
        if (spatialGrid->HasAssignments(userPos)) {
            report << "- ✅ User position has device assignments\n";
        } else {
            report << "- ⚠️ Warning: User position has no device assignments\n";
            warningCount++;
        }
    } else {
        report << "- ❌ Error: No user position is set";
        report << "  This will prevent distance-based effects from working\n";
        errorCount++;
    }
}

void GridValidator::validateLayerDistribution(QStringList& report, const QList<GridPosition>& positions)
{
    GridDimensions dims = spatialGrid->GetDimensions();
    
    report << "## Layer Distribution";
    
    if (dims.depth > 0) {
        QMap<int, int> layerCounts;
        
        // Count positions in each layer
        for (const GridPosition& pos : positions) {
            layerCounts[pos.z]++;
        }
        
        // Report on each layer
        for (int z = 0; z < dims.depth; z++) {
            QString layerName = spatialGrid->GetLayerLabel(z);
            int count = layerCounts.value(z, 0);
            float percentage = positions.isEmpty() ? 0.0f : 
                              (static_cast<float>(count) / positions.size()) * 100.0f;
            
            report << QString("- Layer %1 (%2): %3 assignments (%4%)")
                      .arg(z).arg(layerName)
                      .arg(count).arg(percentage, 0, 'f', 1);
            
            if (count == 0) {
                report << "  ⚠️ Warning: Empty layer with no assignments";
                warningCount++;
            }
        }
        report << "";
    } else {
        report << "- ❌ Error: No layers defined in grid\n";
        errorCount++;
    }
}

void GridValidator::validateAssignments(QStringList& report, const QList<GridPosition>& positions)
{
    report << "## Assignment Validation";
    
    // Check for excessive assignments at a single position
    bool hasExcessiveAssignments = false;
    
    for (const GridPosition& pos : positions) {
        QList<DeviceAssignment> assignments = spatialGrid->GetAssignments(pos);
        if (assignments.size() > MAX_RECOMMENDED_ASSIGNMENTS) {
            hasExcessiveAssignments = true;
            report << QString("- ⚠️ Position (%1, %2, %3) has %4 assignments (recommended max: %5)")
                      .arg(pos.x).arg(pos.y).arg(pos.z)
                      .arg(assignments.size())
                      .arg(MAX_RECOMMENDED_ASSIGNMENTS);
            warningCount++;
        }
    }
    
    if (!hasExcessiveAssignments) {
        report << "- ✅ No positions have an excessive number of assignments";
    }
    report << "";
    
    // Check for device assignments
    QMap<unsigned int, QList<GridPosition>> deviceAssignments;
    for (const GridPosition& pos : positions) {
        QList<DeviceAssignment> assignments = spatialGrid->GetAssignments(pos);
        for (const DeviceAssignment& assignment : assignments) {
            deviceAssignments[assignment.device_index].append(pos);
        }
    }
    
    int multipleAssignmentCount = 0;
    report << "## Device Usage";
    
    for (auto it = deviceAssignments.begin(); it != deviceAssignments.end(); ++it) {
        const QList<GridPosition>& assignedPositions = it.value();
        unsigned int deviceIndex = it.key();
        
        if (assignedPositions.size() > 1) {
            multipleAssignmentCount++;
            report << QString("- Device %1 is assigned to %2 different positions")
                      .arg(deviceIndex)
                      .arg(assignedPositions.size());
        }
    }
    
    if (multipleAssignmentCount == 0) {
        report << "- ✅ All devices are assigned to only one position";
    } else {
        report << "- ⚠️ Note: Multiple assignments for the same device may be intentional";
        warningCount++;
    }
    report << "";
}

void GridValidator::generateSummary(QStringList& report)
{
    report << "## Overall Assessment";
    
    if (errorCount > 0) {
        report << QString("- ❌ Found %1 critical issues that need to be fixed").arg(errorCount);
        report << "- These issues will prevent the spatial effects system from working correctly";
    }
    
    if (warningCount > 0) {
        report << QString("- ⚠️ Found %1 warnings that should be reviewed").arg(warningCount);
        report << "- These may impact the quality or performance of spatial effects";
    }
    
    if (errorCount == 0 && warningCount == 0) {
        report << "- ✅ No issues found. Grid configuration looks good!";
    }
}