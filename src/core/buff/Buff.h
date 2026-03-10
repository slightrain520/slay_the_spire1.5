#ifndef BUFF_H
#define BUFF_H

#include <QString>
#include <memory>

/**
 * @brief Buff 类型枚举
 */
enum class BuffType {
    VULNERABLE,  ///< 易伤：受到伤害增加50%
    WEAK,        ///< 虚弱：造成伤害减少50%
    POISON,      ///< 中毒：回合末扣血等于层数，层数每回合-1
    DOOM,        ///< 灾厄：层数 >= 当前生命値时，避开中毒等结算后判定死亡
    STRENGTH     ///< 力量：每点使所有攻击牌基础伤害+1
};

/**
 * @brief Buff（状态效果）抽象基类
 *
 * 所有状态效果的基类，提供通用属性和虚函数接口。
 * 子类实现具体的回合开始/结束逻辑。
 */
class Buff
{
public:
    /**
     * @param type    Buff 类型
     * @param name    显示名称
     * @param stacks  初始层数/回合数
     */
    Buff(BuffType type, const QString &name, int stacks);
    virtual ~Buff() = default;

    /** @return Buff 类型枚举 */
    BuffType getType() const { return m_type; }

    /** @return Buff 显示名称 */
    QString getName() const { return m_name; }

    /** @return 当前层数 */
    int getStacks() const { return m_stacks; }

    /** @brief 增加层数 */
    void addStacks(int amount) { m_stacks += amount; }

    /** @brief 设置层数 */
    void setStacks(int stacks) { m_stacks = stacks; }

    /** @return 该 Buff 是否已失效（层数 <= 0） */
    bool isExpired() const { return m_stacks <= 0; }

    /**
     * @brief 回合开始时触发
     * @return 对持有者造成的伤害（通常为0）
     */
    virtual int onTurnStart() { return 0; }

    /**
     * @brief 回合结束时触发
     * @return 对持有者造成的伤害（如中毒扣血）
     */
    virtual int onTurnEnd() { return 0; }

    /**
     * @brief 修改该角色造成的伤害
     * @param baseDamage 基础伤害值
     * @return 修改后的伤害值
     */
    virtual int modifyOutgoingDamage(int baseDamage) { return baseDamage; }

    /**
     * @brief 修改该角色受到的伤害
     * @param baseDamage 基础伤害值
     * @return 修改后的伤害值
     */
    virtual int modifyIncomingDamage(int baseDamage) { return baseDamage; }

    /**
     * @brief 获取 Buff 的描述文字
     */
    virtual QString getDescription() const = 0;

protected:
    BuffType m_type;    ///< Buff 类型
    QString  m_name;    ///< 显示名称
    int      m_stacks;  ///< 当前层数/剩余回合数
};

#endif // BUFF_H
