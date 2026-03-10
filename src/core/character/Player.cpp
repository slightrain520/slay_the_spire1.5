#include "Player.h"

Player::Player(const QString &name, int maxHp, int gold, int maxEnergy)
    : Character(name, maxHp)
    , m_gold(gold)
    , m_energy(maxEnergy)
    , m_maxEnergy(maxEnergy)
{
}

bool Player::spendGold(int amount)
{
    if (m_gold < amount) return false;
    m_gold -= amount;
    return true;
}

bool Player::spendEnergy(int amount)
{
    if (m_energy < amount) return false;
    m_energy -= amount;
    return true;
}
