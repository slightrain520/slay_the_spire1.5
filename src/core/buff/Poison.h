#ifndef POISON_H
#define POISON_H

#include "Buff.h"

/**
 * @brief 中毒状态
 *
 * 回合结束时，对持有者造成等同于当前层数的伤害（经过格挡抵扣），
 * 然后层数-1。层数减至0后消失。
 */
class Poison : public Buff
{
public:
    /**
     * @param stacks 初始中毒层数
     */
    explicit Poison(int stacks = 1);

    /**
     * @brief 回合结束时造成等于层数的伤害，然后层数-1
     * @return 造成的伤害值
     */
    int onTurnEnd() override;

    QString getDescription() const override;
};

#endif // POISON_H
