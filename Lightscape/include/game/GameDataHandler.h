#pragma once

#include <QObject>
#include <QVariantMap>

struct GameData {
    QString gameId;          // Which game sent this
    QString dataType;        // Type of data (lighting, health, etc)
    QVariantMap parameters;  // Actual game data
    
    GameData() = default;
    GameData(const QString& game, const QString& type)
        : gameId(game), dataType(type) {}
};

class GameDataHandler : public QObject {
    Q_OBJECT
public:
    explicit GameDataHandler(QObject* parent = nullptr);
    
    void processGameData(const GameData& data);

signals:
    void dataReceived(const GameData& data);
};