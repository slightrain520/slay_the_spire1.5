#include "BattleManager.h"
#include "DamageCalculator.h"
#include "core/character/Player.h"
#include "core/character/Enemy.h"
#include "core/card/Deck.h"
#include "core/item/Inventory.h"
#include "core/buff/Doom.h"
#include "core/buff/Buff.h"
#include "data/GameConfig.h"

BattleManager::BattleManager(QObject *parent)
    : QObject(parent)
{
}

void BattleManager::startBattle(Player *player,
                                 std::vector<std::shared_ptr<Enemy>> enemies,
                                 Inventory *inventory)
{
    m_player = player;
    m_enemies = std::move(enemies);
    m_inventory = inventory;
    m_currentTurn = 0;
    m_battleOver = false;
    m_playerVictory = false;
    m_maxEnergy = player->getMaxEnergy();

    m_player->getBuffManager().clearAll();
    m_player->clearBlock();

    m_player->getDeck().initialize(m_player->getDeck().getAllCards());

    triggerRelicBattleStart();

    emit battleStarted();
    startPlayerTurn();
}

void BattleManager::startPlayerTurn()
{
    m_currentTurn++;
    m_cardsPlayedThisTurn = 0;

    // 格挡应在角色自己的回合开始时清空，需保留到对方行动阶段用于减伤。
    m_player->clearBlock();

    m_energy = m_maxEnergy;
    if (m_inventory) {
        for (auto &relic : m_inventory->getRelics()) {
            m_energy = relic->modifyEnergy(m_energy);
        }
        m_inventory->resetRelicTurnStates();
    }
    m_player->setEnergy(m_energy);

    int startDamage = m_player->getBuffManager().triggerTurnStart();
    if (startDamage > 0) {
        DamageCalculator::applyDamage(m_player, startDamage, m_inventory);
    }

    triggerRelicTurnStart();

    m_player->getDeck().drawCards(GameConfig::CARDS_PER_DRAW);

    emit playerTurnStarted();
    emit energyChanged(m_energy, m_maxEnergy);
    emit handUpdated();
    emit turnChanged(m_currentTurn);
    emit characterUpdated();

    checkBattleEnd();
}

void BattleManager::endPlayerTurn()
{
    if (m_battleOver) return;

    m_player->getDeck().endTurn();

    resolveBuffEndOfTurn(m_player);

    triggerRelicTurnEnd();

    emit handUpdated();
    emit characterUpdated();

    checkBattleEnd();
    if (m_battleOver) return;

    executeEnemyTurn();
}

void BattleManager::executeEnemyTurn()
{
    emit enemyTurnStarted();

    for (auto &enemy : m_enemies) {
        if (enemy->isDead()) continue;

        // 与玩家一致：敌人格挡在自己回合开始时清空。
        enemy->clearBlock();

        int startDmg = enemy->getBuffManager().triggerTurnStart();
        if (startDmg > 0) {
            DamageCalculator::applyDamage(enemy.get(), startDmg);
        }

        if (enemy->isDead()) continue;

        const auto &intent = enemy->getCurrentIntent();
        if (intent.type == IntentType::ATTACK) {
            int damage = intent.value;
            damage = enemy->getBuffManager().modifyOutgoingDamage(damage);
            damage = m_player->getBuffManager().modifyIncomingDamage(damage);
            if (m_inventory) {
                damage = DamageCalculator::applyRelicDamageModifiers(damage, m_inventory);
            }
            damage = std::max(0, damage);
            DamageCalculator::applyDamage(m_player, damage, m_inventory);
        } else {
            enemy->executeIntent(m_player);
        }

        resolveBuffEndOfTurn(enemy.get());
        enemy->advanceIntent();
    }

    // 敌人攻击可能使玩家HP降至灾厄触发线以下，补充一次玩家灾厄检测
    checkDoomDeath();

    emit enemyTurnFinished();
    emit characterUpdated();

    checkBattleEnd();
    if (!m_battleOver) {
        startPlayerTurn();
    }
}

