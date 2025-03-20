/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EnhancedEffectWidget.h                                    |
|                                                           |
| Enhanced widget for managing multiple effects             |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include <QMap>
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"
#include "effects/BaseEffect.h"
#include "effects/panels/EffectsListPanel.h"
#include "effects/panels/EffectsControlPanel.h"
#include "effects/panels/EffectsPreviewPanel.h"
#include "effects/panels/DeviceListPanel.h"
#include "core/Types.h"

namespace Ui {
class EnhancedEffectWidget;
}

namespace Lightscape {

class EnhancedEffectWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EnhancedEffectWidget(::DeviceManager* deviceManager = nullptr, 
                                ::SpatialGrid* spatialGrid = nullptr,
                                QWidget *parent = nullptr);
    ~EnhancedEffectWidget();
    
    // Size management
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    
    // Migration helper - use this to migrate from the old EffectWidget
    void migrateFromExistingEffect(BaseEffect* effect);
    
signals:
    void previewToggled(bool enabled);
    void effectAdded(BaseEffect* effect);
    void effectRemoved(BaseEffect* effect);
    void deviceSelectionChanged(const QList<DeviceInfo>& devices);
    
public slots:
    void refresh();
    void addEffect(const QString& effectId);
    
private slots:
    void onAddEffectClicked();
    void onSaveProfileClicked();
    void onLoadProfileClicked();
    void onResetViewClicked();
    void onToggleViewClicked();
    void onChangeLayerClicked();
    void onSelectAllClicked();
    void onClearAllClicked();
    
    // Effect tab management
    void onEffectChanged(BaseEffect* effect);
    void onEffectClosed(BaseEffect* effect);
    void onEffectStartStopChanged(BaseEffect* effect, bool running);
    void onEffectRenamed(BaseEffect* effect, const QString& newName);
    
    // Device selection
    void onDeviceSelectionChanged(const QList<DeviceInfo>& devices);
    
protected:
    void resizeEvent(QResizeEvent* event) override;
    
private:
    void setupUI();
    void setupConnections();
    void updateActiveDevicesForCurrentEffect();
    void updateDevicePositionsForPreview();
    QSize calculateNeededSize() const;
    
    Ui::EnhancedEffectWidget *ui;
    ::DeviceManager* _deviceManager;
    ::SpatialGrid* _spatialGrid;
    
    // UI panels
    EffectsListPanel* _effectsListPanel;
    EffectsControlPanel* _effectsControlPanel;
    EffectsPreviewPanel* _effectsPreviewPanel;
    DeviceListPanel* _deviceListPanel;
    
    // Active devices per effect
    QMap<BaseEffect*, QList<DeviceInfo>> _effectToDevices;
    
    // Active effect tracking
    BaseEffect* _currentEffect;
};

} // namespace Lightscape