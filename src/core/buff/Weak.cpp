#include "Weak.h"
#include "data/GameConfig.h"
#include <cmath>

Weak::Weak(int stacks)
    : Buff(BuffType::WEAK, "虚弱", stacks)
{
}

int Weak::onTurnEnd()
{
    m_stacks--;
    return 0;
}

int Weak::modifyOutgoingDamage(int baseDamage)
{
    return static_cast<int>(std::floor(baseDamage * GameConfig::WEAK_MULTIPLIER));
}

QString Weak::getDescription() const
{
    return QString("虚弱：造成的伤害减少50%%，剩余%1回合").arg(m_stacks);
}
