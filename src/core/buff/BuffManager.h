#ifndef BUFFMANAGER_H
#define BUFFMANAGER_H

#include "Buff.h"
#include <vector>
#include <memory>

/**
 * @brief Buff 管理器
 *
 * 管理某个角色身上所有的状态效果。
 * 提供添加、查询、回合触发、过期清理等功能。
 */
class BuffManager
{
public:
    BuffManager() = default;

    /**
     * @brief 添加 Buff。若同类型 Buff 已存在，则叠加层数。
     * @param buff 要添加的 Buff（移交所有权）
     */
    void addBuff(std::shared_ptr<Buff> buff);

    /**
     * @brief 获取指定类型的 Buff
     * @return 找到则返回指针，否则 nullptr
     */
    Buff* getBuff(BuffType type) const;

    /**
     * @brief 是否拥有指定类型的 Buff
     */
    bool hasBuff(BuffType type) const;

    /**
     * @brief 获取指定 Buff 的层数
     * @return 层数，未持有则返回0
     */
    int getStacks(BuffType type) const;

    /**
     * @brief 触发所有 Buff 的回合开始效果
     * @return 总共造成的伤害
     */
    int triggerTurnStart();

    /**
     * @brief 触发所有 Buff 的回合结束效果
     * @return 总共造成的伤害（如中毒扣血之和）
     */
    int triggerTurnEnd();

    /**
     * @brief 经过所有 Buff 修改后的输出伤害
     * @param baseDamage 基础伤害
     * @return 修改后的伤害
     */
    int modifyOutgoingDamage(int baseDamage) const;

    /**
     * @brief 经过所有 Buff 修改后的受到伤害
     * @param baseDamage 基础伤害
     * @return 修改后的伤害
     */
    int modifyIncomingDamage(int baseDamage) const;

    /**
     * @brief 清除所有已过期的 Buff
     */
    void removeExpired();

    /**
     * @brief 清除所有 Buff（战斗结束时调用）
     */
    void clearAll();

    /**
     * @brief 获取所有当前 Buff 的只读引用
     */
    const std::vector<std::shared_ptr<Buff>>& getAllBuffs() const { return m_buffs; }

private:
    std::vector<std::shared_ptr<Buff>> m_buffs;  ///< 当前持有的所有 Buff
};

#endif // BUFFMANAGER_H
