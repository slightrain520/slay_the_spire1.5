#include "GlovesRelic.h"
#include "core/character/Character.h"
#include "core/card/Card.h"

void GlovesRelic::onBattleStart(Character *player)
{
    Q_UNUSED(player)
    m_attackCount = 0;
}

void GlovesRelic::onCardPlayed(Character *player, const Card *card)
{
    if (!card || card->getCardType() != CardType::ATTACK) return;
    m_attackCount++;
    if (m_attackCount % 5 == 0) {
        player->addBlock(5);
    }
}
