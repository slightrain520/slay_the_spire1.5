#include "Potions.h"
#include "core/character/Character.h"
#include "core/buff/Vulnerable.h"
#include "core/buff/Weak.h"
#include "data/GameConfig.h"
#include <cmath>

// ─── HealPotion ──────────────────────────────────────────

HealPotion::HealPotion()
    : Potion("heal_potion", "恢复药水", "恢复10%最大生命值", PotionTargetType::SELF)
{
}

void HealPotion::use(Character *user, std::vector<Character*> targets)
{
    Q_UNUSED(targets)
    if (!user) return;
    int healAmount = static_cast<int>(std::floor(user->getMaxHp() * GameConfig::HEAL_POTION_RATIO));
    user->heal(healAmount);
}

std::shared_ptr<Potion> HealPotion::clone() const
{
    return std::make_shared<HealPotion>();
}

// ─── VulnerablePotion ────────────────────────────────────

VulnerablePotion::VulnerablePotion()
    : Potion("vulnerable_potion", "易伤药水", "给予敌人3层易伤", PotionTargetType::SINGLE_ENEMY)
{
}

void VulnerablePotion::use(Character *user, std::vector<Character*> targets)
{
    Q_UNUSED(user)
    for (auto *target : targets) {
        if (target) {
            target->getBuffManager().addBuff(std::make_shared<Vulnerable>(3));
        }
    }
}

std::shared_ptr<Potion> VulnerablePotion::clone() const
{
    return std::make_shared<VulnerablePotion>();
}

// ─── WeakPotion ──────────────────────────────────────────

WeakPotion::WeakPotion()
    : Potion("weak_potion", "虚弱药水", "给予敌人3层虚弱", PotionTargetType::SINGLE_ENEMY)
{
}

void WeakPotion::use(Character *user, std::vector<Character*> targets)
{
    Q_UNUSED(user)
    for (auto *target : targets) {
        if (target) {
            target->getBuffManager().addBuff(std::make_shared<Weak>(3));
        }
    }
}

std::shared_ptr<Potion> WeakPotion::clone() const
{
    return std::make_shared<WeakPotion>();
}

// ─── ExplosivePotion ─────────────────────────────────────

ExplosivePotion::ExplosivePotion()
    : Potion("explosive_potion", "爆炸药水", "对所有敌人造成20点伤害", PotionTargetType::ALL_ENEMIES)
{
}

void ExplosivePotion::use(Character *user, std::vector<Character*> targets)
{
    Q_UNUSED(user)
    for (auto *target : targets) {
        if (target) {
            target->takeDamage(20);
        }
    }
}

std::shared_ptr<Potion> ExplosivePotion::clone() const
{
    return std::make_shared<ExplosivePotion>();
}

// ─── ShieldPotion ────────────────────────────────────────

ShieldPotion::ShieldPotion()
    : Potion("shield_potion", "护盾药水", "为自己增加15点格挡", PotionTargetType::SELF)
{
}

void ShieldPotion::use(Character *user, std::vector<Character*> targets)
{
    Q_UNUSED(targets)
    if (user) {
        user->addBlock(15);
    }
}

std::shared_ptr<Potion> ShieldPotion::clone() const
{
    return std::make_shared<ShieldPotion>();
}

// ─── DoubleShieldPotion ──────────────────────────────────

DoubleShieldPotion::DoubleShieldPotion()
    : Potion("double_shield_potion", "倍盾药水", "当前格挡值翻倍", PotionTargetType::SELF)
{
}

void DoubleShieldPotion::use(Character *user, std::vector<Character*> targets)
{
    Q_UNUSED(targets)
    if (user) {
        user->addBlock(user->getBlock());
    }
}

std::shared_ptr<Potion> DoubleShieldPotion::clone() const
{
    return std::make_shared<DoubleShieldPotion>();
}
