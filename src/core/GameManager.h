#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <memory>
#include "character/Player.h"
#include "character/Warrior.h"
#include "character/Queen.h"
#include "item/Inventory.h"
#include "map/MapManager.h"
#include "battle/BattleManager.h"

/**
 * @brief 游戏全局管理器（单例）
 *
 * 管理全局游戏状态：当前玩家、背包、地图进度、战斗管理器。
 * 作为核心逻辑与 UI 场景之间的桥梁。
 */
class GameManager : public QObject
{
    Q_OBJECT

public:
    static GameManager& instance();

    /** @brief 以选定角色开始新游戏 */
    void startNewGame(int characterIndex);

    Player* getPlayer() const { return m_player.get(); }
    Inventory& getInventory() { return m_inventory; }
    MapManager& getMapManager() { return m_mapManager; }
    BattleManager& getBattleManager() { return m_battleManager; }

    int getCurrentFloor() const { return m_mapManager.getCurrentLayer() + 1; }

signals:
    void gameStarted();
    void sceneChangeRequested(int sceneId);

private:
    explicit GameManager(QObject *parent = nullptr);

    std::unique_ptr<Player> m_player;
    Inventory m_inventory;
    MapManager m_mapManager;
    BattleManager m_battleManager;
};

#endif // GAMEMANAGER_H
