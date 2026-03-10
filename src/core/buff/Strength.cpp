#include "Strength.h"
#include <algorithm>

Strength::Strength(int stacks)
    : Buff(BuffType::STRENGTH, "力量", stacks)
{
}

int Strength::modifyOutgoingDamage(int baseDamage)
{
    return std::max(0, baseDamage + m_stacks);
}

QString Strength::getDescription() const
{
    if (m_stacks >= 0) {
        return QString("力量：攻击伤害+%1").arg(m_stacks);
    } else {
        return QString("力量：攻击伤害%1").arg(m_stacks);
    }
}
