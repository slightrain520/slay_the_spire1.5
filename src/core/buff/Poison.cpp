#include "Poison.h"

Poison::Poison(int stacks)
    : Buff(BuffType::POISON, "中毒", stacks)
{
}

int Poison::onTurnEnd()
{
    int damage = m_stacks;
    m_stacks--;
    return damage;
}

QString Poison::getDescription() const
{
    return QString("中毒：回合结束时受到%1点伤害，层数每回合-1").arg(m_stacks);
}
