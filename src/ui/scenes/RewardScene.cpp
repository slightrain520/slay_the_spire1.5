#include "RewardScene.h"
#include "core/character/Player.h"
#include "core/item/Inventory.h"
#include "data/CardDatabase.h"
#include "data/RelicDatabase.h"
#include "data/PotionDatabase.h"
#include <QPainter>
#include <QVBoxLayout>
#include <random>
#include <chrono>

RewardScene::RewardScene(QWidget *parent)
    : QWidget(parent)
{
    m_titleLabel = new QLabel("战利品", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #E0C068; font-size: 24px; font-weight: bold;");

    m_goldLabel = new QLabel(this);
    m_goldLabel->setAlignment(Qt::AlignCenter);
    m_goldLabel->setStyleSheet("color: #FFD700; font-size: 16px;");

    m_continueBtn = new QPushButton("继续", this);
    m_continueBtn->setFixedSize(120, 40);
    m_continueBtn->setStyleSheet(
        "QPushButton { background-color: #4A6B3A; color: white; border-radius: 6px;"
        "font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #5A8B4A; }");
    connect(m_continueBtn, &QPushButton::clicked, this, &RewardScene::rewardFinished);
}

void RewardScene::initReward(Player *player, Inventory *inventory,
                              int goldReward, bool isElite)
{
    m_player = player;
    m_inventory = inventory;
    m_cardChosen = false;

    for (auto *btn : m_choiceBtns) btn->deleteLater();
    m_choiceBtns.clear();

    player->addGold(goldReward);
    m_goldLabel->setText(QString("获得 %1 金币（总计: %2）")
                             .arg(goldReward).arg(player->getGold()));

    m_titleLabel->setGeometry(0, 20, width(), 35);
    m_goldLabel->setGeometry(0, 65, width(), 25);

    generateCardChoices();

    // 精英战给额外遗物（过滤已拥有的）
    if (isElite) {
        auto relicIds = RelicDatabase::instance().getAllRelicIds();
        std::mt19937 rng(static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
        std::shuffle(relicIds.begin(), relicIds.end(), rng);
        for (auto &relicId : relicIds) {
            if (m_inventory->hasRelic(relicId)) continue;
            auto relic = RelicDatabase::instance().createRelic(relicId);
            if (relic) {
                m_inventory->addRelic(relic, m_player);
                auto *relicLabel = new QLabel(
                    QString("获得遗物: %1").arg(relic->getName()), this);
                relicLabel->setStyleSheet("color: #FFD700; font-size: 14px;");
                relicLabel->setGeometry(width() / 2 - 150, 95, 300, 25);
                relicLabel->setAlignment(Qt::AlignCenter);
                relicLabel->show();
            }
            break;
        }
    }

    m_continueBtn->setGeometry(width() / 2 - 60, height() - 80, 120, 40);

    update();
}

void RewardScene::generateCardChoices()
{
    if (!m_player) return;

    auto &db = CardDatabase::instance();
    auto allIds = db.getAllCardIds();

    std::mt19937 rng(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));
    std::shuffle(allIds.begin(), allIds.end(), rng);

    int choiceCount = std::min(3, static_cast<int>(allIds.size()));
    int startY = 130;

    auto *label = new QLabel("选择一张卡牌加入牌组（或跳过）:", this);
    label->setStyleSheet("color: #CCCCCC; font-size: 14px;");
    label->setGeometry(width() / 2 - 200, startY - 30, 400, 25);
    label->setAlignment(Qt::AlignCenter);
    label->show();
    m_choiceBtns.push_back(reinterpret_cast<QPushButton*>(label));

    for (int i = 0; i < choiceCount; ++i) {
        auto card = db.createCard(allIds[i]);
        if (!card) continue;

        auto *btn = new QPushButton(
            QString("%1 (费用:%2) - %3").arg(card->getName())
                .arg(card->getCost()).arg(card->getDescription()),
            this);
        btn->setGeometry(width() / 2 - 250, startY + i * 55, 500, 44);
        btn->setStyleSheet(
            "QPushButton { background-color: #2A3A4A; color: #CCCCCC; border-radius: 6px;"
            "font-size: 13px; text-align: left; padding-left: 15px;"
            "border: 2px solid #3A5A6A; }"
            "QPushButton:hover { background-color: #3A6A8A; border: 2px solid #FFD700;"
            "color: white; font-weight: bold; }"
            "QPushButton:disabled { background-color: #1A2A3A; color: #555566;"
            "border: 2px solid #2A3A4A; }");
        btn->setCursor(Qt::PointingHandCursor);
        btn->show();

        QString cardId = allIds[i];
        connect(btn, &QPushButton::clicked, this, [this, cardId]() {
            if (m_cardChosen) return;
            m_cardChosen = true;
            auto card = CardDatabase::instance().createCard(cardId);
            if (card) m_player->getDeck().addCard(card);
            for (auto *b : m_choiceBtns) b->setEnabled(false);
        });
        m_choiceBtns.push_back(btn);
    }
}

void RewardScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(20, 25, 35));
}
