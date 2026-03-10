#include "Character.h"
#include <algorithm>

Character::Character(const QString &name, int maxHp)
    : m_name(name)
    , m_hp(maxHp)
    , m_maxHp(maxHp)
    , m_block(0)
{
}

void Character::takeDamage(int amount)
{
    if (amount <= 0) return;

    if (m_block >= amount) {
        m_block -= amount;
    } else {
        amount -= m_block;
        m_block = 0;
        m_hp -= amount;
        if (m_hp < 0) m_hp = 0;
    }
}

void Character::heal(int amount)
{
    m_hp = std::min(m_hp + amount, m_maxHp);
}

void Character::addBlock(int amount)
{
    m_block += amount;
}

void Character::setHp(int hp)
{
    m_hp = std::clamp(hp, 0, m_maxHp);
}
