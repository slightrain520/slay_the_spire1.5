#ifndef SKILLCARD_H
#define SKILLCARD_H

#include "Card.h"

/**
 * @brief 技能牌
 *
 * 继承 Card，卡牌类型固定为 SKILL。
 * 可作用于自身或敌方，根据具体效果而定。
 */
class SkillCard : public Card
{
public:
    /**
     * @param id          卡牌标识
     * @param name        名称
     * @param cost        能量消耗
     * @param description 描述
     * @param targetType  目标类型（默认自身）
     */
    SkillCard(const QString &id, const QString &name, int cost,
              const QString &description,
              TargetType targetType = TargetType::SELF);

    std::shared_ptr<Card> clone() const override;
};

#endif // SKILLCARD_H
