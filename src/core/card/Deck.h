#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

/**
 * @brief 牌组管理
 *
 * 管理抽牌堆、手牌、弃牌堆和消耗牌堆的完整生命周期。
 * 支持抽牌、出牌、弃牌、洗牌、查看各堆等操作。
 */
class Deck
{
public:
    Deck();

    /**
     * @brief 初始化牌组（将所有卡牌放入抽牌堆并洗牌）
     * @param cards 完整卡牌列表（会被克隆一份）
     */
    void initialize(const std::vector<std::shared_ptr<Card>> &cards);

    /**
     * @brief 从抽牌堆抽取指定数量的牌到手牌
     *
     * 若抽牌堆不足，先将弃牌堆洗入抽牌堆再继续抽取。
     * @param count 抽牌数量
     */
    void drawCards(int count);

    /**
     * @brief 将手牌中指定卡牌移入弃牌堆
     * @param card 要打出的卡牌
     */
    void discardFromHand(std::shared_ptr<Card> card);

    /**
     * @brief 将手牌中指定卡牌移入消耗堆（消耗关键字）
     * @param card 要消耗的卡牌
     */
    void exhaustFromHand(std::shared_ptr<Card> card);

    /**
     * @brief 回合结束：手牌全部移入弃牌堆
     */
    void endTurn();

    /**
     * @brief 将弃牌堆洗牌后放入抽牌堆
     */
    void shuffleDiscardIntoDraw();

    /**
     * @brief 添加一张卡牌到牌组（获得新卡时）
     */
    void addCard(std::shared_ptr<Card> card);

    /**
     * @brief 从牌组中永久移除一张卡牌
     */
    void removeCard(const QString &cardId);

    // ─── 查询接口 ──────────────────────────────────
    const std::vector<std::shared_ptr<Card>>& getHand() const { return m_hand; }
    const std::vector<std::shared_ptr<Card>>& getDrawPile() const { return m_drawPile; }
    const std::vector<std::shared_ptr<Card>>& getDiscardPile() const { return m_discardPile; }
    const std::vector<std::shared_ptr<Card>>& getExhaustPile() const { return m_exhaustPile; }

    /** @brief 获取牌组中所有卡牌（用于牌组查看） */
    std::vector<std::shared_ptr<Card>> getAllCards() const;

private:
    /** @brief 洗牌（随机打乱） */
    void shuffle(std::vector<std::shared_ptr<Card>> &pile);

    std::vector<std::shared_ptr<Card>> m_drawPile;     ///< 抽牌堆
    std::vector<std::shared_ptr<Card>> m_hand;          ///< 手牌
    std::vector<std::shared_ptr<Card>> m_discardPile;   ///< 弃牌堆
    std::vector<std::shared_ptr<Card>> m_exhaustPile;   ///< 消耗牌堆
    std::vector<std::shared_ptr<Card>> m_masterDeck;    ///< 主牌组（完整列表）

    std::mt19937 m_rng;  ///< 随机数引擎
};

#endif // DECK_H
