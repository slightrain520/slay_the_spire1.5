#include "EnemyDatabase.h"
#include "core/buff/Buff.h"

EnemyDatabase& EnemyDatabase::instance()
{
    static EnemyDatabase db;
    return db;
}

EnemyDatabase::EnemyDatabase()
{
    registerAll();
}

std::shared_ptr<Enemy> EnemyDatabase::createEnemy(const QString &id) const
{
    if (!m_templates.contains(id)) return nullptr;
    const auto &t = m_templates[id];
    return std::make_shared<Enemy>(t.name, t.maxHp, t.intents, t.isElite);
}

std::vector<QString> EnemyDatabase::getNormalEnemyIds(int tier) const
{
    std::vector<QString> result;
    for (auto it = m_templates.begin(); it != m_templates.end(); ++it) {
        if (it->tier == tier) {
            result.push_back(it.key());
        }
    }
    return result;
}

std::vector<QString> EnemyDatabase::getEliteEnemyIds() const
{
    std::vector<QString> result;
    for (auto it = m_templates.begin(); it != m_templates.end(); ++it) {
        if (it->tier == 3) {
            result.push_back(it.key());
        }
    }
    return result;
}

std::vector<QString> EnemyDatabase::getBossIds() const
{
    std::vector<QString> result;
    for (auto it = m_templates.begin(); it != m_templates.end(); ++it) {
        if (it->tier == 4) {
            result.push_back(it.key());
        }
    }
    return result;
}

void EnemyDatabase::registerAll()
{
    // ═══════════════════════════════════════════════════
    // Tier 1: 低级普通怪
    // ═══════════════════════════════════════════════════

    m_templates["slime"] = {
        "史莱姆", 30,
        {
            {IntentType::ATTACK, 6, "攻击 6"},
            {IntentType::DEFEND, 5, "防御 5"},
            {IntentType::ATTACK, 8, "攻击 8"},
        },
        false, 1
    };

    m_templates["goblin"] = {
        "哥布林", 25,
        {
            {IntentType::ATTACK, 8, "攻击 8"},
            {IntentType::ATTACK, 5, "攻击 5"},
            {IntentType::ATTACK, 10, "攻击 10"},
        },
        false, 1
    };

    m_templates["skeleton"] = {
        "骷髅兵", 28,
        {
            {IntentType::ATTACK, 7, "攻击 7"},
            {IntentType::ATTACK, 7, "攻击 7"},
            {IntentType::DEFEND, 8, "防御 8"},
        },
        false, 1
    };

    // ═══════════════════════════════════════════════════
    // Tier 2: 中级普通怪
    // ═══════════════════════════════════════════════════

    m_templates["dark_elf"] = {
        "暗精灵", 40,
        {
            {IntentType::ATTACK, 10, "攻击 10"},
            {IntentType::DEBUFF_PLAYER, 2, static_cast<int>(BuffType::WEAK), "施加2层虚弱"},
            {IntentType::ATTACK, 14, "攻击 14"},
            {IntentType::DEFEND, 10, "防御 10"},
        },
        false, 2
    };

    m_templates["mushroom"] = {
        "毒蘑菇", 35,
        {
            {IntentType::DEBUFF_PLAYER, 3, static_cast<int>(BuffType::POISON), "施加3层中毒"},
            {IntentType::ATTACK, 8, "攻击 8"},
            {IntentType::DEBUFF_PLAYER, 4, static_cast<int>(BuffType::POISON), "施加4层中毒"},
            {IntentType::ATTACK, 10, "攻击 10"},
        },
        false, 2
    };

    m_templates["stone_golem"] = {
        "石像鬼", 50,
        {
            {IntentType::DEFEND, 12, "防御 12"},
            {IntentType::ATTACK, 15, "攻击 15"},
            {IntentType::ATTACK, 10, "攻击 10"},
            {IntentType::DEFEND, 8, "防御 8"},
        },
        false, 2
    };

    // ═══════════════════════════════════════════════════
    // Tier 3: 精英怪
    // ═══════════════════════════════════════════════════

    m_templates["elite_knight"] = {
        "精英骑士", 65,
        {
            {IntentType::DEFEND, 10, "防御 10"},
            {IntentType::ATTACK, 18, "攻击 18"},
            {IntentType::DEBUFF_PLAYER, 2, static_cast<int>(BuffType::VULNERABLE), "施加2层易伤"},
            {IntentType::ATTACK, 22, "攻击 22"},
            {IntentType::DEFEND, 15, "防御 15"},
        },
        true, 3
    };

    m_templates["elite_assassin"] = {
        "暗影刺客", 55,
        {
            {IntentType::DEBUFF_PLAYER, 3, static_cast<int>(BuffType::POISON), "施加3层中毒"},
            {IntentType::ATTACK, 14, "攻击 14"},
            {IntentType::DEBUFF_PLAYER, 2, static_cast<int>(BuffType::WEAK), "施加2层虚弱"},
            {IntentType::ATTACK, 20, "攻击 20"},
        },
        true, 3
    };

    // 战场霸主 - 具有力量自增效果的精英怪
    m_templates["warlord"] = {
        "战场霸主", 70,
        {
            {IntentType::BUFF_SELF, 2, static_cast<int>(BuffType::STRENGTH), "蓄力：获得2点力量"},
            {IntentType::ATTACK, 16, "攻击 16"},
            {IntentType::ATTACK, 20, "攻击 20"},
            {IntentType::BUFF_SELF, 2, static_cast<int>(BuffType::STRENGTH), "蓄力：获得2点力量"},
            {IntentType::DEFEND, 12, "防御 12"},
            {IntentType::ATTACK, 18, "攻击 18"},
        },
        true, 3
    };

    // ═══════════════════════════════════════════════════
    // Tier 4: Boss
    // ═══════════════════════════════════════════════════

    m_templates["dragon"] = {
        "巨龙", 100,
        {
            {IntentType::DEFEND, 15, "防御 15"},
            {IntentType::ATTACK, 20, "攻击 20"},
            {IntentType::DEBUFF_PLAYER, 3, static_cast<int>(BuffType::VULNERABLE), "施加3层易伤"},
            {IntentType::ATTACK, 30, "攻击 30"},
            {IntentType::DEFEND, 20, "防御 20"},
            {IntentType::ATTACK, 25, "攻击 25"},
        },
        false, 4
    };

    m_templates["lich_king"] = {
        "巫妖王", 90,
        {
            {IntentType::DEBUFF_PLAYER, 5, static_cast<int>(BuffType::POISON), "施加5层中毒"},
            {IntentType::ATTACK, 15, "攻击 15"},
            {IntentType::DEBUFF_PLAYER, 6, static_cast<int>(BuffType::DOOM), "施加6层灾厄"},
            {IntentType::ATTACK, 20, "攻击 20"},
            {IntentType::DEBUFF_PLAYER, 3, static_cast<int>(BuffType::WEAK), "施加3层虚弱"},
            {IntentType::ATTACK, 25, "攻击 25"},
        },
        false, 4
    };
}
