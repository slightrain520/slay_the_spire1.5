#include "Warrior.h"
#include "data/CardDatabase.h"
#include "data/GameConfig.h"

Warrior::Warrior()
    : Player("战士", GameConfig::WARRIOR_MAX_HP, GameConfig::INITIAL_GOLD, GameConfig::INITIAL_ENERGY)
{
    initializeDeck();
}

void Warrior::initializeDeck()
{
    auto &db = CardDatabase::instance();
    std::vector<std::shared_ptr<Card>> cards;

    for (const auto &id : db.getWarriorStarterDeck()) {
        auto card = db.createCard(id);
        if (card) {
            cards.push_back(card);
        }
    }

    m_deck.initialize(cards);
}
