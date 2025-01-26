#pragma once
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include "../effects/EffectsEngine.h"

class EffectManagerDialog : public QDialog {
    Q_OBJECT
public:
    explicit EffectManagerDialog(EffectsEngine* engine, QWidget *parent = nullptr);

private slots:
    void onAddEffect();
    void onRemoveEffect();
    void onEffectSelected();

private:
    void setupUi();
    
    EffectsEngine* effectsEngine;
    QListWidget* effectList;
    QPushButton* addButton;
    QPushButton* removeButton;
};