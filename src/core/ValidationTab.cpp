#include "core/ValidationTab.h"
#include "core/SetupTestDialog.h"

namespace Lightscape {

ValidationTab::ValidationTab(SetupTestDialog* parentDialog, SpatialGrid* grid, QWidget* parent)
    : QWidget(parent)
    , parentDialog(parentDialog)
    , spatialGrid(grid)
{
    // Create validator
    gridValidator = new GridValidator(grid, this);
    
    // Create layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Add header and explanation
    QLabel* titleLabel = new QLabel("<b>Grid Configuration Validation</b>", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 1);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    infoLabel = new QLabel(
        "<b>What this does:</b><br>"
        "This tool checks your grid configuration and device assignments for potential issues. " 
        "It verifies that your spatial grid is properly set up for optimal effect rendering.",
        this);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);
    
    // Add validation button
    validateButton = new QPushButton("Run Validation", this);
    validateButton->setToolTip("Check for potential issues with your grid setup");
    connect(validateButton, &QPushButton::clicked, this, &ValidationTab::onValidateClicked);
    mainLayout->addWidget(validateButton);
    
    // Add results area
    QLabel* resultsTitleLabel = new QLabel("Validation Results:", this);
    resultsTitleLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(resultsTitleLabel);
    
    resultsTextEdit = new QTextEdit(this);
    resultsTextEdit->setReadOnly(true);
    resultsTextEdit->setMinimumHeight(300);
    // No custom styling - let OpenRGB handle theming
    // Set a default text to indicate no validation has been run yet
    resultsTextEdit->setMarkdown("*No validation results yet. Click 'Run Validation' to check your grid configuration.*");
    mainLayout->addWidget(resultsTextEdit);
    
    setLayout(mainLayout);
}

void ValidationTab::onValidateClicked()
{
    // Clear previous results
    resultsTextEdit->clear();
    
    // Run validation
    QString report = gridValidator->validateGrid();
    
    // Display results
    resultsTextEdit->setMarkdown(report);
}

} // namespace Lightscape