bool BattleManager::playCard(std::shared_ptr<Card> card, Character *target)
{
    if (m_battleOver || !card) return false;

    if (m_energy < card->getCost()) return false;

    m_energy -= card->getCost();
    m_player->setEnergy(m_energy);

    Character *effectTarget = target;
    if (card->getTargetType() == TargetType::SELF) {
        effectTarget = m_player;
    }

    if (card->getTargetType() == TargetType::ALL_ENEMIES) {
        int playCount = 1 + card->getRepeat();
        for (int r = 0; r < playCount; ++r) {
            for (auto &enemy : m_enemies) {
                if (!enemy->isDead()) {
                    card->play(m_player, enemy.get(), this);
                }
            }
        }
    } else {
        int playCount = 1 + card->getRepeat();
        for (int r = 0; r < playCount; ++r) {
            card->play(m_player, effectTarget, this);
        }
    }

    if (card->isExhaust()) {
        m_player->getDeck().exhaustFromHand(card);
    } else {
        m_player->getDeck().discardFromHand(card);
    }
    m_cardsPlayedThisTurn++;

    if (m_inventory) {
        for (auto &relic : m_inventory->getRelics()) {
            relic->onCardPlayed(m_player, card.get());
        }
    }

    emit cardPlayed(card);
    emit energyChanged(m_energy, m_maxEnergy);
    emit handUpdated();
    emit characterUpdated();

    checkBattleEnd();
    return true;
}

void BattleManager::drawCards(int count)
{
    if (m_player) {
        m_player->getDeck().drawCards(count);
        emit handUpdated();
    }
}

bool BattleManager::usePotion(int index, std::vector<Character*> targets)
{
    if (!m_inventory || m_battleOver) return false;
    bool ok = m_inventory->usePotion(index, m_player, targets);
    if (!ok) return false;
    emit characterUpdated();
    checkBattleEnd();
    return true;
}

std::vector<Enemy*> BattleManager::getAliveEnemies() const
{
    std::vector<Enemy*> alive;
    for (auto &e : m_enemies) {
        if (!e->isDead()) {
            alive.push_back(e.get());
        }
    }
    return alive;
}

void BattleManager::resolveBuffEndOfTurn(Character *character)
{
    // 灾厄判定先于中毒扣血：根据执行中毒前的当前生命値判断
    checkDoomForCharacter(character);
    if (character->isDead()) return;  // 灾厄才就死了，不再执行后续

    Inventory *inv = (character == m_player) ? m_inventory : nullptr;
    int damage = character->getBuffManager().triggerTurnEnd();
    if (damage > 0) {
        DamageCalculator::applyDamage(character, damage, inv);
    }
}

void BattleManager::checkDoomForCharacter(Character *character)
{
    Buff *doom = character->getBuffManager().getBuff(BuffType::DOOM);
    if (doom) {
        auto *doomBuff = dynamic_cast<Doom*>(doom);
        if (doomBuff && doomBuff->shouldTriggerDeath(character->getHp())) {
            character->setHp(0);
        }
    }
}

void BattleManager::checkDoomDeath()
{
    // 敌人回合结束后只需对玩家再次检测（敌人已在 resolveBuffEndOfTurn 中逐个检测过）
    checkDoomForCharacter(m_player);
}

void BattleManager::checkBattleEnd()
{
    if (m_battleOver) return;

    if (m_player->isDead()) {
        m_battleOver = true;
        m_playerVictory = false;
        emit battleEnded(false);
        return;
    }

    bool allEnemiesDead = true;
    for (auto &e : m_enemies) {
        if (!e->isDead()) {
            allEnemiesDead = false;
            break;
        }
    }
    if (allEnemiesDead) {
        m_battleOver = true;
        m_playerVictory = true;
        emit battleEnded(true);
    }
}

void BattleManager::triggerRelicBattleStart()
{
    if (!m_inventory) return;
    for (auto &relic : m_inventory->getRelics()) {
        relic->onBattleStart(m_player);
    }
}

void BattleManager::triggerRelicTurnStart()
{
    if (!m_inventory) return;
    for (auto &relic : m_inventory->getRelics()) {
        relic->onTurnStart(m_player);
    }
}

void BattleManager::triggerRelicTurnEnd()
{
    if (!m_inventory) return;
    for (auto &relic : m_inventory->getRelics()) {
        relic->onTurnEnd(m_player);
    }
}
