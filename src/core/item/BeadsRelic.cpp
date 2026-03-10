#include "BeadsRelic.h"
#include "core/character/Character.h"
#include "data/GameConfig.h"
#include <cmath>

BeadsRelic::BeadsRelic()
    : Relic("beads", "佛珠", "即将死亡时恢复30%最大生命值（一次性）")
{
}

bool BeadsRelic::onAboutToDie(Character *player)
{
    if (m_used || !player) return false;

    m_used = true;
    int healAmount = static_cast<int>(std::floor(player->getMaxHp() * GameConfig::BEADS_HEAL_RATIO));
    player->setHp(healAmount);
    return true;
}

std::shared_ptr<Relic> BeadsRelic::clone() const
{
    return std::make_shared<BeadsRelic>();
}
