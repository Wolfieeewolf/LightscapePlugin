/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectsControlPanel.h                                     |
|                                                           |
| Panel for effect controls                                 |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QMap>
#include "effects/BaseEffect.h"

namespace Ui {
class EffectsControlPanel;
}

namespace Lightscape {

class EffectsControlPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectsControlPanel(QWidget *parent = nullptr);
    ~EffectsControlPanel();
    
    // Effect management
    void setEffect(BaseEffect* effect);
    void clearEffect(BaseEffect* effect = nullptr);
    bool hasEffect(BaseEffect* effect) const;
    void setCurrentEffect(BaseEffect* effect);
    
private:
    Ui::EffectsControlPanel *ui;
    QTabWidget* _tabWidget;
    QMap<BaseEffect*, int> _effectTabs; // Maps effects to their tab index
    BaseEffect* _currentEffect;
    
    void setupUI();
    QString getEffectTabName(BaseEffect* effect) const;
};

} // namespace Lightscape