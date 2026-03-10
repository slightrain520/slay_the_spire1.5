#include "Queen.h"
#include "data/CardDatabase.h"
#include "data/GameConfig.h"

Queen::Queen()
    : Player("女王", GameConfig::QUEEN_MAX_HP, GameConfig::INITIAL_GOLD, GameConfig::INITIAL_ENERGY)
{
    initializeDeck();
}

void Queen::initializeDeck()
{
    auto &db = CardDatabase::instance();
    std::vector<std::shared_ptr<Card>> cards;

    for (const auto &id : db.getQueenStarterDeck()) {
        auto card = db.createCard(id);
        if (card) {
            cards.push_back(card);
        }
    }

    m_deck.initialize(cards);
}
