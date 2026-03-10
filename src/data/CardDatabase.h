#ifndef CARDDATABASE_H
#define CARDDATABASE_H

#include <QMap>
#include <QString>
#include <memory>
#include <vector>
#include "core/card/Card.h"

/**
 * @brief 卡牌数据库（单例）
 *
 * 注册所有卡牌模板，通过 ID 创建卡牌实例。
 * 新卡牌只需在 registerAll() 中添加注册即可。
 */
class CardDatabase
{
public:
    static CardDatabase& instance();

    /**
     * @brief 通过 ID 创建一张卡牌的副本
     * @param id 卡牌唯一标识
     * @return 卡牌实例，未找到则返回 nullptr
     */
    std::shared_ptr<Card> createCard(const QString &id) const;

    /**
     * @brief 获取战士初始牌组的所有卡牌 ID
     */
    std::vector<QString> getWarriorStarterDeck() const;

    /**
     * @brief 获取女王初始牌组的所有卡牌 ID
     */
    std::vector<QString> getQueenStarterDeck() const;

    /**
     * @brief 获取所有已注册的卡牌 ID
     */
    QList<QString> getAllCardIds() const;

private:
    CardDatabase();
    void registerAll();

    /**
     * @brief 注册一张卡牌模板
     */
    void registerCard(std::shared_ptr<Card> card);

    QMap<QString, std::shared_ptr<Card>> m_templates;  ///< ID -> 卡牌模板
};

#endif // CARDDATABASE_H
