/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectSelectorDialog.cpp                                  |
|                                                           |
| Dialog for selecting effects to add to the UI             |
\*---------------------------------------------------------*/

#include "effects/EffectSelectorDialog.h"
#include "ui_EffectSelectorDialog.h"
#include "effects/EffectList.h"
#include <QPushButton>

namespace Lightscape {

EffectSelectorDialog::EffectSelectorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EffectSelectorDialog)
{
    ui->setupUi(this);
    
    // Set up the effect tree
    ui->effectsTree->setColumnWidth(0, 150);
    
    // Connect signals
    connect(ui->effectsTree, &QTreeWidget::itemSelectionChanged,
            this, &EffectSelectorDialog::onItemSelectionChanged);
    
    connect(ui->effectsTree, &QTreeWidget::itemDoubleClicked,
            this, &EffectSelectorDialog::onItemDoubleClicked);
    
    // Populate the tree
    populateEffectTree();
    
    // Clear details
    ui->detailsEdit->clear();
    ui->detailsLabel->setText("Select an effect to see details");
}

EffectSelectorDialog::~EffectSelectorDialog()
{
    delete ui;
}

QString EffectSelectorDialog::getSelectedEffectId() const
{
    return _selectedEffectId;
}

void EffectSelectorDialog::onItemSelectionChanged()
{
    QList<QTreeWidgetItem*> selected = ui->effectsTree->selectedItems();
    
    if (selected.isEmpty()) {
        ui->detailsEdit->clear();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }
    
    QTreeWidgetItem* item = selected.first();
    
    // Check if this is an effect or a category
    if (item->parent() == nullptr) {
        // This is a category header
        ui->detailsEdit->clear();
        ui->detailsLabel->setText("Select an effect to see details");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }
    
    // Get the effect ID from the item data
    _selectedEffectId = item->data(0, Qt::UserRole).toString();
    
    // Find the effect info
    for (const auto& category : EffectList::getInstance().getCategories()) {
        EffectCategory cat;
        if (category == "Spatial") cat = EffectCategory::Spatial;
        else if (category == "Basic") cat = EffectCategory::Basic;
        else if (category == "Advanced") cat = EffectCategory::Advanced;
        else cat = EffectCategory::Custom;
        
        for (const auto& info : EffectList::getInstance().getEffectsByCategory(cat)) {
            if (info.id == _selectedEffectId) {
                setDetailsForEffect(info);
                break;
            }
        }
    }
    
    // Enable the OK button
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void EffectSelectorDialog::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    
    // Check if this is an effect item
    if (item->parent() != nullptr) {
        _selectedEffectId = item->data(0, Qt::UserRole).toString();
        accept();
    }
}

