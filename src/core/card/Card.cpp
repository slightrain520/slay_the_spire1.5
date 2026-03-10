#include "Card.h"

Card::Card(const QString &id, const QString &name, int cost,
           CardType cardType, TargetType targetType,
           const QString &description)
    : m_id(id)
    , m_name(name)
    , m_cost(cost)
    , m_cardType(cardType)
    , m_targetType(targetType)
    , m_description(description)
{
}

void Card::addEffect(std::shared_ptr<CardEffect> effect)
{
    m_effects.push_back(std::move(effect));
}

void Card::play(Character *source, Character *target, BattleManager *battle)
{
    for (auto &effect : m_effects) {
        effect->apply(source, target, battle);
    }
}
