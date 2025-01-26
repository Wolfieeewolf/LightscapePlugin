#include "game/GameDataHandler.h"

GameDataHandler::GameDataHandler(QObject* parent)
    : QObject(parent)
{
}

void GameDataHandler::processGameData(const GameData& data)
{
    // Validate and process the game data
    if (data.gameId.isEmpty() || data.dataType.isEmpty())
        return;
        
    // Emit the processed data
    emit dataReceived(data);
}
