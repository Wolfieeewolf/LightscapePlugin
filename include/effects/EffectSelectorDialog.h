/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectSelectorDialog.h                                    |
|                                                           |
| Dialog for selecting effects to add to the UI             |
\*---------------------------------------------------------*/

#pragma once

#include <QDialog>
#include <QTreeWidgetItem>
#include "effects/EffectInfo.h"

namespace Ui {
class EffectSelectorDialog;
}

namespace Lightscape {

class EffectSelectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EffectSelectorDialog(QWidget *parent = nullptr);
    ~EffectSelectorDialog();
    
    // Get the selected effect
    QString getSelectedEffectId() const;
    
private slots:
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
    
private:
    void populateEffectTree();
    void setDetailsForEffect(const EffectInfo& info);
    
    Ui::EffectSelectorDialog *ui;
    QString _selectedEffectId;
};

} // namespace Lightscape
