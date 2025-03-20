/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectTabWidget.cpp                                       |
|                                                           |
| Tab widget for managing multiple effect instances         |
\*---------------------------------------------------------*/

#include "effects/EffectTabWidget.h"
#include "effects/EffectManager.h"
#include <QTabBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

namespace Lightscape {

void EffectTabWidget::applyCustomStyles()
{
    QTabBar* bar = tabBar();
    if (!bar) return;
    
    // Make sure tab bar has a high z-order so it's not hidden
    bar->raise();
    
    // Tab bar style for high contrast
    bar->setStyleSheet(
        "QTabBar::tab { "
        "  min-height: 35px; "
        "  max-height: 35px; "
        "  min-width: 180px; "
        "  padding: 2px; "
        "  margin: 1px; "
        "  background-color: #444; "
        "  color: white; "
        "  border: 1px solid #555; "
        "} "
        "QTabBar::tab:selected { "
        "  background-color: #666; "
        "  font-weight: bold; "
        "}"
    );
    
    // Style the main widget
    setStyleSheet(
        "QTabWidget::pane { "
        "  background-color: #333; "
        "  border: 1px solid #555; "
        "} "
    );
    
    printf("[Lightscape] Applied custom styles to EffectTabWidget\n");
}

EffectTabWidget::EffectTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    // Configure tab widget for vertical layout
    setTabPosition(QTabWidget::West); // Tabs on the left side
    setTabsClosable(false); // We handle this ourselves with the custom tab headers
    setMovable(true);
    setUsesScrollButtons(true);
    
    // Make sure tabs are visible
    tabBar()->setVisible(true);
    tabBar()->setExpanding(false); // Don't expand tabs to fill width
    tabBar()->setDrawBase(true);   // Draw the base line
    
    // Set minimum width for left side tabs
    tabBar()->setStyleSheet("QTabBar::tab { min-width: 220px; }");
    
    // Set tabbing direction explicitly
    setElideMode(Qt::ElideRight);
    tabBar()->setShape(QTabBar::RoundedWest); // Ensure tabs are on the left
    
    // Apply custom styling
    applyCustomStyles();
    
    // Debug info
    printf("[Lightscape] EffectTabWidget created with vertical tabs on the left side\n");
    
    // Connect signals
    connect(this, &QTabWidget::tabCloseRequested, 
            this, &EffectTabWidget::onTabCloseRequested);
    
    connect(this, &QTabWidget::currentChanged,
            this, &EffectTabWidget::onCurrentChanged);
    
    // Set default minimum size
    setMinimumHeight(200);
}

EffectTabWidget::~EffectTabWidget()
{
    // Block signals to prevent handling of tab removal events during destruction
    blockSignals(true);
    
    // First, ensure all effects are stopped
    for (BaseEffect* effect : _effects) {
        if (effect && effect->getEnabled()) {
            effect->stop();
        }
    }
    
    // Clear tracking collections first to avoid accessing deleted objects
    _effects.clear();
    _headers.clear();
    _tabIndexToEffect.clear();
    
    // The QTabWidget will delete child widgets (including effects) automatically
    // We've removed our references to them to avoid double-delete issues
}

