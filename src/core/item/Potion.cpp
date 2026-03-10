#include "Potion.h"

Potion::Potion(const QString &id, const QString &name,
               const QString &description, PotionTargetType targetType)
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_targetType(targetType)
{
}
