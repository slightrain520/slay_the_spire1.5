#include "BuffManager.h"
#include <algorithm>

void BuffManager::addBuff(std::shared_ptr<Buff> buff)
{
    for (auto &existing : m_buffs) {
        if (existing->getType() == buff->getType()) {
            existing->addStacks(buff->getStacks());
            return;
        }
    }
    m_buffs.push_back(std::move(buff));
}

Buff* BuffManager::getBuff(BuffType type) const
{
    for (auto &b : m_buffs) {
        if (b->getType() == type) {
            return b.get();
        }
    }
    return nullptr;
}

bool BuffManager::hasBuff(BuffType type) const
{
    return getBuff(type) != nullptr;
}

int BuffManager::getStacks(BuffType type) const
{
    Buff* b = getBuff(type);
    return b ? b->getStacks() : 0;
}

int BuffManager::triggerTurnStart()
{
    int totalDamage = 0;
    for (auto &b : m_buffs) {
        totalDamage += b->onTurnStart();
    }
    removeExpired();
    return totalDamage;
}

int BuffManager::triggerTurnEnd()
{
    int totalDamage = 0;
    for (auto &b : m_buffs) {
        totalDamage += b->onTurnEnd();
    }
    removeExpired();
    return totalDamage;
}

int BuffManager::modifyOutgoingDamage(int baseDamage) const
{
    int damage = baseDamage;
    for (auto &b : m_buffs) {
        damage = b->modifyOutgoingDamage(damage);
    }
    return damage;
}

int BuffManager::modifyIncomingDamage(int baseDamage) const
{
    int damage = baseDamage;
    for (auto &b : m_buffs) {
        damage = b->modifyIncomingDamage(damage);
    }
    return damage;
}

void BuffManager::removeExpired()
{
    m_buffs.erase(
        std::remove_if(m_buffs.begin(), m_buffs.end(),
            [](const std::shared_ptr<Buff> &b) { return b->isExpired(); }),
        m_buffs.end()
    );
}

void BuffManager::clearAll()
{
    m_buffs.clear();
}
