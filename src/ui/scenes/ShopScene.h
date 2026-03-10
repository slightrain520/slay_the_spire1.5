#ifndef SHOPSCENE_H
#define SHOPSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <memory>

class Player;
class Inventory;
class Card;
class Relic;
class Potion;

/**
 * @brief 商店场景
 *
 * 玩家可用金币购买卡牌、药水、遗物。
 */
class ShopScene : public QWidget
{
    Q_OBJECT

public:
    explicit ShopScene(QWidget *parent = nullptr);

    void initShop(Player *player, Inventory *inventory);

signals:
    void shopFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void refreshItems();

    Player *m_player = nullptr;
    Inventory *m_inventory = nullptr;

    struct ShopItem {
        QString name;
        QString description;
        int cost;
        int type; // 0=card, 1=relic, 2=potion
        QString id;
        QPushButton *btn;
    };

    std::vector<ShopItem> m_items;
    QLabel *m_goldLabel;
    QLabel *m_titleLabel;
    QPushButton *m_leaveBtn;
};

#endif // SHOPSCENE_H
