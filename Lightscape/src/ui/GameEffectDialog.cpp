#include "ui/GameEffectDialog.h"
#include <QVBoxLayout>

GameEffectDialog::GameEffectDialog(GameEffectManager* manager, QWidget* parent)
    : QDialog(parent)
    , effectManager(manager)
{
    setupUI();
    
    // Add test game
    gameSelect->addItem("Minecraft");
    
    // Add available effects
    effectsList->addItem("3D Lighting");
    effectsList->addItem("Explosion");
    effectsList->addItem("Health Bar");
    
    setWindowTitle("Game Effects");
}

void GameEffectDialog::setupUI()
{
    auto layout = new QVBoxLayout(this);
    
    gameSelect = new QComboBox(this);
    effectsList = new QListWidget(this);
    testButton = new QPushButton("Test Selected Effect", this);
    
    layout->addWidget(new QLabel("Select Game:"));
    layout->addWidget(gameSelect);
    layout->addWidget(new QLabel("Available Effects:"));
    layout->addWidget(effectsList);
    layout->addWidget(testButton);
    
    connect(testButton, &QPushButton::clicked, this, &GameEffectDialog::onTestEffect);
}

void GameEffectDialog::onTestEffect()
{
    QString selectedEffect = effectsList->currentItem() ? effectsList->currentItem()->text() : "";
    if (selectedEffect.isEmpty())
        return;

    if (selectedEffect == "Explosion") {
        // Create test explosion in center of grid
        GridDimensions dims = effectManager->GetGrid()->GetDimensions();
        GridPosition center(dims.width / 2, dims.height / 2, dims.depth / 2);
        
        GameData data;
        data.gameId = gameSelect->currentText();
        data.dataType = "explosion";
        data.parameters["x"] = center.x;
        data.parameters["y"] = center.y;
        data.parameters["z"] = center.z;
        data.parameters["intensity"] = 1.0f;
        
        QVariantMap color;
        color["r"] = 255;
        color["g"] = 100;
        color["b"] = 0;
        data.parameters["color"] = color;
        
        effectManager->handleGameData(data);
    }
}