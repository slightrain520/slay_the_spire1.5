#include "SkillCard.h"

SkillCard::SkillCard(const QString &id, const QString &name, int cost,
                     const QString &description, TargetType targetType)
    : Card(id, name, cost, CardType::SKILL, targetType, description)
{
}

std::shared_ptr<Card> SkillCard::clone() const
{
    auto copy = std::make_shared<SkillCard>(m_id, m_name, m_cost, m_description, m_targetType);
    for (auto &effect : m_effects) {
        copy->addEffect(effect);
    }
    copy->setExhaust(m_exhaust);
    copy->setRepeat(m_repeat);
    copy->setInnate(m_innate);
    copy->setRetain(m_retain);
    return copy;
}
