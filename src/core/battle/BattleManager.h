#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <QObject>
#include <vector>
#include <memory>
#include "core/card/Card.h"

class Character;
class Player;
class Enemy;
class Deck;
class Inventory;

/**
 * @brief 战斗管理器
 *
 * 控制整个战斗流程：初始化、回合管理、出牌执行、
 * Buff 结算、灾厄判定、遗物钩子、胜负判定。
 */
class BattleManager : public QObject
{
    Q_OBJECT

public:
    explicit BattleManager(QObject *parent = nullptr);
    ~BattleManager() override = default;

    /**
     * @brief 开始战斗
     * @param player    玩家指针
     * @param enemies   敌人列表
     * @param inventory 玩家背包（用于遗物钩子）
     */
    void startBattle(Player *player, std::vector<std::shared_ptr<Enemy>> enemies, Inventory *inventory);

    /** @brief 开始玩家回合 */
    void startPlayerTurn();

    /** @brief 结束玩家回合 */
    void endPlayerTurn();

    /** @brief 执行敌人回合 */
    void executeEnemyTurn();

    /**
     * @brief 打出一张卡牌
     * @param card   要打出的卡牌
     * @param target 目标角色（对自身使用时为 player）
     * @return true 表示成功打出
     */
    bool playCard(std::shared_ptr<Card> card, Character *target);

    /** @brief 供 DrawCardEffect 调用的抽牌接口 */
    void drawCards(int count);

    /**
     * @brief 在战斗中使用药水（含战斗结束检测）
     * @param index   药水槽索引
     * @param targets 目标列表
     * @return true 表示成功使用
     */
    bool usePotion(int index, std::vector<Character*> targets);

    // ─── 查询接口 ──────────────────────────────────
    int getCurrentTurn() const { return m_currentTurn; }
    int getEnergy() const { return m_energy; }
    int getMaxEnergy() const { return m_maxEnergy; }
    bool isBattleOver() const { return m_battleOver; }
    bool isPlayerVictory() const { return m_playerVictory; }
    int getCardsPlayedThisTurn() const { return m_cardsPlayedThisTurn; }

    Player* getPlayer() const { return m_player; }
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return m_enemies; }
    std::vector<Enemy*> getAliveEnemies() const;

signals:
    void battleStarted();
    void playerTurnStarted();
    void enemyTurnStarted();
    void enemyTurnFinished();
    void battleEnded(bool playerWon);
    void handUpdated();
    void energyChanged(int current, int max);
    void characterUpdated();
    void cardPlayed(std::shared_ptr<Card> card);
    void turnChanged(int turn);

private:
    /** @brief 结算双方 Buff 回合末效果（中毒扣血等），先判定灾厄再执行 */
    void resolveBuffEndOfTurn(Character *character);

    /** @brief 单个角色的灾厄判定（在中毒扣血之前调用） */
    void checkDoomForCharacter(Character *character);

    /** @brief 灾厄判定（双方回合都结束后）—保留为崔，仅对玩家回合开始时检查玩家自身灾厄 */
    void checkDoomDeath();

    /** @brief 检查战斗是否结束 */
    void checkBattleEnd();

    /** @brief 触发遗物的战斗开始钩子 */
    void triggerRelicBattleStart();

    /** @brief 触发遗物的回合开始钩子 */
    void triggerRelicTurnStart();

    /** @brief 触发遗物的回合结束钩子 */
    void triggerRelicTurnEnd();

    Player *m_player = nullptr;
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    Inventory *m_inventory = nullptr;

    int m_currentTurn = 0;
    int m_energy = 3;
    int m_maxEnergy = 3;
    int m_cardsPlayedThisTurn = 0;
    bool m_battleOver = false;
    bool m_playerVictory = false;
};

#endif // BATTLEMANAGER_H
