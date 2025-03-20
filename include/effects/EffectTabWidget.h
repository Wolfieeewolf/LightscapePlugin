/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectTabWidget.h                                         |
|                                                           |
| Tab widget for managing multiple effect instances         |
\*---------------------------------------------------------*/

#pragma once

#include <QTabWidget>
#include <QList>
#include <QMap>
#include "effects/BaseEffect.h"
#include "effects/EffectTabHeader.h"

namespace Lightscape {

class EffectTabWidget : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit EffectTabWidget(QWidget *parent = nullptr);
    ~EffectTabWidget();
    
    // Tab management
    void addEffectTab(BaseEffect* effect);
    void closeEffectTab(int index);
    void onTabCloseRequested(int index); // Make this public so it can be called directly
    BaseEffect* getEffectAtTab(int index) const;
    int indexOf(BaseEffect* effect) const; // Add custom implementation to find effect index
    
    // Current effect
    BaseEffect* getCurrentEffect() const;
    QList<BaseEffect*> getAllEffects() const;
    
signals:
    void effectChanged(BaseEffect* effect);
    void effectClosed(BaseEffect* effect);
    void effectStartStopChanged(BaseEffect* effect, bool running);
    void effectRenamed(BaseEffect* effect, const QString& newName);
    
private slots:
    void onCurrentChanged(int index);
    void onEffectStartStopRequested(bool start);
    void onEffectRenamed(const QString& newName);
    
private:
    QList<BaseEffect*> _effects;
    QMap<BaseEffect*, EffectTabHeader*> _headers;
    QMap<int, BaseEffect*> _tabIndexToEffect;
    QMap<BaseEffect*, QWidget*> _tabPlaceholders;
    
    // Helper method to apply consistent styling
    void applyCustomStyles();
};

} // namespace Lightscape
