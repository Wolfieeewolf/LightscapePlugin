/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectsControlPanel.cpp                                   |
|                                                           |
| Panel for effect controls                                 |
\*---------------------------------------------------------*/

#include "effects/panels/EffectsControlPanel.h"
#include "ui_EffectsControlPanel.h"
#include <QDebug>
#include <QScrollArea>
#include <QSlider>
#include <QApplication>
#include <QTimer>

namespace Lightscape {

EffectsControlPanel::EffectsControlPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EffectsControlPanel)
    , _tabWidget(nullptr)
    , _currentEffect(nullptr)
{
    ui->setupUi(this);
    setupUI();
}

EffectsControlPanel::~EffectsControlPanel()
{
    // We don't need to delete the effects, as they are owned elsewhere
    // Just clear the map and delete the UI
    _effectTabs.clear();
    delete ui;
}

void EffectsControlPanel::setupUI()
{
    // Remove the existing scroll area completely from the UI
    if (ui->controlsScrollArea) {
        ui->mainLayout->removeWidget(ui->controlsScrollArea);
        delete ui->controlsScrollArea;
        ui->controlsScrollArea = nullptr; // Avoid dangling pointer
    }
    
    // Create tab widget directly in the panel
    _tabWidget = new QTabWidget(this);
    _tabWidget->setObjectName("effectsTabWidget");
    _tabWidget->setTabPosition(QTabWidget::North);
    _tabWidget->setUsesScrollButtons(true);
    _tabWidget->setElideMode(Qt::ElideRight);
    _tabWidget->setDocumentMode(true);
    
    // Apply styles to make tabs more visible
    _tabWidget->setStyleSheet(
        "QTabWidget::pane { border-top: 2px solid #666; padding-top: 5px; } "
        "QTabBar::tab { background-color: #333; color: #ccc; padding: 8px 16px; margin-right: 2px; font-weight: bold; min-width: 100px; margin-bottom: 1px; } "
        "QTabBar::tab:selected { background-color: #444; color: white; border-bottom: 2px solid #0088ff; margin-bottom: 0px; } "
        "QTabBar::tab:hover:!selected { background-color: #3a3a3a; }"
    );
    
    // Add the tab widget to the main layout
    ui->mainLayout->addWidget(_tabWidget);
    
    printf("[Lightscape] EffectsControlPanel: Created tabbed interface with tabs directly in panel\n");
}

