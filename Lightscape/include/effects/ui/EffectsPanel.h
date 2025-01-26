#pragma once

#include <QWidget>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QListWidget>
#include "effects/base/EffectEngine.h"
#include "effects/game/GameEffectManager.h"

namespace Ui {
class EffectsPanel;
}

class EffectsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EffectsPanel(EffectEngine* effectEngine = nullptr, 
                         GameEffectManager* gameEffectManager = nullptr, 
                         QWidget *parent = nullptr);
    ~EffectsPanel();

private slots:
    void onEffectChanged(int index);
    void onEffectToggled(bool checked);
    void onSpeedChanged(int value);
    void onIntensityChanged(int value);
    void onColorPickerClicked();
    void onClearEffectsClicked();
    void onSavePresetClicked();
    void onLoadPresetClicked();
    void onPresetSelected(QListWidgetItem* item);
    void onDeletePresetClicked();

private:
    void setupUI();
    void setupConnections();
    void updatePresetsList();
    void loadPreset(const QString& name);
    void savePreset(const QString& name);

    Ui::EffectsPanel *ui;
    EffectEngine *effectEngine;
    GameEffectManager *gameEffectManager;

    // UI Elements
    QComboBox* effectCombo;
    QPushButton* toggleButton;
    QSlider* speedSlider;
    QSlider* intensitySlider;
    QPushButton* colorPickerButton;
    QListWidget* presetsList;
    QPushButton* savePresetButton;
    QPushButton* loadPresetButton;
    QPushButton* deletePresetButton;
    QPushButton* clearEffectsButton;
};