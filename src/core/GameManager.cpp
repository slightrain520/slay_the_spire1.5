#include "GameManager.h"

GameManager& GameManager::instance()
{
    static GameManager mgr;
    return mgr;
}

GameManager::GameManager(QObject *parent)
    : QObject(parent)
{
}

void GameManager::startNewGame(int characterIndex)
{
    m_inventory = Inventory();

    if (characterIndex == 0) {
        m_player = std::make_unique<Warrior>();
    } else {
        m_player = std::make_unique<Queen>();
    }

    m_mapManager.generateNewMap();

    emit gameStarted();
}
