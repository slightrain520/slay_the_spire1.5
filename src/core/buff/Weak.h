#ifndef WEAK_H
#define WEAK_H

#include "Buff.h"

/**
 * @brief 虚弱状态
 *
 * 使目标造成的伤害减少50%。
 * stacks 表示剩余生效回合数，每回合结束-1，减至0后消失。
 */
class Weak : public Buff
{
public:
    /**
     * @param stacks 虚弱持续回合数
     */
    explicit Weak(int stacks = 1);

    /**
     * @brief 回合结束时回合数-1
     */
    int onTurnEnd() override;

    /**
     * @brief 造成的伤害减少50%
     */
    int modifyOutgoingDamage(int baseDamage) override;

    QString getDescription() const override;
};

#endif // WEAK_H
