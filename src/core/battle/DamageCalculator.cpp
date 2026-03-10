#include "DamageCalculator.h"
#include "core/character/Character.h"
#include "core/item/Inventory.h"
#include <cmath>

int DamageCalculator::calculateAttackDamage(int baseDamage, Character *source, Character *target)
{
    int damage = baseDamage;

    if (source) {
        damage = source->getBuffManager().modifyOutgoingDamage(damage);
    }
    if (target) {
        damage = target->getBuffManager().modifyIncomingDamage(damage);
    }

    return std::max(0, damage);
}

void DamageCalculator::applyDamage(Character *target, int amount, Inventory *inventory)
{
    if (!target || amount <= 0) return;

    target->takeDamage(amount);

    if (target->isDead() && inventory) {
        checkDeathPrevention(target, inventory);
    }
}

int DamageCalculator::applyRelicDamageModifiers(int damage, Inventory *inventory)
{
    if (!inventory) return damage;

    int modified = damage;
    for (auto &relic : inventory->getRelics()) {
        modified = relic->modifyEnemyDamage(modified);
    }
    return std::max(0, modified);
}

bool DamageCalculator::checkDeathPrevention(Character *target, Inventory *inventory)
{
    if (!target || !inventory || !target->isDead()) return false;

    for (auto &relic : inventory->getRelics()) {
        if (relic->onAboutToDie(target)) {
            return true;
        }
    }
    return false;
}
