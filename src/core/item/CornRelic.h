#ifndef CORNRELIC_H
#define CORNRELIC_H

#include "Relic.h"

/**
 * @brief 玉米遗物
 *
 * 拾取时：最大生命值翻倍，并恢复满血。
 */
class CornRelic : public Relic
{
public:
    CornRelic()
        : Relic("corn", "玉米", "获得时最大生命值翻倍并恢复满血")
    {}

    void onPickup(Character *player) override;

    std::shared_ptr<Relic> clone() const override
    {
        return std::make_shared<CornRelic>(*this);
    }
};

#endif // CORNRELIC_H
