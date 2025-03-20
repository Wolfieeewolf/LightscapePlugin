#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>

#include "grid/SpatialGrid.h"
#include "core/GridValidator.h"

namespace Lightscape {

class SetupTestDialog; // Forward declaration

/**
 * @brief Tab widget for validating grid configuration
 */
class ValidationTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a new Validation Tab
     * 
     * @param parentDialog Parent setup test dialog
     * @param grid Pointer to spatial grid
     * @param parent Parent widget
     */
    explicit ValidationTab(SetupTestDialog* parentDialog, SpatialGrid* grid, QWidget* parent = nullptr);

public slots:
    /**
     * @brief Handle validate button click
     */
    void onValidateClicked();

private:
    SetupTestDialog* parentDialog;
    SpatialGrid* spatialGrid;
    GridValidator* gridValidator;
    
    // UI components
    QPushButton* validateButton;
    QTextEdit* resultsTextEdit;
    QLabel* infoLabel;
};

} // namespace Lightscape