#include "CrownRelic.h"
#include <cmath>

CrownRelic::CrownRelic()
    : Relic("crown", "王冠", "若本回合出牌不超过2张，敌人对你造成的伤害减半")
{
}

void CrownRelic::onCardPlayed(Character *player, const Card *card)
{
    Q_UNUSED(player)
    Q_UNUSED(card)
    m_cardsPlayedThisTurn++;
}

int CrownRelic::modifyEnemyDamage(int damage)
{
    if (m_cardsPlayedThisTurn <= 2) {
        return static_cast<int>(std::floor(damage * 0.5));
    }
    return damage;
}

void CrownRelic::resetTurnState()
{
    m_cardsPlayedThisTurn = 0;
}

std::shared_ptr<Relic> CrownRelic::clone() const
{
    return std::make_shared<CrownRelic>();
}
