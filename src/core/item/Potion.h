#ifndef POTION_H
#define POTION_H

#include <QString>
#include <memory>
#include <vector>

class Character;

/**
 * @brief 药水目标类型
 */
enum class PotionTargetType {
    SELF,           ///< 对自身使用
    SINGLE_ENEMY,   ///< 对单个敌人使用
    ALL_ENEMIES     ///< 对所有敌人使用
};

/**
 * @brief 药水抽象基类
 *
 * 一次性消耗品，使用后即消失。
 */
class Potion
{
public:
    /**
     * @param id          唯一标识
     * @param name        名称
     * @param description 描述
     * @param targetType  目标类型
     */
    Potion(const QString &id, const QString &name,
           const QString &description, PotionTargetType targetType);
    virtual ~Potion() = default;

    /**
     * @brief 使用药水
     * @param user    使用者（玩家）
     * @param targets 目标角色列表（对己使用时为玩家自身）
     */
    virtual void use(Character *user, std::vector<Character*> targets) = 0;

    virtual std::shared_ptr<Potion> clone() const = 0;

    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    PotionTargetType getTargetType() const { return m_targetType; }

protected:
    QString m_id;
    QString m_name;
    QString m_description;
    PotionTargetType m_targetType;
};

#endif // POTION_H
