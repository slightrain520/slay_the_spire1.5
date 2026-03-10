#ifndef CARDEFFECT_H
#define CARDEFFECT_H

#include <QString>
#include <memory>

class Character;
class BattleManager;

/**
 * @brief 卡牌效果抽象基类（策略模式）
 *
 * 每张卡牌可组合多个 CardEffect 实例。
 * 使用卡牌时依次执行所有效果的 apply() 方法。
 */
class CardEffect
{
public:
    virtual ~CardEffect() = default;

    /**
     * @brief 执行此效果
     * @param source  效果来源（出牌者）
     * @param target  效果目标
     * @param battle  战斗管理器（用于抽牌等全局操作）
     */
    virtual void apply(Character *source, Character *target, BattleManager *battle) = 0;

    /**
     * @brief 获取效果描述文字
     */
    virtual QString getDescription() const = 0;
};

/**
 * @brief 造成伤害效果
 *
 * 对目标造成指定点数的伤害（受易伤/虚弱等状态修正）。
 */
class DamageEffect : public CardEffect
{
public:
    /**
     * @param damage 基础伤害值
     */
    explicit DamageEffect(int damage);

    void apply(Character *source, Character *target, BattleManager *battle) override;
    QString getDescription() const override;

    int getDamage() const { return m_damage; }

private:
    int m_damage;  ///< 基础伤害值
};

/**
 * @brief 增加格挡效果
 *
 * 为来源角色增加指定点数的格挡。
 */
class BlockEffect : public CardEffect
{
public:
    /**
     * @param block 格挡值
     */
    explicit BlockEffect(int block);

    void apply(Character *source, Character *target, BattleManager *battle) override;
    QString getDescription() const override;

private:
    int m_block;  ///< 格挡值
};

/**
 * @brief 施加 Buff 效果
 *
 * 对目标施加指定类型和层数的状态效果。
 */
class ApplyBuffEffect : public CardEffect
{
public:
    /**
     * @param buffType  Buff 类型（使用 BuffType 枚举的 int 值）
     * @param stacks    层数
     */
    ApplyBuffEffect(int buffType, int stacks);

    void apply(Character *source, Character *target, BattleManager *battle) override;
    QString getDescription() const override;

private:
    int m_buffType;  ///< BuffType 枚举值
    int m_stacks;    ///< 施加层数
};

/**
 * @brief 恢复生命效果
 */
class HealEffect : public CardEffect
{
public:
    /**
     * @param amount 恢复的生命值
     */
    explicit HealEffect(int amount);

    void apply(Character *source, Character *target, BattleManager *battle) override;
    QString getDescription() const override;

private:
    int m_amount;  ///< 恢复量
};

/**
 * @brief 抽牌效果
 */
class DrawCardEffect : public CardEffect
{
public:
    /**
     * @param count 抽取的牌数
     */
    explicit DrawCardEffect(int count);

    void apply(Character *source, Character *target, BattleManager *battle) override;
    QString getDescription() const override;

private:
    int m_count;  ///< 抽牌数
};

#endif // CARDEFFECT_H
