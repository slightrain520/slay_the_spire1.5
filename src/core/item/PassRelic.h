#ifndef PASSRELIC_H
#define PASSRELIC_H

#include "Relic.h"

/**
 * @brief 通行证遗物
 *
 * 拾取时激活跳过精英标志，下一次进入精英节点时跳过战斗并直接领取奖励。
 * Inventory::addRelic(relic, player) 调用 onPickup 后检查 shouldSkipElite()，
 * 并设置到 Inventory 的标志中。
 */
class PassRelic : public Relic
{
public:
    PassRelic()
        : Relic("pass", "通行证", "跳过下一个精英节点的战斗，直接获得奖励")
    {}

    /** onPickup 无需额外操作；标志由 Inventory 在 addRelic(relic,player) 中设置 */
    bool wantsSkipElite() const { return true; }

    std::shared_ptr<Relic> clone() const override
    {
        return std::make_shared<PassRelic>(*this);
    }
};

#endif // PASSRELIC_H
