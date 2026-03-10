#include "Inventory.h"
#include "PassRelic.h"

void Inventory::addRelic(std::shared_ptr<Relic> relic)
{
    if (hasRelic(relic->getId())) return;  // 每件遗物最多获得一次
    m_relics.push_back(std::move(relic));
}

void Inventory::addRelic(std::shared_ptr<Relic> relic, Character *player)
{
    if (hasRelic(relic->getId())) return;
    relic->onPickup(player);
    // 通行证遗物：激活跳过精英标志
    if (auto *pass = dynamic_cast<PassRelic*>(relic.get())) {
        Q_UNUSED(pass)
        m_skipNextElite = true;
    }
    m_relics.push_back(std::move(relic));
}

bool Inventory::hasRelic(const QString &id) const
{
    for (const auto &r : m_relics) {
        if (r->getId() == id) return true;
    }
    return false;
}

void Inventory::resetRelicTurnStates()
{
    for (auto &r : m_relics) {
        r->resetTurnState();
    }
}

bool Inventory::addPotion(std::shared_ptr<Potion> potion)
{
    if (isPotionsFull()) return false;
    m_potions.push_back(std::move(potion));
    return true;
}

bool Inventory::usePotion(int index, Character *user, std::vector<Character*> targets)
{
    if (index < 0 || index >= static_cast<int>(m_potions.size())) return false;

    m_potions[index]->use(user, targets);
    m_potions.erase(m_potions.begin() + index);
    return true;
}

void Inventory::discardPotion(int index)
{
    if (index >= 0 && index < static_cast<int>(m_potions.size())) {
        m_potions.erase(m_potions.begin() + index);
    }
}
