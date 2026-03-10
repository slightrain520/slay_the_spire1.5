#include "Enemy.h"
#include "core/buff/Buff.h"
#include "core/buff/Vulnerable.h"
#include "core/buff/Weak.h"
#include "core/buff/Poison.h"
#include "core/buff/Doom.h"
#include "core/buff/Strength.h"

Enemy::Enemy(const QString &name, int maxHp,
             const std::vector<EnemyIntent> &intents, bool isElite)
    : Character(name, maxHp)
    , m_intents(intents)
    , m_currentIntentIndex(0)
    , m_isElite(isElite)
{
}

const EnemyIntent& Enemy::getCurrentIntent() const
{
    return m_intents[m_currentIntentIndex];
}

void Enemy::advanceIntent()
{
    m_currentIntentIndex++;
    if (m_currentIntentIndex >= static_cast<int>(m_intents.size())) {
        m_currentIntentIndex = 0;
    }
}

void Enemy::executeIntent(Character *player)
{
    if (!player) return;
    const auto &intent = getCurrentIntent();

    switch (intent.type) {
    case IntentType::ATTACK: {
        int damage = intent.value;
        damage = m_buffManager.modifyOutgoingDamage(damage);
        damage = player->getBuffManager().modifyIncomingDamage(damage);
        if (damage < 0) damage = 0;
        player->takeDamage(damage);
        break;
    }
    case IntentType::DEFEND:
        addBlock(intent.value);
        break;
    case IntentType::BUFF_SELF: {
        // 目前用于自增力量
        std::shared_ptr<Buff> selfBuff;
        switch (static_cast<BuffType>(intent.buffType)) {
        case BuffType::STRENGTH:
            selfBuff = std::make_shared<Strength>(intent.value);
            break;
        default:
            break;
        }
        if (selfBuff) {
            m_buffManager.addBuff(selfBuff);
        }
        break;
    }
    case IntentType::DEBUFF_PLAYER: {
        std::shared_ptr<Buff> buff;
        switch (static_cast<BuffType>(intent.buffType)) {
        case BuffType::VULNERABLE:
            buff = std::make_shared<Vulnerable>(intent.value);
            break;
        case BuffType::WEAK:
            buff = std::make_shared<Weak>(intent.value);
            break;
        case BuffType::POISON:
            buff = std::make_shared<Poison>(intent.value);
            break;
        case BuffType::DOOM:
            buff = std::make_shared<Doom>(intent.value);
            break;
        case BuffType::STRENGTH:
            buff = std::make_shared<Strength>(intent.value);
            break;
        }
        if (buff) {
            player->getBuffManager().addBuff(buff);
        }
        break;
    }
    }
}
