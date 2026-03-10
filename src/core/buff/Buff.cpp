#include "Buff.h"

Buff::Buff(BuffType type, const QString &name, int stacks)
    : m_type(type)
    , m_name(name)
    , m_stacks(stacks)
{
}
