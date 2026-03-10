#ifndef DOOM_H
#define DOOM_H

#include "Buff.h"

/**
 * @brief 灾厄状态
 *
 * 持续累积层数。当灾厄层数 >= 持有者当前生命值时，
 * 在双方回合都结束后判定持有者死亡。
 * 灾厄层数不会自然递减。
 */
class Doom : public Buff
{
public:
    /**
     * @param stacks 初始灾厄层数
     */
    explicit Doom(int stacks = 1);

    /**
     * @brief 检查灾厄是否触发死亡判定
     * @param currentHp 持有者当前生命值
     * @return true 表示应判定死亡
     */
    bool shouldTriggerDeath(int currentHp) const;

    QString getDescription() const override;
};

#endif // DOOM_H
