#ifndef CARD_H
#define CARD_H

#include <QString>
#include <vector>
#include <memory>
#include "CardEffect.h"

class Character;
class BattleManager;

/**
 * @brief 卡牌目标类型枚举
 */
enum class TargetType {
    SELF,           ///< 作用于自身（格挡、恢复等）
    SINGLE_ENEMY,   ///< 作用于单个敌人（攻击、debuff）
    ALL_ENEMIES     ///< 作用于所有敌人
};

/**
 * @brief 卡牌类型枚举
 */
enum class CardType {
    ATTACK,  ///< 攻击牌
    SKILL    ///< 技能牌
};

/**
 * @brief 卡牌抽象基类
 *
 * 每张卡牌拥有名称、费用、描述、目标类型和一组效果。
 * 使用时依次执行所有效果。
 */
class Card
{
public:
    /**
     * @param id          卡牌唯一标识
     * @param name        显示名称
     * @param cost        能量消耗
     * @param cardType    卡牌类型（攻击/技能）
     * @param targetType  目标类型
     * @param description 描述文字
     */
    Card(const QString &id, const QString &name, int cost,
         CardType cardType, TargetType targetType,
         const QString &description);
    virtual ~Card() = default;

    /** @brief 添加一个效果到卡牌 */
    void addEffect(std::shared_ptr<CardEffect> effect);

    /**
     * @brief 使用卡牌，依次执行所有效果
     * @param source 出牌者
     * @param target 目标角色
     * @param battle 战斗管理器
     */
    virtual void play(Character *source, Character *target, BattleManager *battle);

    /**
     * @brief 创建此卡牌的副本（深拷贝效果列表）
     */
    virtual std::shared_ptr<Card> clone() const = 0;

    // ─── Getters ──────────────────────────────────
    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    int getCost() const { return m_cost; }
    CardType getCardType() const { return m_cardType; }
    TargetType getTargetType() const { return m_targetType; }
    QString getDescription() const { return m_description; }
    const std::vector<std::shared_ptr<CardEffect>>& getEffects() const { return m_effects; }

    // ─── 关键字属性 ────────────────────────────────
    /** 消耗：打出后（或留在手牌至回合结束时）进入消耗堆，不再循环 */
    bool isExhaust() const { return m_exhaust; }
    void setExhaust(bool v) { m_exhaust = v; }

    /** 重放：打出时额外重复执行效果 N 次（总共 1+N 次） */
    int getRepeat() const { return m_repeat; }
    void setRepeat(int v) { m_repeat = v; }

    /** 固有：每场战斗第一手牌必然包含此牌 */
    bool isInnate() const { return m_innate; }
    void setInnate(bool v) { m_innate = v; }

    /** 保留：回合结束时留在手牌不入弃牌堆 */
    bool isRetain() const { return m_retain; }
    void setRetain(bool v) { m_retain = v; }

protected:
    QString  m_id;           ///< 唯一标识
    QString  m_name;         ///< 显示名称
    int      m_cost;         ///< 能量消耗
    CardType m_cardType;     ///< 攻击 / 技能
    TargetType m_targetType; ///< 目标类型
    QString  m_description;  ///< 描述文字
    std::vector<std::shared_ptr<CardEffect>> m_effects;  ///< 效果列表

    bool m_exhaust = false;  ///< 消耗关键字
    int  m_repeat  = 0;      ///< 重放次数（额外）
    bool m_innate  = false;  ///< 固有关键字
    bool m_retain  = false;  ///< 保留关键字
};

#endif // CARD_H
