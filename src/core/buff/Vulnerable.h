#ifndef VULNERABLE_H
#define VULNERABLE_H

#include "Buff.h"

/**
 * @brief 易伤状态
 *
 * 使目标受到的伤害增加50%。
 * stacks 表示剩余生效回合数，每回合结束-1，减至0后消失。
 */
class Vulnerable : public Buff
{
public:
    /**
     * @param stacks 易伤持续回合数
     */
    explicit Vulnerable(int stacks = 1);

    /**
     * @brief 回合结束时回合数-1
     * @return 不造成直接伤害，返回0
     */
    int onTurnEnd() override;

    /**
     * @brief 受到的伤害增加50%
     */
    int modifyIncomingDamage(int baseDamage) override;

    QString getDescription() const override;
};

#endif // VULNERABLE_H