void EffectsControlPanel::setEffect(BaseEffect* effect)
{
    if (!effect) {
        printf("[Lightscape] EffectsControlPanel: Cannot add null effect\n");
        return;
    }
    
    // If we already have this effect, just show its tab
    if (hasEffect(effect)) {
        setCurrentEffect(effect);
        return;
    }
    
    // Store the effect and make it current
    _currentEffect = effect;
    
    // Make sure we have a tab widget
    if (!_tabWidget) {
        printf("[Lightscape] EffectsControlPanel: Tab widget is null, recreating\n");
        setupUI();
        if (!_tabWidget) {
            printf("[Lightscape] EffectsControlPanel: Failed to create tab widget\n");
            return;
        }
    }
    
    // Create a scroll area for this effect
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setObjectName(QString("scrollArea_%1").arg(effect->objectName()));
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setMinimumHeight(250);
    
    // Create a container and layout for this effect
    QWidget* container = new QWidget();
    container->setObjectName(QString("container_%1").arg(effect->objectName()));
    container->setStyleSheet("background-color: #2D2D2D; padding: 5px;");
    
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(6);
    
    // Safely reparent the effect using a safer process
    // First note the current parent for debugging
    QWidget* oldParent = qobject_cast<QWidget*>(effect->parent());
    
    // Try to avoid crashes by carefully detaching and reparenting
    if (oldParent) {
        printf("[Lightscape] Effect has existing parent %p, detaching carefully\n", 
               static_cast<void*>(oldParent));
               
        // We'll use the safer QWidget::setParent() method
        effect->setParent(nullptr);
        QApplication::processEvents(); // Let Qt process the detachment
    }
    
    // Now reparent to the new container
    effect->setParent(container);
    printf("[Lightscape] Effect parent is now %p\n", static_cast<void*>(container));
    
    // Configure the effect properties
    effect->setMinimumWidth(300);
    effect->setMinimumHeight(200);
    effect->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    effect->setVisible(true);
    
    // Add effect to the layout and check child widgets
    layout->addWidget(effect);
    layout->addStretch();
    
    // Force all child widgets to be visible
    QList<QWidget*> childWidgets = effect->findChildren<QWidget*>();
    printf("[Lightscape] Ensuring %d child widgets are visible\n", childWidgets.count());
    for (QWidget* widget : childWidgets) {
        widget->setVisible(true);
    }
    
    // Set the container as the scroll area widget
    scrollArea->setWidget(container);
    
    // Make sure everything is properly laid out
    container->updateGeometry();
    effect->updateGeometry();
    scrollArea->updateGeometry();
    
    // Add a delayed update to ensure everything is visible
    QTimer::singleShot(50, [effect, container, scrollArea]() {
        // Process any pending events first
        QApplication::processEvents();
        
        // Make sure all widgets are visible and properly sized
        effect->adjustSize();
        container->adjustSize();
        scrollArea->updateGeometry();
        
        // Ensure all child widgets are visible
        QList<QWidget*> childWidgets = effect->findChildren<QWidget*>();
        for (QWidget* widget : childWidgets) {
            widget->setVisible(true);
            widget->update();
        }
        
        // Add extra delay for sliders which might need more time to initialize
        QList<QSlider*> sliders = effect->findChildren<QSlider*>();
        if (!sliders.isEmpty()) {
            QTimer::singleShot(100, [sliders]() {
                for (QSlider* slider : sliders) {
                    slider->update();
                    slider->parentWidget()->update();
                }
            });
        }
        
        printf("[Lightscape] Delayed layout update complete for effect %p\n", 
               static_cast<void*>(effect));
    });
    
    // Add to tab widget
    EffectInfo info = effect->GetStaticInfo();
    QString tabName = info.name;
    
    // Make sure we have a valid name
    if (tabName.isEmpty()) {
        tabName = info.id.isEmpty() ? "Effect" : info.id;
    }
    
    // Log tab name for debugging
    printf("[Lightscape] EffectsControlPanel: Setting tab name '%s' for effect %p\n", 
           tabName.toStdString().c_str(), static_cast<void*>(effect));
    
    int tabIndex = _tabWidget->addTab(scrollArea, tabName);
    
    // Store the mapping
    _effectTabs[effect] = tabIndex;
    
    // Select this tab
    _tabWidget->setCurrentIndex(tabIndex);
    
    // Make sure the tab is visible
    _tabWidget->update();
    
    // Debug output
    printf("[Lightscape] EffectsControlPanel: Added effect %p to tab %d (%s)\n", 
           static_cast<void*>(effect), tabIndex, tabName.toStdString().c_str());
}

void EffectsControlPanel::clearEffect(BaseEffect* effect)
{
    if (!_tabWidget) {
        printf("[Lightscape] EffectsControlPanel: Tab widget is null, nothing to clear\n");
        return;
    }
    
    if (!effect) {
        // Clear all effects
        while (_tabWidget->count() > 0) {
            QWidget* widget = _tabWidget->widget(0);
            _tabWidget->removeTab(0);
            
            // The widget is a QScrollArea, get its contents
            QScrollArea* scrollArea = qobject_cast<QScrollArea*>(widget);
            if (scrollArea && scrollArea->widget()) {
                // Find the effect widgets inside the container
                QList<BaseEffect*> effects = scrollArea->widget()->findChildren<BaseEffect*>();
                for (BaseEffect* childEffect : effects) {
                    // Hide the effect and detach it
                    childEffect->hide();
                    childEffect->setParent(nullptr);
                }
            }
            
            // Now delete the scroll area
            if (widget) {
                widget->deleteLater();
            }
        }
        
        _effectTabs.clear();
        _currentEffect = nullptr;
        
        printf("[Lightscape] EffectsControlPanel: Cleared all effects\n");
    } else {
        // Clear a specific effect
        if (_effectTabs.contains(effect)) {
            int tabIndex = _effectTabs[effect];
            
            // Get the widget before removing the tab (which is a QScrollArea)
            QWidget* widget = _tabWidget->widget(tabIndex);
            
            // Process the scroll area to safely detach the effect
            QScrollArea* scrollArea = qobject_cast<QScrollArea*>(widget);
            if (scrollArea && scrollArea->widget()) {
                // Stop any UI operations first
                effect->blockSignals(true);
                
                // Safely detach by hiding first
                effect->hide();
                QApplication::processEvents();
                
                // Then detach from container
                effect->setParent(nullptr);
                effect->blockSignals(false);
                
                printf("[Lightscape] Safely detached effect %p\n", static_cast<void*>(effect));
            } else {
                // Fallback approach
                printf("[Lightscape] Using fallback detachment for effect %p\n", static_cast<void*>(effect));
                effect->hide();
                effect->setParent(nullptr);
            }
            
            // Remove the tab
            _tabWidget->removeTab(tabIndex);
            
            // Schedule the scroll area for deletion
            if (widget) {
                widget->deleteLater();
            }
            
            // Remove from the map
            _effectTabs.remove(effect);
            
            // Update tab indices for tabs that came after this one
            QMutableMapIterator<BaseEffect*, int> i(_effectTabs);
            while (i.hasNext()) {
                i.next();
                if (i.value() > tabIndex) {
                    i.setValue(i.value() - 1);
                }
            }
            
            // Update current effect if needed
            if (_currentEffect == effect) {
                _currentEffect = nullptr;
            }
            
            printf("[Lightscape] EffectsControlPanel: Removed effect %p from tabs\n", 
                   static_cast<void*>(effect));
        } else {
            printf("[Lightscape] EffectsControlPanel: Effect %p not found in tabs\n",
                   static_cast<void*>(effect));
        }
    }
}

