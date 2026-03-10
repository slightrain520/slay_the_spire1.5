#include "Vulnerable.h"
#include "data/GameConfig.h"
#include <cmath>

Vulnerable::Vulnerable(int stacks)
    : Buff(BuffType::VULNERABLE, "易伤", stacks)
{
}

int Vulnerable::onTurnEnd()
{
    m_stacks--;
    return 0;
}

int Vulnerable::modifyIncomingDamage(int baseDamage)
{
    return static_cast<int>(std::floor(baseDamage * GameConfig::VULNERABLE_MULTIPLIER));
}

QString Vulnerable::getDescription() const
{
    return QString("易伤：受到的伤害增加50%%，剩余%1回合").arg(m_stacks);
}
