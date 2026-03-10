#include "CardDatabase.h"
#include "core/card/AttackCard.h"
#include "core/card/SkillCard.h"
#include "core/card/CardEffect.h"
#include "core/buff/Buff.h"

CardDatabase& CardDatabase::instance()
{
    static CardDatabase db;
    return db;
}

CardDatabase::CardDatabase()
{
    registerAll();
}

std::shared_ptr<Card> CardDatabase::createCard(const QString &id) const
{
    if (m_templates.contains(id)) {
        return m_templates[id]->clone();
    }
    return nullptr;
}

void CardDatabase::registerCard(std::shared_ptr<Card> card)
{
    m_templates[card->getId()] = card;
}

QList<QString> CardDatabase::getAllCardIds() const
{
    return m_templates.keys();
}

void CardDatabase::registerAll()
{
    // ═══════════════════════════════════════════════════
    // 通用卡牌（两个角色共享）
    // ═══════════════════════════════════════════════════

    // 打击 - 基础攻击牌
    {
        auto card = std::make_shared<AttackCard>("strike", "打击", 1, "造成6点伤害");
        card->addEffect(std::make_shared<DamageEffect>(6));
        registerCard(card);
    }

    // 防御 - 基础技能牌
    {
        auto card = std::make_shared<SkillCard>("defend", "防御", 1, "获得5点格挡");
        card->addEffect(std::make_shared<BlockEffect>(5));
        registerCard(card);
    }

    // ═══════════════════════════════════════════════════
    // 战士特有卡牌（易伤/虚弱方向）
    // ═══════════════════════════════════════════════════

    // 重击 - 高伤害攻击
    {
        auto card = std::make_shared<AttackCard>("heavy_blow", "重击", 2, "造成12点伤害");
        card->addEffect(std::make_shared<DamageEffect>(12));
        registerCard(card);
    }

    // 猛击 - 攻击并施加易伤
    {
        auto card = std::make_shared<AttackCard>("bash", "猛击", 2, "造成8点伤害，施加2层易伤");
        card->addEffect(std::make_shared<DamageEffect>(8));
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::VULNERABLE), 2));
        registerCard(card);
    }

    // 战吼 - 施加虚弱
    {
        auto card = std::make_shared<SkillCard>("war_cry", "战吼", 1, "施加2层虚弱",
                                                 TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::WEAK), 2));
        registerCard(card);
    }

    // 铁壁 - 高格挡
    {
        auto card = std::make_shared<SkillCard>("iron_wall", "铁壁", 2, "获得12点格挡");
        card->addEffect(std::make_shared<BlockEffect>(12));
        registerCard(card);
    }

    // 旋风斩 - 全体攻击
    {
        auto card = std::make_shared<AttackCard>("whirlwind", "旋风斩", 2, "对所有敌人造成7点伤害",
                                                  TargetType::ALL_ENEMIES);
        card->addEffect(std::make_shared<DamageEffect>(7));
        registerCard(card);
    }

    // 战斗姿态 - 格挡+抽牌
    {
        auto card = std::make_shared<SkillCard>("battle_stance", "战斗姿态", 1, "获得6点格挡，抽1张牌");
        card->addEffect(std::make_shared<BlockEffect>(6));
        card->addEffect(std::make_shared<DrawCardEffect>(1));
        registerCard(card);
    }

    // 将军命令 - 增加自身力量
    {
        auto card = std::make_shared<SkillCard>("warcry_power", "将军命令", 1,
            "获徙2点力量", TargetType::SELF);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::STRENGTH), 2));
        registerCard(card);
    }

    // 战斗怒吼 - 增加自身力量
    {
        auto card = std::make_shared<SkillCard>("battle_roar", "战斗怒吼", 2,
            "获徙4点力量，获徙4点格挡", TargetType::SELF);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::STRENGTH), 4));
        card->addEffect(std::make_shared<BlockEffect>(4));
        registerCard(card);
    }

    // 蚱蟀记 - 降低敌人力量
    {
        auto card = std::make_shared<SkillCard>("corrode", "蚱蟀", 1,
            "降低敌人2点力量", TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::STRENGTH), -2));
        registerCard(card);
    }

    // 强化打击 - 确保有力量时更强
    {
        auto card = std::make_shared<AttackCard>("power_strike", "强力冲击", 2, "造抅10点伤害，自身获徙1点力量");
        card->addEffect(std::make_shared<DamageEffect>(10));
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::STRENGTH), 1));
        registerCard(card);
    }

    // ═══════════════════════════════════════════════════
    // 女王特有卡牌（中毒/灾厄方向）
    // ═══════════════════════════════════════════════════

    // 毒刺 - 攻击并施加中毒
    {
        auto card = std::make_shared<AttackCard>("poison_stab", "毒刺", 1, "造成4点伤害，施加3层中毒");
        card->addEffect(std::make_shared<DamageEffect>(4));
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::POISON), 3));
        registerCard(card);
    }

    // 剧毒 - 纯中毒
    {
        auto card = std::make_shared<SkillCard>("deadly_poison", "剧毒", 1, "施加5层中毒",
                                                 TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::POISON), 5));
        registerCard(card);
    }

    // 诅咒 - 施加灾厄
    {
        auto card = std::make_shared<SkillCard>("curse", "诅咒", 2, "施加8层灾厄",
                                                 TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::DOOM), 8));
        registerCard(card);
    }

    // 暗影打击 - 攻击+灾厄
    {
        auto card = std::make_shared<AttackCard>("shadow_strike", "暗影打击", 2, "造成6点伤害，施加4层灾厄");
        card->addEffect(std::make_shared<DamageEffect>(6));
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::DOOM), 4));
        registerCard(card);
    }

    // 毒雾 - 全体中毒
    {
        auto card = std::make_shared<SkillCard>("poison_mist", "毒雾", 2, "对所有敌人施加3层中毒",
                                                 TargetType::ALL_ENEMIES);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::POISON), 3));
        registerCard(card);
    }

    // 暗影护盾 - 格挡+抽牌
    {
        auto card = std::make_shared<SkillCard>("shadow_shield", "暗影护盾", 1, "获得5点格挡，抽1张牌");
        card->addEffect(std::make_shared<BlockEffect>(5));
        card->addEffect(std::make_shared<DrawCardEffect>(1));
        registerCard(card);
    }

    // 阴影迫与 - 降低敌人力量
    {
        auto card = std::make_shared<SkillCard>("enervate", "阴影迫与", 1,
            "降低敌人2点力量，施加2层虚弱", TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::STRENGTH), -2));
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::WEAK), 2));
        registerCard(card);
    }

    // ═══════════════════════════════════════════════════
    // 关键字卡牌 - 消耗系列
    // ═══════════════════════════════════════════════════

    // 绝地反击 - 消耗：大量格挡
    {
        auto card = std::make_shared<SkillCard>("last_stand", "绝地反击", 1,
            "【消耗】获得18点格挡");
        card->addEffect(std::make_shared<BlockEffect>(18));
        card->setExhaust(true);
        registerCard(card);
    }

    // 爆发 - 消耗：高伤害单体
    {
        auto card = std::make_shared<AttackCard>("burst", "爆发", 2,
            "【消耗】造成20点伤害");
        card->addEffect(std::make_shared<DamageEffect>(20));
        card->setExhaust(true);
        registerCard(card);
    }

    // 涅槃 - 消耗：治疗
    {
        auto card = std::make_shared<SkillCard>("nirvana", "涅槃", 1,
            "【消耗】恢复8点生命值", TargetType::SELF);
        card->addEffect(std::make_shared<HealEffect>(8));
        card->setExhaust(true);
        registerCard(card);
    }

    // ═══════════════════════════════════════════════════
    // 关键字卡牌 - 重放系列
    // ═══════════════════════════════════════════════════

    // 连斩 - 重放2：连续攻击
    {
        auto card = std::make_shared<AttackCard>("triple_slash", "连斩", 2,
            "【重放2】造成5点伤害（共15点）");
        card->addEffect(std::make_shared<DamageEffect>(5));
        card->setRepeat(2);
        registerCard(card);
    }

    // 剧毒雨 - 重放1：双次中毒
    {
        auto card = std::make_shared<SkillCard>("poison_rain", "毒雨", 1,
            "【重放1】施加3层中毒（共6层）", TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::POISON), 3));
        card->setRepeat(1);
        registerCard(card);
    }

    // ═══════════════════════════════════════════════════
    // 关键字卡牌 - 固有系列
    // ═══════════════════════════════════════════════════

    // 先手防御 - 固有：战斗一开始就有格挡
    {
        auto card = std::make_shared<SkillCard>("vanguard", "先手防御", 1,
            "【固有】获得8点格挡");
        card->addEffect(std::make_shared<BlockEffect>(8));
        card->setInnate(true);
        registerCard(card);
    }

    // 战前誓言 - 固有+消耗：战斗开始必获3力量
    {
        auto card = std::make_shared<SkillCard>("war_oath", "战前誓言", 1,
            "【固有】【消耗】获得3点力量", TargetType::SELF);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::STRENGTH), 3));
        card->setInnate(true);
        card->setExhaust(true);
        registerCard(card);
    }

    // ═══════════════════════════════════════════════════
    // 关键字卡牌 - 保留系列
    // ═══════════════════════════════════════════════════

    // 防御阵型 - 保留：格挡持续到下回合
    {
        auto card = std::make_shared<SkillCard>("fortify", "防御阵型", 1,
            "【保留】获得7点格挡（不在回合结束时弃牌）");
        card->addEffect(std::make_shared<BlockEffect>(7));
        card->setRetain(true);
        registerCard(card);
    }

    // 潜伏 - 保留：等待时机的毒
    {
        auto card = std::make_shared<SkillCard>("lurk", "潜伏", 0,
            "【保留】施加2层中毒（不在回合结束时弃牌）", TargetType::SINGLE_ENEMY);
        card->addEffect(std::make_shared<ApplyBuffEffect>(static_cast<int>(BuffType::POISON), 2));
        card->setRetain(true);
        registerCard(card);
    }
}

std::vector<QString> CardDatabase::getWarriorStarterDeck() const
{
    return {
        "strike", "strike", "strike", "strike", "strike",
        "defend", "defend", "defend", "defend",
        "bash"
    };
}

std::vector<QString> CardDatabase::getQueenStarterDeck() const
{
    return {
        "strike", "strike", "strike", "strike",
        "defend", "defend", "defend", "defend",
        "poison_stab", "deadly_poison"
    };
}
