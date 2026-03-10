#include "Doom.h"

Doom::Doom(int stacks)
    : Buff(BuffType::DOOM, "灾厄", stacks)
{
}

bool Doom::shouldTriggerDeath(int currentHp) const
{
    return m_stacks >= currentHp;
}

QString Doom::getDescription() const
{
    return QString("灾厄：当前%1层，层数≥生命值时判定死亡").arg(m_stacks);
}
