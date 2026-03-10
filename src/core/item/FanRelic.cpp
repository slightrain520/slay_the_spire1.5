#include "FanRelic.h"
#include "core/character/Character.h"

void FanRelic::onBattleStart(Character *player)
{
    Q_UNUSED(player)
    m_turnCount = 0;
}

void FanRelic::onTurnStart(Character *player)
{
    m_turnCount++;
    if (m_turnCount == 2) {
        player->addBlock(14);
    }
}
