#ifndef POTIONS_H
#define POTIONS_H

#include "Potion.h"

/**
 * @brief 恢复药水 - 恢复10%最大生命值
 */
class HealPotion : public Potion
{
public:
    HealPotion();
    void use(Character *user, std::vector<Character*> targets) override;
    std::shared_ptr<Potion> clone() const override;
};

/**
 * @brief 易伤药水 - 给予敌人3层易伤
 */
class VulnerablePotion : public Potion
{
public:
    VulnerablePotion();
    void use(Character *user, std::vector<Character*> targets) override;
    std::shared_ptr<Potion> clone() const override;
};

/**
 * @brief 虚弱药水 - 给予敌人3层虚弱
 */
class WeakPotion : public Potion
{
public:
    WeakPotion();
    void use(Character *user, std::vector<Character*> targets) override;
    std::shared_ptr<Potion> clone() const override;
};

/**
 * @brief 爆炸药水 - 对所有敌人造成20点伤害
 */
class ExplosivePotion : public Potion
{
public:
    ExplosivePotion();
    void use(Character *user, std::vector<Character*> targets) override;
    std::shared_ptr<Potion> clone() const override;
};

/**
 * @brief 护盾药水 - 为自己增加15点格挡
 */
class ShieldPotion : public Potion
{
public:
    ShieldPotion();
    void use(Character *user, std::vector<Character*> targets) override;
    std::shared_ptr<Potion> clone() const override;
};

/**
 * @brief 倍盾药水 - 当前格挡值翻倍
 */
class DoubleShieldPotion : public Potion
{
public:
    DoubleShieldPotion();
    void use(Character *user, std::vector<Character*> targets) override;
    std::shared_ptr<Potion> clone() const override;
};

#endif // POTIONS_H
