/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectsListPanel.cpp                                      |
|                                                           |
| Panel for effect tabs management                          |
\*---------------------------------------------------------*/

#include "effects/panels/EffectsListPanel.h"
#include "ui_EffectsListPanel.h"
#include "effects/EffectTabHeader.h"
#include <QTabBar>
#include <QTimer>

namespace Lightscape {

EffectsListPanel::EffectsListPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EffectsListPanel)
    , _effectTabWidget(nullptr)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

EffectsListPanel::~EffectsListPanel()
{
    delete ui;
}

void EffectsListPanel::setupUI()
{
    // Create the tab widget
    _effectTabWidget = new EffectTabWidget(this);
    if (ui->effectTabContainerLayout) {
        ui->effectTabContainerLayout->addWidget(_effectTabWidget);
        _effectTabWidget->setVisible(true);
        
        // Make sure it has a proper size
        _effectTabWidget->setMinimumHeight(300);
        _effectTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        
        // Debug output
        printf("[Lightscape] EffectsListPanel: Added EffectTabWidget to layout\n");
    } else {
        printf("[Lightscape] ERROR: EffectsListPanel has no effectTabContainerLayout\n");
    }
    
    // Make sure the overall panel is visible and sized properly
    setMinimumWidth(200);
    setMinimumHeight(400); 
}

void EffectsListPanel::setupConnections()
{
    if (_effectTabWidget) {
        connect(_effectTabWidget, &EffectTabWidget::effectChanged,
                this, &EffectsListPanel::effectChanged);
        
        connect(_effectTabWidget, &EffectTabWidget::effectClosed,
                this, &EffectsListPanel::effectClosed);
        
        connect(_effectTabWidget, &EffectTabWidget::effectStartStopChanged,
                this, &EffectsListPanel::effectStartStopChanged);
        
        connect(_effectTabWidget, &EffectTabWidget::effectRenamed,
                this, &EffectsListPanel::effectRenamed);
    }
    
    // Connect the start/stop all button
    if (ui->startStopAllButton) {
        connect(ui->startStopAllButton, &QToolButton::clicked, this, [this](bool checked) {
            // Toggle all effects
            QList<BaseEffect*> effects = getAllEffects();
            for (BaseEffect* effect : effects) {
                updateEffectRunningState(effect, checked);
                emit effectStartStopChanged(effect, checked);
            }
        });
    }
    
    // Connect the profile management buttons
    if (ui->addEffectButton) {
        connect(ui->addEffectButton, &QPushButton::clicked,
                this, &EffectsListPanel::addEffectClicked);
    }
    
    if (ui->saveProfileButton) {
        connect(ui->saveProfileButton, &QPushButton::clicked,
                this, &EffectsListPanel::saveProfileClicked);
    }
    
    if (ui->loadProfileButton) {
        connect(ui->loadProfileButton, &QPushButton::clicked,
                this, &EffectsListPanel::loadProfileClicked);
    }
}

EffectTabWidget* EffectsListPanel::getTabWidget() const
{
    return _effectTabWidget;
}

void EffectsListPanel::addEffect(BaseEffect* effect)
{
    if (!effect) {
        printf("[Lightscape] ERROR: Attempted to add null effect to EffectsListPanel\n");
        return;
    }
    
    if (!_effectTabWidget) {
        printf("[Lightscape] ERROR: EffectTabWidget is null, cannot add effect\n");
        return;
    }
    
    printf("[Lightscape] Adding effect LIST ENTRY to EffectsListPanel: %s\n", 
           effect->GetStaticInfo().name.toStdString().c_str());
    
    // This adds an entry tab with text only, NOT the full effect control panel
    // The actual effect controls will go in the EffectsControlPanel
    
    // Make sure the tab widget is visible
    _effectTabWidget->setVisible(true);
    _effectTabWidget->addEffectTab(effect);
    
    // Force layout update to make sure everything is visible
    _effectTabWidget->updateGeometry();
    updateGeometry();
    
    // Schedule a delayed update to ensure UI is fully refreshed
    QTimer::singleShot(50, this, [this]() {
        if (_effectTabWidget) {
            printf("[Lightscape] Performing delayed layout update after adding effect\n");
            _effectTabWidget->updateGeometry();
            _effectTabWidget->update();
            layout()->update();
            update();
        }
    });
}

void EffectsListPanel::closeEffect(BaseEffect* effect)
{
    if (effect && _effectTabWidget) {
        printf("[Lightscape][EffectsListPanel] Closing effect: %s\n", 
               effect->GetStaticInfo().name.toStdString().c_str());
               
        int index = _effectTabWidget->indexOf(effect);
        if (index >= 0) {
            printf("[Lightscape][EffectsListPanel] Found effect at index %d, closing tab\n", index);
            _effectTabWidget->onTabCloseRequested(index); // Call the method that emits the signal first
            
            // Force a layout update
            QTimer::singleShot(50, this, [this]() {
                updateGeometry();
                layout()->update();
                if (_effectTabWidget) {
                    _effectTabWidget->updateGeometry();
                    _effectTabWidget->layout()->update();
                }
            });
        } else {
            printf("[Lightscape][EffectsListPanel] Effect not found in tab widget\n");
        }
    } else {
        printf("[Lightscape][EffectsListPanel] Invalid effect or tab widget\n");
    }
}

BaseEffect* EffectsListPanel::getCurrentEffect() const
{
    if (_effectTabWidget) {
        return _effectTabWidget->getCurrentEffect();
    }
    return nullptr;
}

QList<BaseEffect*> EffectsListPanel::getAllEffects() const
{
    if (_effectTabWidget) {
        return _effectTabWidget->getAllEffects();
    }
    return QList<BaseEffect*>();
}

void EffectsListPanel::updateEffectRunningState(BaseEffect* effect, bool running)
{
    if (!effect || !_effectTabWidget) return;
    
    // Find the tab header for this effect
    for (int i = 0; i < _effectTabWidget->count(); i++) {
        BaseEffect* tabEffect = _effectTabWidget->getEffectAtTab(i);
        if (tabEffect == effect) {
            // Get the tab button (which should be an EffectTabHeader)
            QWidget* tabButton = _effectTabWidget->tabBar()->tabButton(i, QTabBar::LeftSide);
            EffectTabHeader* header = qobject_cast<EffectTabHeader*>(tabButton);
            
            if (header) {
                // Update the running indicator
                header->toggleRunningIndicator(running);
                
                // Also update the effect's enabled state directly
                effect->setEnabled(running);
            }
            break;
        }
    }
}

} // namespace Lightscape