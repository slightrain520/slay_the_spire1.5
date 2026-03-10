#ifndef RELIC_H
#define RELIC_H

#include <QString>
#include <memory>

class Character;
class Card;

/**
 * @brief 遗物抽象基类
 *
 * 遗物通过钩子机制在战斗的关键时机触发特殊效果。
 * 子类重写需要的钩子方法实现具体逻辑。
 */
class Relic
{
public:
    /**
     * @param id          唯一标识
     * @param name        显示名称
     * @param description 效果描述
     */
    Relic(const QString &id, const QString &name, const QString &description);
    virtual ~Relic() = default;

    // ─── 战斗钩子（子类按需重写） ──────────────────

    /** @brief 战斗开始时触发（每场战斗触发一次，用于重置战斗内部状态）*/
    virtual void onBattleStart(Character *player) { Q_UNUSED(player) }

    /** @brief 获得遗物时触发（用于即时效果如满血、提升上限）*/
    virtual void onPickup(Character *player) { Q_UNUSED(player) }

    /** @brief 回合开始时触发 */
    virtual void onTurnStart(Character *player) { Q_UNUSED(player) }

    /** @brief 回合结束时触发 */
    virtual void onTurnEnd(Character *player) { Q_UNUSED(player) }

    /** @brief 打出卡牌时触发 */
    virtual void onCardPlayed(Character *player, const Card *card) { Q_UNUSED(player) Q_UNUSED(card) }

    /** @brief 受到伤害时触发 */
    virtual void onDamageTaken(Character *player, int damage) { Q_UNUSED(player) Q_UNUSED(damage) }

    /**
     * @brief 即将死亡时触发（HP<=0）
     * @return true 表示阻止了死亡（如佛珠恢复HP）
     */
    virtual bool onAboutToDie(Character *player) { Q_UNUSED(player) return false; }

    /**
     * @brief 修改敌人回合执行攻击前的伤害值
     * @param damage 原始伤害值
     * @return 修改后的伤害值
     */
    virtual int modifyEnemyDamage(int damage) { return damage; }

    /**
     * @brief 修改回合初始能量
     * @param energy 原始能量值
     * @return 修改后的能量值
     */
    virtual int modifyEnergy(int energy) { return energy; }

    /** @brief 每回合开始时重置遗物内部状态（如出牌计数） */
    virtual void resetTurnState() {}

    // ─── Getters ──────────────────────────────────
    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }

    virtual std::shared_ptr<Relic> clone() const = 0;

protected:
    QString m_id;
    QString m_name;
    QString m_description;
};

#endif // RELIC_H
