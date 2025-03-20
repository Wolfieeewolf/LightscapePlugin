#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include "grid/SpatialGrid.h"

namespace Lightscape {

/**
 * @brief Validates grid configuration and device assignments
 * 
 * This class provides validation functionality for the spatial grid,
 * checking for common issues and providing recommendations.
 */
class GridValidator : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Grid Validator
     * 
     * @param grid Pointer to the spatial grid
     * @param parent Parent QObject
     */
    explicit GridValidator(SpatialGrid* grid, QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~GridValidator() = default;
    
    /**
     * @brief Run a complete validation of the grid
     * 
     * @return QString Formatted validation report
     */
    QString validateGrid();
    
    /**
     * @brief Get the number of warnings found in the last validation
     */
    int getWarningCount() const { return warningCount; }
    
    /**
     * @brief Get the number of errors found in the last validation
     */
    int getErrorCount() const { return errorCount; }

private:
    /**
     * @brief Validate the grid dimensions and basic configuration
     * 
     * @param report Validation report to append to
     */
    void validateDimensions(QStringList& report);
    
    /**
     * @brief Validate user position configuration
     * 
     * @param report Validation report to append to
     */
    void validateUserPosition(QStringList& report);
    
    /**
     * @brief Validate layer distribution
     * 
     * @param report Validation report to append to
     * @param positions List of assigned positions
     */
    void validateLayerDistribution(QStringList& report, const QList<GridPosition>& positions);
    
    /**
     * @brief Validate device assignments
     * 
     * @param report Validation report to append to
     * @param positions List of assigned positions
     */
    void validateAssignments(QStringList& report, const QList<GridPosition>& positions);
    
    /**
     * @brief Generate a summary based on errors and warnings
     * 
     * @param report Validation report to append to
     */
    void generateSummary(QStringList& report);

    SpatialGrid* spatialGrid;
    int warningCount;
    int errorCount;
    
    // Constants
    static const int MAX_RECOMMENDED_ASSIGNMENTS = 3;
};

} // namespace Lightscape