#ifndef ATTACKCARD_H
#define ATTACKCARD_H

#include "Card.h"

/**
 * @brief 攻击牌
 *
 * 继承 Card，卡牌类型固定为 ATTACK。
 * 默认目标类型为 SINGLE_ENEMY，也可设为 ALL_ENEMIES。
 */
class AttackCard : public Card
{
public:
    /**
     * @param id          卡牌标识
     * @param name        名称
     * @param cost        能量消耗
     * @param description 描述
     * @param targetType  目标类型（默认单体）
     */
    AttackCard(const QString &id, const QString &name, int cost,
               const QString &description,
               TargetType targetType = TargetType::SINGLE_ENEMY);

    std::shared_ptr<Card> clone() const override;
};

#endif // ATTACKCARD_H
