#include "ShopScene.h"
#include "core/character/Player.h"
#include "core/item/Inventory.h"
#include "data/CardDatabase.h"
#include "data/RelicDatabase.h"
#include "data/PotionDatabase.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QMessageBox>
#include <random>
#include <chrono>

ShopScene::ShopScene(QWidget *parent)
    : QWidget(parent)
{
    m_titleLabel = new QLabel("商店", this);
    m_titleLabel->setStyleSheet("color: #E0C068; font-size: 24px; font-weight: bold;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setGeometry(0, 15, 1280, 35);

    m_goldLabel = new QLabel(this);
    m_goldLabel->setStyleSheet("color: #FFD700; font-size: 16px; font-weight: bold;");

    m_leaveBtn = new QPushButton("离开商店", this);
    m_leaveBtn->setFixedSize(120, 40);
    m_leaveBtn->setStyleSheet(
        "QPushButton { background-color: #5A5A6A; color: white; border-radius: 6px;"
        "font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #7A7A8A; }");
    connect(m_leaveBtn, &QPushButton::clicked, this, &ShopScene::shopFinished);
}

void ShopScene::initShop(Player *player, Inventory *inventory)
{
    m_player = player;
    m_inventory = inventory;

    for (auto &item : m_items) {
        if (item.btn) item.btn->deleteLater();
    }
    m_items.clear();

    refreshItems();
    update();
}

void ShopScene::refreshItems()
{
    if (!m_player) return;

    m_goldLabel->setText(QString("金币: %1").arg(m_player->getGold()));

    std::mt19937 rng(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));

    auto &cardDb = CardDatabase::instance();
    auto cardIds = cardDb.getAllCardIds();
    std::shuffle(cardIds.begin(), cardIds.end(), rng);

    int y = 80;
    int idx = 0;

    auto addItem = [&](const QString &name, const QString &desc, int cost,
                        int type, const QString &id) {
        ShopItem item;
        item.name = name;
        item.description = desc;
        item.cost = cost;
        item.type = type;
        item.id = id;

        item.btn = new QPushButton(QString("%1 - %2金币").arg(name).arg(cost), this);
        item.btn->setGeometry(100, y + idx * 50, 400, 40);
        item.btn->setStyleSheet(
            "QPushButton { background-color: #3A4A5A; color: white; border-radius: 6px;"
            "font-size: 13px; text-align: left; padding-left: 15px; }"
            "QPushButton:hover { background-color: #4A5A6A; }");
        item.btn->setToolTip(desc);
        item.btn->show();

        connect(item.btn, &QPushButton::clicked, this, [this, item]() {
            if (!m_player->spendGold(item.cost)) {
                QMessageBox::warning(this, "金币不足", "你没有足够的金币！");
                return;
            }
            if (item.type == 0) {
                auto card = CardDatabase::instance().createCard(item.id);
                if (card) m_player->getDeck().addCard(card);
            } else if (item.type == 1) {
                auto relic = RelicDatabase::instance().createRelic(item.id);
                if (relic) m_inventory->addRelic(relic, m_player);
            } else if (item.type == 2) {
                auto potion = PotionDatabase::instance().createPotion(item.id);
                if (potion) {
                    if (!m_inventory->addPotion(potion)) {
                        m_player->addGold(item.cost);
                        QMessageBox::warning(this, "药水已满", "药水栏已满！");
                        return;
                    }
                }
            }
            item.btn->setEnabled(false);
            item.btn->setText(item.btn->text() + " (已购买)");
            m_goldLabel->setText(QString("金币: %1").arg(m_player->getGold()));
        });

        m_items.push_back(item);
        idx++;
    };

    int cardCount = std::min(3, static_cast<int>(cardIds.size()));
    for (int i = 0; i < cardCount; ++i) {
        auto card = cardDb.createCard(cardIds[i]);
        if (card) {
            int cost = 50 + card->getCost() * 25;
            addItem(card->getName(), card->getDescription(), cost, 0, cardIds[i]);
        }
    }

    auto relicIds = RelicDatabase::instance().getAllRelicIds();
    std::shuffle(relicIds.begin(), relicIds.end(), rng);
    for (auto &relicId : relicIds) {
        if (m_inventory && m_inventory->hasRelic(relicId)) continue;
        auto relic = RelicDatabase::instance().createRelic(relicId);
        if (relic) {
            addItem(relic->getName(), relic->getDescription(), 150, 1, relicId);
        }
        break;
    }

    auto potionIds = PotionDatabase::instance().getAllPotionIds();
    std::shuffle(potionIds.begin(), potionIds.end(), rng);
    int potionCount = std::min(2, static_cast<int>(potionIds.size()));
    for (int i = 0; i < potionCount; ++i) {
        auto potion = PotionDatabase::instance().createPotion(potionIds[i]);
        if (potion) {
            addItem(potion->getName(), potion->getDescription(), 50, 2, potionIds[i]);
        }
    }

    m_goldLabel->setGeometry(100, y + idx * 50 + 20, 200, 30);
    m_leaveBtn->setGeometry(width() / 2 - 60, y + idx * 50 + 60, 120, 40);
}

void ShopScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(25, 30, 40));
}
