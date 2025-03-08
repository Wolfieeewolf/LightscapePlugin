#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QMap>
#include "effects/EffectList.h"
#include "effects/EffectRegistry.h"
#include "effects/BaseEffect.h"
#include "devices/DeviceManager.h"
#include "grid/SpatialGrid.h"

namespace Ui {
class EffectWidget;
}

class EffectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EffectWidget(DeviceManager* deviceManager = nullptr, 
                         SpatialGrid* spatialGrid = nullptr,
                         QWidget *parent = nullptr);
    ~EffectWidget();

    // Size management
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void previewToggled(bool enabled);
    void effectSelected(const QString& effectName);
    void deviceSelectionChanged();

public slots:
    void updateEffectsList();
    void updateDeviceList();
    void updateDeviceSelections();

private slots:
    void onPreviewToggled(bool checked);
    void onEffectSelected();
    void onDeviceToggled();
    void onSelectAllClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void setupConnections();
    QSize calculateNeededSize() const;

    // Device list management
    void createDeviceCheckbox(const QString& name, int deviceIndex, Lightscape::DeviceType type);
    void clearDeviceList();

    Ui::EffectWidget *ui;
    DeviceManager* deviceManager;
    SpatialGrid* spatialGrid;

    struct LocalDeviceInfo {
        int index;
        Lightscape::DeviceType type;
        bool selected;
    };
    QMap<QCheckBox*, LocalDeviceInfo> deviceCheckboxes;
};