void EffectSelectorDialog::populateEffectTree()
{
    ui->effectsTree->clear();
    
    // Get categories and sort them
    QStringList categories = EffectList::getInstance().getCategories();
    categories.sort();
    
    printf("[Lightscape][EffectSelector] Found %d effect categories\n", categories.size());
    
    // Check if any categories were found
    if (categories.isEmpty()) {
        // Add a direct entry for TestEffect as a fallback
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->effectsTree);
        rootItem->setText(0, "Test");
        rootItem->setExpanded(true);
        
        QTreeWidgetItem* testItem = new QTreeWidgetItem(rootItem);
        testItem->setText(0, "Test Effect");
        testItem->setText(1, "A simple test effect that changes color based on position");
        testItem->setData(0, Qt::UserRole, "test_effect");
        
        printf("[Lightscape][EffectSelector] No categories found, added fallback Test Effect\n");
        return;
    }
    
    // Create category nodes
    QMap<QString, QTreeWidgetItem*> categoryItems;
    for (const QString& category : categories) {
        QTreeWidgetItem* categoryItem = new QTreeWidgetItem(ui->effectsTree);
        categoryItem->setText(0, category);
        categoryItem->setFlags(categoryItem->flags() | Qt::ItemIsAutoTristate);
        categoryItem->setExpanded(true);
        categoryItems[category] = categoryItem;
        
        printf("[Lightscape][EffectSelector] Added category: %s\n", category.toStdString().c_str());
    }
    
    // Add effects to each category
    for (const QString& category : categories) {
        EffectCategory cat;
        if (category == "Spatial") cat = EffectCategory::Spatial;
        else if (category == "Basic") cat = EffectCategory::Basic;
        else if (category == "Advanced") cat = EffectCategory::Advanced;
        else if (category == "Test") cat = EffectCategory::Basic; // Add Test category to Basic
        else cat = EffectCategory::Custom;
        
        QTreeWidgetItem* categoryItem = categoryItems[category];
        
        QList<EffectInfo> effects = EffectList::getInstance().getEffectsByCategory(cat);
        printf("[Lightscape][EffectSelector] Category %s has %d effects\n", 
               category.toStdString().c_str(), effects.size());
        
        for (const auto& effect : effects) {
            QTreeWidgetItem* effectItem = new QTreeWidgetItem(categoryItem);
            effectItem->setText(0, effect.name);
            effectItem->setText(1, effect.description);
            effectItem->setData(0, Qt::UserRole, effect.id);
            printf("[Lightscape][EffectSelector] Added effect: %s (id: %s)\n", 
                   effect.name.toStdString().c_str(), effect.id.toStdString().c_str());
        }
    }
    
    // Add TestEffect directly if not found
    bool testEffectFound = false;
    for (int i = 0; i < ui->effectsTree->topLevelItemCount(); i++) {
        QTreeWidgetItem* categoryItem = ui->effectsTree->topLevelItem(i);
        for (int j = 0; j < categoryItem->childCount(); j++) {
            QTreeWidgetItem* effectItem = categoryItem->child(j);
            QString id = effectItem->data(0, Qt::UserRole).toString();
            if (id == "test_effect") {
                testEffectFound = true;
                break;
            }
        }
        if (testEffectFound) break;
    }
    
    if (!testEffectFound) {
        // Add Test category if it doesn't exist
        QTreeWidgetItem* testCategory;
        if (!categoryItems.contains("Test")) {
            testCategory = new QTreeWidgetItem(ui->effectsTree);
            testCategory->setText(0, "Test");
            testCategory->setFlags(testCategory->flags() | Qt::ItemIsAutoTristate);
            testCategory->setExpanded(true);
        } else {
            testCategory = categoryItems["Test"];
        }
        
        // Add the Test Effect
        QTreeWidgetItem* testItem = new QTreeWidgetItem(testCategory);
        testItem->setText(0, "Test Effect");
        testItem->setText(1, "A simple test effect that changes color based on position");
        testItem->setData(0, Qt::UserRole, "test_effect");
        
        printf("[Lightscape][EffectSelector] Added fallback Test Effect\n");
    }
}

void EffectSelectorDialog::setDetailsForEffect(const EffectInfo& info)
{
    QString details = QString("<b>%1</b><br><br>").arg(info.name);
    details += info.description + "<br><br>";
    
    details += "<b>Features:</b><br>";
    details += QString("- Reference Point: %1<br>").arg(info.requiresReferencePoint ? "Required" : "Optional");
    details += QString("- Preview Support: %1<br>").arg(info.supportsPreview ? "Yes" : "No");
    
    // Find category name from category enum
    QString categoryName = "Custom";
    switch (info.category) {
        case EffectCategory::Spatial: categoryName = "Spatial"; break;
        case EffectCategory::Basic: categoryName = "Basic"; break;
        case EffectCategory::Advanced: categoryName = "Advanced"; break;
        default: break;
    }
    
    details += QString("- Category: %1<br>").arg(categoryName);
    
    ui->detailsEdit->setHtml(details);
    ui->detailsLabel->setText("Effect Details:");
}

} // namespace Lightscape
