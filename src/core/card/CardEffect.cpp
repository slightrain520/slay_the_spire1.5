#include "CardEffect.h"
#include "core/character/Character.h"
#include "core/battle/BattleManager.h"
#include "core/buff/Buff.h"
#include "core/buff/Vulnerable.h"
#include "core/buff/Weak.h"
#include "core/buff/Poison.h"
#include "core/buff/Doom.h"
#include "core/buff/Strength.h"
#include <cmath>

// ─── DamageEffect ────────────────────────────────────────

DamageEffect::DamageEffect(int damage)
    : m_damage(damage)
{
}

void DamageEffect::apply(Character *source, Character *target, BattleManager *battle)
{
    Q_UNUSED(battle)
    if (!source || !target) return;

    int damage = m_damage;
    damage = source->getBuffManager().modifyOutgoingDamage(damage);
    damage = target->getBuffManager().modifyIncomingDamage(damage);
    if (damage < 0) damage = 0;

    target->takeDamage(damage);
}

QString DamageEffect::getDescription() const
{
    return QString("造成%1点伤害").arg(m_damage);
}

// ─── BlockEffect ─────────────────────────────────────────

BlockEffect::BlockEffect(int block)
    : m_block(block)
{
}

void BlockEffect::apply(Character *source, Character *target, BattleManager *battle)
{
    Q_UNUSED(target)
    Q_UNUSED(battle)
    if (!source) return;
    source->addBlock(m_block);
}

QString BlockEffect::getDescription() const
{
    return QString("获得%1点格挡").arg(m_block);
}

// ─── ApplyBuffEffect ─────────────────────────────────────

ApplyBuffEffect::ApplyBuffEffect(int buffType, int stacks)
    : m_buffType(buffType)
    , m_stacks(stacks)
{
}

void ApplyBuffEffect::apply(Character *source, Character *target, BattleManager *battle)
{
    Q_UNUSED(source)
    Q_UNUSED(battle)
    if (!target) return;

    std::shared_ptr<Buff> buff;
    switch (static_cast<BuffType>(m_buffType)) {
    case BuffType::VULNERABLE:
        buff = std::make_shared<Vulnerable>(m_stacks);
        break;
    case BuffType::WEAK:
        buff = std::make_shared<Weak>(m_stacks);
        break;
    case BuffType::POISON:
        buff = std::make_shared<Poison>(m_stacks);
        break;
    case BuffType::DOOM:
        buff = std::make_shared<Doom>(m_stacks);
        break;
    case BuffType::STRENGTH:
        buff = std::make_shared<Strength>(m_stacks);
        break;
    }
    if (buff) {
        target->getBuffManager().addBuff(buff);
    }
}

QString ApplyBuffEffect::getDescription() const
{
    QString buffName;
    switch (static_cast<BuffType>(m_buffType)) {
    case BuffType::VULNERABLE: buffName = "易伤"; break;
    case BuffType::WEAK:       buffName = "虚弱"; break;
    case BuffType::POISON:     buffName = "中毒"; break;
    case BuffType::DOOM:       buffName = "灾厄"; break;
    case BuffType::STRENGTH:   buffName = "力量"; break;
    }
    if (m_stacks > 0) {
        return QString("施加%1点%2").arg(m_stacks).arg(buffName);
    } else {
        return QString("降低%1点%2").arg(-m_stacks).arg(buffName);
    }
}

// ─── HealEffect ──────────────────────────────────────────

HealEffect::HealEffect(int amount)
    : m_amount(amount)
{
}

void HealEffect::apply(Character *source, Character *target, BattleManager *battle)
{
    Q_UNUSED(target)
    Q_UNUSED(battle)
    if (!source) return;
    source->heal(m_amount);
}

QString HealEffect::getDescription() const
{
    return QString("恢复%1点生命").arg(m_amount);
}

// ─── DrawCardEffect ──────────────────────────────────────

DrawCardEffect::DrawCardEffect(int count)
    : m_count(count)
{
}

void DrawCardEffect::apply(Character *source, Character *target, BattleManager *battle)
{
    Q_UNUSED(source)
    Q_UNUSED(target)
    if (battle) {
        battle->drawCards(m_count);
    }
}

QString DrawCardEffect::getDescription() const
{
    return QString("抽%1张牌").arg(m_count);
}

