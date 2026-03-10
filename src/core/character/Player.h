#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "core/card/Deck.h"
#include <memory>

/**
 * @brief 玩家角色基类
 *
 * 在 Character 基础上增加金币、能量、牌组等玩家特有属性。
 * Warrior 和 Queen 继承此类，提供不同初始牌组。
 */
class Player : public Character
{
public:
    /**
     * @param name      角色名称
     * @param maxHp     最大生命值
     * @param gold      初始金币
     * @param maxEnergy 每回合最大能量
     */
    Player(const QString &name, int maxHp, int gold, int maxEnergy);
    ~Player() override = default;

    /**
     * @brief 初始化牌组（子类在构造函数中调用）
     */
    virtual void initializeDeck() = 0;

    // ─── 金币 ──────────────────────────────────
    int getGold() const { return m_gold; }
    void addGold(int amount) { m_gold += amount; }
    bool spendGold(int amount);

    // ─── 能量 ──────────────────────────────────
    int getEnergy() const { return m_energy; }
    int getMaxEnergy() const { return m_maxEnergy; }
    void setEnergy(int energy) { m_energy = energy; }
    void setMaxEnergy(int maxEnergy) { m_maxEnergy = maxEnergy; }

    /**
     * @brief 消耗能量
     * @return 能量是否足够
     */
    bool spendEnergy(int amount);

    /** @brief 回合开始时重置能量 */
    void resetEnergy() { m_energy = m_maxEnergy; }

    // ─── 牌组 ──────────────────────────────────
    Deck& getDeck() { return m_deck; }
    const Deck& getDeck() const { return m_deck; }

protected:
    int  m_gold;       ///< 金币
    int  m_energy;     ///< 当前能量
    int  m_maxEnergy;  ///< 最大能量
    Deck m_deck;       ///< 玩家牌组
};

#endif // PLAYER_H
