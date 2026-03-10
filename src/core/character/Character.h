#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include "core/buff/BuffManager.h"

/**
 * @brief 角色抽象基类
 *
 * 定义所有角色（玩家和敌人）的共有属性和行为：
 * 生命值、格挡值、Buff 管理等。
 */
class Character
{
public:
    /**
     * @param name  角色名称
     * @param maxHp 最大生命值
     */
    Character(const QString &name, int maxHp);
    virtual ~Character() = default;

    // ─── 生命值操作 ──────────────────────────────────

    /**
     * @brief 受到伤害（格挡优先抵扣）
     *
     * 所有扣血效果统一通过此方法，格挡优先级最高。
     * @param amount 伤害值（已经过 Buff 修正）
     */
    void takeDamage(int amount);

    /**
     * @brief 恢复生命值（不超过最大值）
     * @param amount 恢复量
     */
    void heal(int amount);

    /**
     * @brief 增加格挡值
     */
    void addBlock(int amount);

    /**
     * @brief 清除格挡（回合结束时调用）
     */
    void clearBlock() { m_block = 0; }

    /** @return 是否已死亡 */
    bool isDead() const { return m_hp <= 0; }

    /**
     * @brief 直接设置生命值（用于遗物复活等特殊情况）
     */
    void setHp(int hp);

    /**
     * @brief 直接设置最大生命值（用于玉米遗物等特殊效果）
     * HP同步调整不超过新上限
     */
    void setMaxHp(int maxHp) { m_maxHp = maxHp; m_hp = std::min(m_hp, m_maxHp); }

    // ─── Getters ──────────────────────────────────

    QString getName() const { return m_name; }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
    int getBlock() const { return m_block; }

    /** @return Buff 管理器引用 */
    BuffManager& getBuffManager() { return m_buffManager; }
    const BuffManager& getBuffManager() const { return m_buffManager; }

protected:
    QString     m_name;         ///< 角色名称
    int         m_hp;           ///< 当前生命值
    int         m_maxHp;        ///< 最大生命值
    int         m_block;        ///< 当前格挡值
    BuffManager m_buffManager;  ///< Buff 管理器
};

#endif // CHARACTER_H