bool EffectsControlPanel::hasEffect(BaseEffect* effect) const
{
    return effect && _effectTabs.contains(effect);
}

void EffectsControlPanel::setCurrentEffect(BaseEffect* effect)
{
    if (!effect || !_tabWidget) {
        return;
    }
    
    if (_effectTabs.contains(effect)) {
        int tabIndex = _effectTabs[effect];
        _tabWidget->setCurrentIndex(tabIndex);
        _currentEffect = effect;
        
        printf("[Lightscape] EffectsControlPanel: Set current effect to %p (tab %d)\n", 
               static_cast<void*>(effect), tabIndex);
        
        // Force an update to ensure the tab content is visible
        QTimer::singleShot(10, [this, effect, tabIndex]() {
            // Find the tab widget
            if (_tabWidget && _effectTabs.contains(effect)) {
                QWidget* widget = _tabWidget->widget(tabIndex);
                
                if (widget) {
                    // If it's a scroll area, update its contents
                    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(widget);
                    if (scrollArea && scrollArea->widget()) {
                        QWidget* container = scrollArea->widget();
                        container->updateGeometry();
                        
                        // Force display update of the effect
                        QList<BaseEffect*> effects = container->findChildren<BaseEffect*>();
                        for (BaseEffect* e : effects) {
                            e->setVisible(true);
                            e->updateGeometry();
                            e->QWidget::update(); // Use QWidget's update method, not BaseEffect's
                            
                            // Make sure all child widgets are visible
                            QList<QWidget*> childWidgets = e->findChildren<QWidget*>();
                            for (QWidget* w : childWidgets) {
                                w->setVisible(true);
                                w->update();
                            }
                            
                            // Special handling for sliders
                            QList<QSlider*> sliders = e->findChildren<QSlider*>();
                            for (QSlider* slider : sliders) {
                                slider->update();
                            }
                        }
                    }
                    
                    // Update the tab widget itself
                    widget->updateGeometry();
                    widget->update();
                    _tabWidget->update();
                }
            }
            
            // Add one more delayed update for complex widgets
            QTimer::singleShot(100, [this, effect]() {
                if (_tabWidget && _effectTabs.contains(effect)) {
                    _tabWidget->update();
                }
            });
        });
    } else {
        // Effect not found, add it
        setEffect(effect);
    }
}

QString EffectsControlPanel::getEffectTabName(BaseEffect* effect) const
{
    if (!effect) {
        return "Unknown";
    }
    
    // Get the effect name from its info, or use a fallback
    QString effectName = effect->GetStaticInfo().name;
    
    // If effect name is empty, use its ID or class name as a fallback
    if (effectName.isEmpty()) {
        effectName = effect->GetStaticInfo().id;
        
        // If ID is empty too, use a generic name
        if (effectName.isEmpty()) {
            effectName = "Effect";
        }
    }
    
    printf("[Lightscape] EffectsControlPanel: Using tab name '%s' for effect %p\n", 
           effectName.toStdString().c_str(), static_cast<void*>(effect));
           
    return effectName;
}

} // namespace Lightscape