#ifndef GLOVESRELIC_H
#define GLOVESRELIC_H

#include "Relic.h"

class Card;

/**
 * @brief 拳套遗物
 *
 * 每打出5张攻击牌，获得5点格挡。计数在每场战斗开始时重置。
 */
class GlovesRelic : public Relic
{
public:
    GlovesRelic()
        : Relic("gloves", "拳套", "每打出5张攻击牌，获得5点格挡")
    {}

    void onBattleStart(Character *player) override;
    void onCardPlayed(Character *player, const Card *card) override;

    std::shared_ptr<Relic> clone() const override
    {
        return std::make_shared<GlovesRelic>(*this);
    }

private:
    int m_attackCount = 0;
};

#endif // GLOVESRELIC_H
