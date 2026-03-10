#ifndef INVENTORY_H
#define INVENTORY_H

#include "Relic.h"
#include "Potion.h"
#include <vector>
#include <memory>
#include "data/GameConfig.h"

/**
 * @brief 背包管理
 *
 * 管理玩家持有的遗物和药水。药水有上限，遗物无上限。
 */
class Inventory
{
public:
    Inventory() = default;

    // ─── 遗物管理 ──────────────────────────────────
    void addRelic(std::shared_ptr<Relic> relic);
    void addRelic(std::shared_ptr<Relic> relic, Character *player);  // 触发onPickup版本
    bool hasRelic(const QString &id) const;
    const std::vector<std::shared_ptr<Relic>>& getRelics() const { return m_relics; }

    /** @brief 在回合开始时重置所有遗物的回合状态 */
    void resetRelicTurnStates();

    // ─── 通行证遗物跳过精英节点 ──────────────────
    bool shouldSkipNextElite() const { return m_skipNextElite; }
    void setSkipNextElite(bool v) { m_skipNextElite = v; }
    void consumeSkipElite() { m_skipNextElite = false; }

    // ─── 药水管理 ──────────────────────────────────
    /**
     * @brief 添加药水
     * @return false 表示药水已满
     */
    bool addPotion(std::shared_ptr<Potion> potion);

    /**
     * @brief 使用药水并从背包中移除
     * @param index  药水槽索引
     * @param user   使用者
     * @param targets 目标
     * @return false 表示索引无效
     */
    bool usePotion(int index, Character *user, std::vector<Character*> targets);

    /**
     * @brief 丢弃药水
     */
    void discardPotion(int index);

    int getPotionCount() const { return static_cast<int>(m_potions.size()); }
    bool isPotionsFull() const { return getPotionCount() >= GameConfig::MAX_POTIONS; }
    const std::vector<std::shared_ptr<Potion>>& getPotions() const { return m_potions; }

private:
    std::vector<std::shared_ptr<Relic>>  m_relics;   ///< 遗物列表
    std::vector<std::shared_ptr<Potion>> m_potions;  ///< 药水列表（上限3）
    bool m_skipNextElite = false;  ///< 通行证遗物：跳过下一个精英节点
};

#endif // INVENTORY_H
