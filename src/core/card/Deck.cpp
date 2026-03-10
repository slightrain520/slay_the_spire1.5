#include "Deck.h"
#include <chrono>

Deck::Deck()
    : m_rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
{
}

void Deck::initialize(const std::vector<std::shared_ptr<Card>> &cards)
{
    m_masterDeck.clear();
    m_drawPile.clear();
    m_hand.clear();
    m_discardPile.clear();
    m_exhaustPile.clear();

    std::vector<std::shared_ptr<Card>> innateCards;
    std::vector<std::shared_ptr<Card>> normalCards;

    for (auto &card : cards) {
        auto cloned = card->clone();
        m_masterDeck.push_back(cloned);
        if (cloned->isInnate()) {
            innateCards.push_back(cloned);
        } else {
            normalCards.push_back(cloned);
        }
    }
    // 先洗非固有牌，再将固有牌放到末尾（drawCards从末尾取牌，固有牌保证第一回合必定抽到）
    shuffle(normalCards);
    m_drawPile = normalCards;
    for (auto &c : innateCards) {
        m_drawPile.push_back(c);
    }
}

void Deck::drawCards(int count)
{
    for (int i = 0; i < count; ++i) {
        if (m_drawPile.empty()) {
            if (m_discardPile.empty()) {
                return;
            }
            shuffleDiscardIntoDraw();
        }
        if (!m_drawPile.empty()) {
            m_hand.push_back(m_drawPile.back());
            m_drawPile.pop_back();
        }
    }
}

void Deck::discardFromHand(std::shared_ptr<Card> card)
{
    auto it = std::find(m_hand.begin(), m_hand.end(), card);
    if (it != m_hand.end()) {
        m_discardPile.push_back(*it);
        m_hand.erase(it);
    }
}

void Deck::exhaustFromHand(std::shared_ptr<Card> card)
{
    auto it = std::find(m_hand.begin(), m_hand.end(), card);
    if (it != m_hand.end()) {
        m_exhaustPile.push_back(*it);
        m_hand.erase(it);
    }
}

void Deck::endTurn()
{
    std::vector<std::shared_ptr<Card>> retained;
    for (auto &card : m_hand) {
        if (card->isRetain()) {
            retained.push_back(card);  // 保留：留在手牌
        } else if (card->isExhaust()) {
            m_exhaustPile.push_back(card);  // 消耗：进消耗堆
        } else {
            m_discardPile.push_back(card);  // 普通：入弃牌堆
        }
    }
    m_hand = retained;
}

void Deck::shuffleDiscardIntoDraw()
{
    for (auto &card : m_discardPile) {
        m_drawPile.push_back(card);
    }
    m_discardPile.clear();
    shuffle(m_drawPile);
}

void Deck::addCard(std::shared_ptr<Card> card)
{
    m_masterDeck.push_back(card);
    m_drawPile.push_back(card);
}

void Deck::removeCard(const QString &cardId)
{
    auto removeById = [&](std::vector<std::shared_ptr<Card>> &pile) {
        pile.erase(
            std::remove_if(pile.begin(), pile.end(),
                [&](const std::shared_ptr<Card> &c) { return c->getId() == cardId; }),
            pile.end()
        );
    };

    removeById(m_masterDeck);
    removeById(m_drawPile);
    removeById(m_hand);
    removeById(m_discardPile);
    removeById(m_exhaustPile);
}

std::vector<std::shared_ptr<Card>> Deck::getAllCards() const
{
    return m_masterDeck;
}

void Deck::shuffle(std::vector<std::shared_ptr<Card>> &pile)
{
    std::shuffle(pile.begin(), pile.end(), m_rng);
}