void EffectTabWidget::addEffectTab(BaseEffect* effect)
{
    if (!effect) {
        qWarning() << "Attempted to add null effect to EffectTabWidget";
        return;
    }
    
    if (_effects.contains(effect)) {
        qWarning() << "Effect already in EffectTabWidget";
        return;
    }
    
    // Debug info about effect being added
    EffectInfo info = effect->GetStaticInfo();
    printf("[Lightscape][EffectTabWidget] Adding effect tab - Name: %s, ID: %s\n", 
           info.name.toStdString().c_str(), 
           info.id.toStdString().c_str());
    
    // Create a placeholder widget for the tab instead of using the effect directly
    // We don't actually add the effect here anymore - it will be controlled in EffectsControlPanel
    QWidget* tabPlaceholder = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tabPlaceholder);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // Add a simple label displaying the effect name
    QLabel* nameLabel = new QLabel(info.name, tabPlaceholder);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-weight: bold; color: white;");
    layout->addWidget(nameLabel);
    layout->addStretch();
    
    // Create tab header with controls
    EffectTabHeader* header = new EffectTabHeader(this, effect);
    if (!info.name.isEmpty()) {
        header->setEffectName(info.name); // Set the name explicitly from effect info
    } else {
        header->setEffectName(QString("Effect %1").arg(_effects.size() + 1)); // Use a numbered default name
    }
    header->setMinimumWidth(180); // Make sure the header is wide enough
    header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    header->setVisible(true); // Ensure visibility
    
    // Connect header signals
    connect(header, &EffectTabHeader::closeRequested, [this, effect]() {
        if (!effect || !_effects.contains(effect)) return;
        
        int index = indexOf(effect);
        if (index >= 0) {
            onTabCloseRequested(index);
        }
    });
    
    connect(header, &EffectTabHeader::startStopRequested, 
            this, &EffectTabWidget::onEffectStartStopRequested);
    
    connect(header, &EffectTabHeader::renamed,
            this, &EffectTabWidget::onEffectRenamed);
    
    // Add the placeholder to the tab widget
    int index = addTab(tabPlaceholder, ""); // Empty text since we'll use custom header
    
    // Set the tab header as the tab button
    tabBar()->setTabButton(index, QTabBar::LeftSide, header);
    
    // Make sure it's visible
    tabBar()->setTabVisible(index, true);
    header->show();
    
    // Reapply custom styling to ensure consistency
    applyCustomStyles();
    
    // Debug info
    printf("[Lightscape][EffectTabWidget] Added effect tab: %s (index: %d)\n", 
           info.name.toStdString().c_str(), index);
    
    // Store in our tracking collections
    _effects.append(effect);
    _headers[effect] = header;
    _tabIndexToEffect[index] = effect;
    _tabPlaceholders[effect] = tabPlaceholder;
    
    // Set as current if it's the first tab
    if (_effects.size() == 1) {
        setCurrentIndex(0);
    }
}

void EffectTabWidget::closeEffectTab(int index)
{
    if (index < 0 || index >= count()) {
        qWarning() << "Invalid tab index for closeEffectTab:" << index;
        return;
    }
    
    // Get the effect at this tab
    BaseEffect* effect = _tabIndexToEffect.value(index, nullptr);
    if (!effect) {
        qWarning() << "No effect found at tab index:" << index;
        removeTab(index); // Remove the tab anyway
        return;
    }
    
    printf("[Lightscape][EffectTabWidget] Closing effect tab at index %d: %s\n", 
           index, effect->GetStaticInfo().name.toStdString().c_str());
    
    // Stop the effect if it's running
    if (effect->getEnabled()) {
        printf("[Lightscape][EffectTabWidget] Stopping running effect before closing\n");
        effect->stop();
    }
    
    // Get the placeholder widget
    QWidget* placeholder = _tabPlaceholders.value(effect, nullptr);
    
    // Remove from tracking collections
    _effects.removeOne(effect);
    _headers.remove(effect);
    _tabPlaceholders.remove(effect);
    
    // Clear tab index map as we'll rebuild it completely
    _tabIndexToEffect.clear();
    
    // Block signals to avoid recursive calls
    blockSignals(true);
    
    // Remove the tab
    removeTab(index);
    
    // Delete the placeholder widget if it exists
    if (placeholder) {
        placeholder->deleteLater();
    }
    
    // Delete the effect object
    printf("[Lightscape][EffectTabWidget] Deleting effect object\n");
    effect->deleteLater();
    
    // Rebuild tab index map completely to ensure consistency
    for (int i = 0; i < count(); i++) {
        QWidget* tabWidget = widget(i);
        for (BaseEffect* tabEffect : _effects) {
            QWidget* effectPlaceholder = _tabPlaceholders.value(tabEffect);
            if (tabWidget == effectPlaceholder) {
                _tabIndexToEffect[i] = tabEffect;
                break;
            }
        }
    }
    
    // Force layout update to properly redraw tab widget
    updateGeometry();
    update();
    
    blockSignals(false);
    
    printf("[Lightscape][EffectTabWidget] Tab closed successfully, remaining tabs: %d\n", count());
}

BaseEffect* EffectTabWidget::getEffectAtTab(int index) const
{
    return _tabIndexToEffect.value(index, nullptr);
}

BaseEffect* EffectTabWidget::getCurrentEffect() const
{
    int index = currentIndex();
    if (index >= 0) {
        return getEffectAtTab(index);
    }
    return nullptr;
}

QList<BaseEffect*> EffectTabWidget::getAllEffects() const
{
    return _effects;
}

