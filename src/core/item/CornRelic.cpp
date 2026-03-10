#include "CornRelic.h"
#include "core/character/Character.h"

void CornRelic::onPickup(Character *player)
{
    if (!player) return;
    player->setMaxHp(player->getMaxHp() * 2);
    player->heal(player->getMaxHp());  // 全额治疗（新上限）
}
