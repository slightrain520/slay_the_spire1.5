#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <vector>
#include <QString>

/**
 * @brief 敌人意图（行动）类型
 */
enum class IntentType {
    ATTACK,       ///< 攻击
    DEFEND,       ///< 防御（给自己加格挡）
    BUFF_SELF,    ///< 给自己施加增益（目前未使用，预留扩展）
    DEBUFF_PLAYER ///< 给玩家施加减益
};

/**
 * @brief 敌人单回合行动描述
 *
 * 每回合敌人按预设的行动序列执行一个 Intent。
 */
struct EnemyIntent {
    IntentType type;     ///< 行动类型
    int value;           ///< 伤害值 / 格挡值 / Buff 层数
    int buffType;        ///< 当 type 为 DEBUFF_PLAYER 时，指定 BuffType 枚举值
    QString description; ///< 意图描述文字

    EnemyIntent()
        : type(IntentType::ATTACK), value(0), buffType(0) {}

    EnemyIntent(IntentType t, int v, const QString &desc = "")
        : type(t), value(v), buffType(0), description(desc) {}

    EnemyIntent(IntentType t, int v, int bt, const QString &desc = "")
        : type(t), value(v), buffType(bt), description(desc) {}
};

/**
 * @brief 敌人角色
 *
 * 不通过卡牌攻击，而是按预设行动序列逐回合执行。
 * 每回合开始时向玩家展示本回合意图。
 */
class Enemy : public Character
{
public:
    /**
     * @param name       敌人名称
     * @param maxHp      最大生命值
     * @param intents    预设行动序列
     * @param isElite    是否为精英怪
     */
    Enemy(const QString &name, int maxHp,
          const std::vector<EnemyIntent> &intents,
          bool isElite = false);
    ~Enemy() override = default;

    /**
     * @brief 获取当前回合的意图
     */
    const EnemyIntent& getCurrentIntent() const;

    /**
     * @brief 推进到下一个意图（回合结束时调用）
     */
    void advanceIntent();

    /**
     * @brief 执行当前意图的行动
     * @param player 目标玩家
     */
    void executeIntent(Character *player);

    bool isElite() const { return m_isElite; }

    /** @brief 获取完整意图列表 */
    const std::vector<EnemyIntent>& getIntentList() const { return m_intents; }

private:
    std::vector<EnemyIntent> m_intents;  ///< 预设行动序列
    int m_currentIntentIndex;            ///< 当前意图索引
    bool m_isElite;                      ///< 是否为精英怪
};

#endif // ENEMY_H
