#ifndef CROWNRELIC_H
#define CROWNRELIC_H

#include "Relic.h"

/**
 * @brief 王冠遗物
 *
 * 若玩家本回合出牌不超过2张，则该回合敌人对玩家造成的伤害减半。
 * 通过 onCardPlayed 累计出牌数，modifyEnemyDamage 中判断并修改伤害。
 */
class CrownRelic : public Relic
{
public:
    CrownRelic();

    void onCardPlayed(Character *player, const Card *card) override;
    int modifyEnemyDamage(int damage) override;
    void resetTurnState() override;
    std::shared_ptr<Relic> clone() const override;

private:
    int m_cardsPlayedThisTurn = 0;  ///< 本回合已出牌数
};

#endif // CROWNRELIC_H
