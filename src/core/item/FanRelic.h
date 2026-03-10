#ifndef FANRELIC_H
#define FANRELIC_H

#include "Relic.h"

/**
 * @brief 扇子遗物
 *
 * 每场战斗第二回合开始时，获得14点格挡。
 */
class FanRelic : public Relic
{
public:
    FanRelic()
        : Relic("fan", "扇子", "每场战斗第二回合开始时获得14点格挡")
    {}

    void onBattleStart(Character *player) override;
    void onTurnStart(Character *player) override;

    std::shared_ptr<Relic> clone() const override
    {
        return std::make_shared<FanRelic>(*this);
    }

private:
    int m_turnCount = 0;
};

#endif // FANRELIC_H
