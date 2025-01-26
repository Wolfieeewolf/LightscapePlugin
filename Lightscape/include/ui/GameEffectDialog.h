#pragma once

#include <QDialog>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "../effects/game/GameEffectManager.h"

class GameEffectDialog : public QDialog {
    Q_OBJECT
public:
    explicit GameEffectDialog(GameEffectManager* manager, QWidget* parent = nullptr);

private slots:
    void onTestEffect();

private:
    QComboBox* gameSelect;
    QListWidget* effectsList;
    QPushButton* testButton;
    GameEffectManager* effectManager;
    
    void setupUI();
};