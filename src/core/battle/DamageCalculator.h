#ifndef DAMAGECALCULATOR_H
#define DAMAGECALCULATOR_H

class Character;
class Inventory;

/**
 * @brief 伤害计算器
 *
 * 集中处理所有伤害计算逻辑：
 * - 攻击伤害（考虑易伤/虚弱修正）
 * - 中毒/荆棘等固定扣血（均经格挡优先抵扣）
 * - 遗物修正
 * - 死亡阻止（佛珠等遗物检查）
 */
class DamageCalculator
{
public:
    /**
     * @brief 计算攻击伤害（含 Buff 修正）
     * @param baseDamage 基础伤害值（卡牌数值）
     * @param source     攻击来源
     * @param target     攻击目标
     * @return 经 Buff 修正后的伤害值（未扣除格挡）
     */
    static int calculateAttackDamage(int baseDamage, Character *source, Character *target);

    /**
     * @brief 对目标执行扣血（格挡优先抵扣）
     *
     * 所有扣血效果统一调用此方法。
     * @param target    目标角色
     * @param amount    伤害值
     * @param inventory 玩家背包（用于遗物死亡阻止检查，可为 nullptr）
     */
    static void applyDamage(Character *target, int amount, Inventory *inventory = nullptr);

    /**
     * @brief 经遗物修正后的敌人伤害值
     * @param damage    原始伤害
     * @param inventory 玩家背包
     * @return 修正后伤害
     */
    static int applyRelicDamageModifiers(int damage, Inventory *inventory);

    /**
     * @brief 检查并处理死亡阻止（佛珠等）
     * @return true 表示死亡被阻止
     */
    static bool checkDeathPrevention(Character *target, Inventory *inventory);
};

#endif // DAMAGECALCULATOR_H
