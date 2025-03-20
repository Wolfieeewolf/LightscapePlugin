/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectsListPanel.h                                        |
|                                                           |
| Panel for effect tabs management                          |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "effects/EffectTabWidget.h"
#include "effects/BaseEffect.h"

namespace Ui {
class EffectsListPanel;
}

namespace Lightscape {

class EffectsListPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectsListPanel(QWidget *parent = nullptr);
    ~EffectsListPanel();
    
    // Expose the tab widget
    EffectTabWidget* getTabWidget() const;
    
    // Forward main operations
    void addEffect(BaseEffect* effect);
    void closeEffect(BaseEffect* effect);
    BaseEffect* getCurrentEffect() const;
    QList<BaseEffect*> getAllEffects() const;
    
    // Update effect running state in UI
    void updateEffectRunningState(BaseEffect* effect, bool running);
    
signals:
    void effectChanged(BaseEffect* effect);
    void effectClosed(BaseEffect* effect);
    void effectStartStopChanged(BaseEffect* effect, bool running);
    void effectRenamed(BaseEffect* effect, const QString& newName);
    
    // Profile management signals
    void addEffectClicked();
    void saveProfileClicked();
    void loadProfileClicked();
    
private:
    Ui::EffectsListPanel *ui;
    EffectTabWidget* _effectTabWidget;
    
    void setupUI();
    void setupConnections();
};

} // namespace Lightscape