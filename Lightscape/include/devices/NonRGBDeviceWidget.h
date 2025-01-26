#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include "devices/NonRGBDevice.h"
#include "grid/NonRGBGridManager.h"

class NonRGBDeviceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit NonRGBDeviceWidget(NonRGBDevice* device, NonRGBGridManager* gridManager, QWidget* parent = nullptr);
    NonRGBDevice* getDevice() const { return device; }

    // Override size hints
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void deviceModified(NonRGBDevice* device);
    void removeRequested(const QString& deviceName);

private slots:
    void onNameChanged(const QString& name);
    void onTypeChanged(int index);
    void onDimensionsChanged();
    void onPositionChanged();
    void onRemoveClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    NonRGBDevice* device;
    NonRGBGridManager* gridManager;

    // UI Elements
    QLineEdit* nameEdit;
    QComboBox* typeCombo;
    QSpinBox* widthSpin;
    QSpinBox* heightSpin;
    QSpinBox* depthSpin;
    QSpinBox* posXSpin;
    QSpinBox* posYSpin;
    QSpinBox* posZSpin;

    void setupUI();
    void setupConnections();
    void loadDeviceData();
    bool validateInput();
    
    // Helper to calculate needed size
    QSize calculateNeededSize() const;
};