#ifndef STRENGTH_H
#define STRENGTH_H

#include "Buff.h"

/**
 * @brief 力量状态
 *
 * 永久性叠加层数，不会随回合递减。
 * 每点力量使角色本回合打出的所有攻击牌基础伤害 +1（通过 modifyOutgoingDamage 实现）。
 * 敌人也可以持有力量，使其意图攻击伤害增加。
 */
class Strength : public Buff
{
public:
    /**
     * @param stacks 初始力量点数（正数增益，负数减益）
     */
    explicit Strength(int stacks = 1);

    /**
     * @brief 对输出的攻击伤害增加力量点数
     * @param baseDamage 基础伤害
     * @return 加上力量后的伤害（最低为0）
     */
    int modifyOutgoingDamage(int baseDamage) override;

    QString getDescription() const override;
};

#endif // STRENGTH_H