int EffectTabWidget::indexOf(BaseEffect* effect) const
{
    if (!effect || !_effects.contains(effect)) {
        return -1;
    }
    
    // Search for the effect's index in our map
    for (auto it = _tabIndexToEffect.begin(); it != _tabIndexToEffect.end(); ++it) {
        if (it.value() == effect) {
            return it.key();
        }
    }
    
    // If not found in the map, search through actual tab widgets
    QWidget* placeholder = _tabPlaceholders.value(effect, nullptr);
    if (placeholder) {
        for (int i = 0; i < count(); i++) {
            if (widget(i) == placeholder) {
                return i;
            }
        }
    }
    
    // Not found
    return -1;
}

void EffectTabWidget::onTabCloseRequested(int index)
{
    if (index < 0 || index >= count()) {
        qWarning() << "Invalid tab index for onTabCloseRequested:" << index;
        return;
    }
    
    BaseEffect* effect = getEffectAtTab(index);
    if (!effect) {
        qWarning() << "No effect found at tab index:" << index;
        closeEffectTab(index); // Close the tab anyway
        return;
    }
    
    printf("[Lightscape][EffectTabWidget] Closing tab requested at index %d for effect: %s\n", 
           index, effect->GetStaticInfo().name.toStdString().c_str());
    
    // Emit signal BEFORE closing the tab, so subscribers can respond before objects are deleted
    emit effectClosed(effect);
    
    // Now close the tab - this will handle stopping the effect
    closeEffectTab(index);
}

void EffectTabWidget::onCurrentChanged(int index)
{
    // If index is -1, there are no tabs
    if (index < 0) {
        emit effectChanged(nullptr);
        return;
    }
    
    // Force complete tab index map rebuild when switching tabs
    // This helps keep the map in sync with the actual tab widget state
    _tabIndexToEffect.clear();
    for (int i = 0; i < count(); i++) {
        QWidget* tabWidget = widget(i);
        for (BaseEffect* tabEffect : _effects) {
            QWidget* effectPlaceholder = _tabPlaceholders.value(tabEffect);
            if (tabWidget == effectPlaceholder) {
                _tabIndexToEffect[i] = tabEffect;
                break;
            }
        }
    }
    
    // Now get the effect at the current index
    BaseEffect* effect = getEffectAtTab(index);
    
    // Update geometry to ensure proper tab layout
    updateGeometry();
    tabBar()->updateGeometry();
    
    emit effectChanged(effect);
}

void EffectTabWidget::onEffectStartStopRequested(bool start)
{
    // Identify which tab header sent the signal
    EffectTabHeader* header = qobject_cast<EffectTabHeader*>(sender());
    if (!header) {
        qWarning() << "onEffectStartStopRequested: Sender is not an EffectTabHeader";
        return;
    }
    
    // Find associated effect
    BaseEffect* effect = nullptr;
    for (auto it = _headers.begin(); it != _headers.end(); ++it) {
        if (it.value() == header) {
            effect = it.key();
            break;
        }
    }
    
    if (!effect) {
        qWarning() << "onEffectStartStopRequested: No effect found for header";
        return;
    }
    
    printf("[Lightscape][EffectTabWidget] Start/stop request for effect %s (%p), start: %s\n",
           effect->GetStaticInfo().name.toStdString().c_str(), 
           (void*)effect,
           start ? "true" : "false");
    
    // Start or stop the effect
    if (start) {
        effect->start();
    } else {
        effect->stop();
    }
    
    // Update effect state
    effect->setEnabled(start);
    
    // Emit signal for effect manager to handle
    emit effectStartStopChanged(effect, start);
}

void EffectTabWidget::onEffectRenamed(const QString& newName)
{
    // Identify which tab header sent the signal
    EffectTabHeader* header = qobject_cast<EffectTabHeader*>(sender());
    if (!header) {
        qWarning() << "onEffectRenamed: Sender is not an EffectTabHeader";
        return;
    }
    
    // Find associated effect
    BaseEffect* effect = nullptr;
    for (auto it = _headers.begin(); it != _headers.end(); ++it) {
        if (it.value() == header) {
            effect = it.key();
            break;
        }
    }
    
    if (!effect) {
        qWarning() << "onEffectRenamed: No effect found for header";
        return;
    }
    
    // Find the tab index for this effect
    int index = -1;
    for (auto it = _tabIndexToEffect.begin(); it != _tabIndexToEffect.end(); ++it) {
        if (it.value() == effect) {
            index = it.key();
            break;
        }
    }
    
    // Update tab text
    if (index >= 0) {
        setTabText(index, newName);
    }
    
    // Emit signal for tracking
    emit effectRenamed(effect, newName);
}

} // namespace Lightscape
