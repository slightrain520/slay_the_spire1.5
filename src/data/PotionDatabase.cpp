#include "PotionDatabase.h"
#include "core/item/Potions.h"

PotionDatabase& PotionDatabase::instance()
{
    static PotionDatabase db;
    return db;
}

PotionDatabase::PotionDatabase()
{
    registerAll();
}

std::shared_ptr<Potion> PotionDatabase::createPotion(const QString &id) const
{
    if (m_templates.contains(id)) {
        return m_templates[id]->clone();
    }
    return nullptr;
}

QList<QString> PotionDatabase::getAllPotionIds() const
{
    return m_templates.keys();
}

void PotionDatabase::registerAll()
{
    m_templates["heal_potion"]         = std::make_shared<HealPotion>();
    m_templates["vulnerable_potion"]   = std::make_shared<VulnerablePotion>();
    m_templates["weak_potion"]         = std::make_shared<WeakPotion>();
    m_templates["explosive_potion"]    = std::make_shared<ExplosivePotion>();
    m_templates["shield_potion"]       = std::make_shared<ShieldPotion>();
    m_templates["double_shield_potion"]= std::make_shared<DoubleShieldPotion>();
}
