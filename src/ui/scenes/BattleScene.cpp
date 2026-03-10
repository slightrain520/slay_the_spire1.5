#include "BattleScene.h"
#include "ui/widgets/CardWidget.h"
#include "core/battle/BattleManager.h"
#include "core/character/Player.h"
#include "core/character/Enemy.h"
#include "core/item/Inventory.h"
#include "core/item/Potion.h"
#include "core/buff/BuffManager.h"
#include "core/buff/Buff.h"
#include "data/GameConfig.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QToolTip>
#include <QMessageBox>
#include <QHBoxLayout>
#include <algorithm>

BattleScene::BattleScene(QWidget *parent)
    : QWidget(parent)
{
    m_endTurnBtn = new QPushButton("结束回合", this);
    m_endTurnBtn->setFixedSize(100, 40);
    m_endTurnBtn->setStyleSheet(
        "QPushButton { background-color: #8B4513; color: white; border-radius: 6px;"
        "font-size: 14px; font-weight: bold; border: 2px solid #CD853F; }"
        "QPushButton:hover { background-color: #A0522D; }");
    connect(m_endTurnBtn, &QPushButton::clicked, this, &BattleScene::onEndTurnClicked);

    m_energyLabel = new QLabel(this);
    m_energyLabel->setStyleSheet("color: #FFD700; font-size: 22px; font-weight: bold;");
    m_energyLabel->setAlignment(Qt::AlignCenter);

    m_turnLabel = new QLabel(this);
    m_turnLabel->setStyleSheet("color: #CCCCCC; font-size: 13px;");
    m_turnLabel->setAlignment(Qt::AlignCenter);

    m_goldLabel = new QLabel(this);
    m_goldLabel->setStyleSheet("color: #FFD700; font-size: 14px; font-weight: bold;");

    m_floorLabel = new QLabel(this);
    m_floorLabel->setStyleSheet("color: #AAAAAA; font-size: 14px;");

    m_exitBtn = new QPushButton("退出", this);
    m_exitBtn->setFixedSize(60, 30);
    m_exitBtn->setStyleSheet(
        "QPushButton { background-color: #6B3A3A; color: white; border-radius: 4px; font-size: 12px; }"
        "QPushButton:hover { background-color: #8B4A4A; }");
    connect(m_exitBtn, &QPushButton::clicked, this, &BattleScene::exitRequested);

    m_drawPileBtn = new QPushButton("抽牌堆", this);
    m_discardPileBtn = new QPushButton("弃牌堆", this);
    m_exhaustPileBtn = new QPushButton("消耗堆", this);

    auto pileStyle = "QPushButton { background-color: #3A3A4A; color: #AAA;"
                     "border-radius: 4px; font-size: 11px; padding: 4px 10px; }"
                     "QPushButton:hover { background-color: #4A4A5A; color: white; }";
    m_drawPileBtn->setStyleSheet(pileStyle);
    m_discardPileBtn->setStyleSheet(pileStyle);
    m_exhaustPileBtn->setStyleSheet(pileStyle);

    connect(m_drawPileBtn, &QPushButton::clicked, this, [this]() {
        if (!m_player) return;
        auto &pile = m_player->getDeck().getDrawPile();
        QString text = QString("抽牌堆 (%1 张):\n").arg(pile.size());
        for (auto &c : pile) text += c->getName() + "\n";
        QMessageBox::information(this, "抽牌堆", text);
    });
    connect(m_discardPileBtn, &QPushButton::clicked, this, [this]() {
        if (!m_player) return;
        auto &pile = m_player->getDeck().getDiscardPile();
        QString text = QString("弃牌堆 (%1 张):\n").arg(pile.size());
        for (auto &c : pile) text += c->getName() + "\n";
        QMessageBox::information(this, "弃牌堆", text);
    });
    connect(m_exhaustPileBtn, &QPushButton::clicked, this, [this]() {
        if (!m_player) return;
        auto &pile = m_player->getDeck().getExhaustPile();
        QString text = QString("消耗堆 (%1 张):\n").arg(pile.size());
        for (auto &c : pile) text += c->getName() + "\n";
        QMessageBox::information(this, "消耗堆", text);
    });

    setMouseTracking(true);
}

void BattleScene::initBattle(BattleManager *battle, Player *player,
                              std::vector<std::shared_ptr<Enemy>> enemies,
                              Inventory *inventory, int floor)
{
    m_battle = battle;
    m_player = player;
    m_enemies = enemies;
    m_inventory = inventory;
    m_floor = floor;
    m_selectedCard = nullptr;

    connect(m_battle, &BattleManager::handUpdated, this, &BattleScene::onHandUpdated, Qt::UniqueConnection);
    connect(m_battle, &BattleManager::characterUpdated, this, &BattleScene::onCharacterUpdated, Qt::UniqueConnection);
    connect(m_battle, &BattleManager::energyChanged, this, &BattleScene::onEnergyChanged, Qt::UniqueConnection);
    connect(m_battle, &BattleManager::battleEnded, this, &BattleScene::onBattleEnded, Qt::UniqueConnection);

    updateStatusDisplay();
    update();
}

void BattleScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(20, 20, 30));

    // 顶栏背景
    painter.fillRect(0, 0, width(), 45, QColor(30, 30, 45));
    painter.setPen(QPen(QColor(60, 60, 80), 1));
    painter.drawLine(0, 45, width(), 45);

    if (!m_player) return;

    // 玩家区域（中部左侧）
    int midY = height() / 2 - 60;
    m_playerArea = QRect(60, midY - 80, 200, 220);
    drawCharacterInfo(painter, m_player, m_playerArea, true);

    // 敌人区域（中部右侧）
    m_enemyAreas.clear();
    int enemyCount = 0;
    for (auto &e : m_enemies) {
        if (!e->isDead()) enemyCount++;
    }

    int enemyStartX = width() - 60 - enemyCount * 200;
    int idx = 0;
    for (size_t i = 0; i < m_enemies.size(); ++i) {
        if (m_enemies[i]->isDead()) {
            m_enemyAreas.push_back(QRect());
            continue;
        }
        QRect area(enemyStartX + idx * 210, midY - 80, 200, 220);
        m_enemyAreas.push_back(area);
        drawCharacterInfo(painter, m_enemies[i].get(), area, false);
        idx++;
    }

    // 底部手牌区背景
    int handAreaY = height() - 230;
    painter.fillRect(0, handAreaY, width(), 230, QColor(25, 25, 38));
    painter.setPen(QPen(QColor(60, 60, 80), 1));
    painter.drawLine(0, handAreaY, width(), handAreaY);

    // 药水显示（顶栏）
    if (m_inventory) {
        auto &potions = m_inventory->getPotions();
        QFont pFont;
        pFont.setPointSize(11);
        painter.setFont(pFont);
        int px = 150;
        for (int i = 0; i < 3; ++i) {
            QRect slot(px + i * 70, 8, 60, 30);
            if (static_cast<size_t>(i) < potions.size()) {
                bool isPending = (m_pendingPotionIdx == i);
                bool isHovered = (m_hoveredPotionIdx == i);
                QColor bg = isPending ? QColor(180, 130, 20)
                          : isHovered ? QColor(80, 150, 80)
                          : QColor(60, 100, 60);
                painter.setPen(isPending ? QPen(QColor(255, 210, 0), 2)
                             : isHovered ? QPen(QColor(100, 220, 100), 1)
                             : Qt::NoPen);
                painter.setBrush(bg);
                painter.drawRoundedRect(slot, 4, 4);
                painter.setPen(Qt::white);
                painter.drawText(slot, Qt::AlignCenter, potions[i]->getName().left(3));
            } else {
                painter.setPen(QPen(QColor(80, 80, 80), 1));
                painter.setBrush(QColor(40, 40, 50));
                painter.drawRoundedRect(slot, 4, 4);
                painter.setPen(QColor(80, 80, 80));
                painter.drawText(slot, Qt::AlignCenter, "空");
            }
        }

        // 靶向模式提示
        if (m_pendingPotionIdx >= 0 && m_pendingPotionIdx < static_cast<int>(potions.size())) {
            QFont tipFont;
            tipFont.setPointSize(11);
            tipFont.setBold(true);
            painter.setFont(tipFont);
            painter.setPen(QColor(255, 220, 60));
            painter.drawText(QRect(0, 48, width(), 24), Qt::AlignCenter,
                             "请点击敌人以使用药水（右键取消）");
            // 高亮敌人区域
            for (auto &area : m_enemyAreas) {
                if (area.isNull()) continue;
                painter.setPen(QPen(QColor(255, 220, 60), 3));
                painter.setBrush(Qt::NoBrush);
                painter.drawRoundedRect(area.adjusted(-5, -5, 5, 5), 12, 12);
            }
        }

        // 遗物显示
        auto &relics = m_inventory->getRelics();
        px = 380;
        QFont rFont;
        rFont.setPointSize(9);
        painter.setFont(rFont);
        for (size_t i = 0; i < relics.size(); ++i) {
            QRect rSlot(px + static_cast<int>(i) * 50, 10, 40, 26);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(100, 80, 40));
            painter.drawRoundedRect(rSlot, 4, 4);
            painter.setPen(QColor(255, 215, 0));
            painter.drawText(rSlot, Qt::AlignCenter, relics[i]->getName().left(2));
        }
    }
}

void BattleScene::mousePressEvent(QMouseEvent *event)
{
    if (!m_inventory || !m_battle || m_battle->isBattleOver()) {
        QWidget::mousePressEvent(event);
        return;
    }

    QPoint pos = event->pos();

    // 右键：取消靶向模式
    if (event->button() == Qt::RightButton) {
        if (m_pendingPotionIdx >= 0) {
            m_pendingPotionIdx = -1;
            update();
        }
        return;
    }

    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    // 靶向模式：点击敌人完成使用
    if (m_pendingPotionIdx >= 0) {
        for (size_t i = 0; i < m_enemyAreas.size(); ++i) {
            if (!m_enemyAreas[i].isNull() && m_enemyAreas[i].contains(pos)
                && i < m_enemies.size() && !m_enemies[i]->isDead()) {
                std::vector<Character*> targets = { m_enemies[i].get() };
                m_battle->usePotion(m_pendingPotionIdx, targets);
                m_pendingPotionIdx = -1;
                update();
                return;
            }
        }
        // 点击到其他位置则取消
        m_pendingPotionIdx = -1;
        update();
        return;
    }

    // 检测药水槽点击
    auto &potions = m_inventory->getPotions();
    int px = 150;
    for (int i = 0; i < 3; ++i) {
        QRect slot(px + i * 70, 8, 60, 30);
        if (i < static_cast<int>(potions.size()) && slot.contains(pos)) {
            auto targetType = potions[i]->getTargetType();
            if (targetType == PotionTargetType::SELF) {
                std::vector<Character*> targets = { m_player };
                m_battle->usePotion(i, targets);
                update();
            } else if (targetType == PotionTargetType::ALL_ENEMIES) {
                std::vector<Character*> targets;
                for (auto &e : m_enemies) {
                    if (!e->isDead()) targets.push_back(e.get());
                }
                m_battle->usePotion(i, targets);
                update();
            } else { // SINGLE_ENEMY
                auto alive = m_battle->getAliveEnemies();
                if (alive.size() == 1) {
                    std::vector<Character*> targets = { alive[0] };
                    m_battle->usePotion(i, targets);
                    update();
                } else if (!alive.empty()) {
                    m_pendingPotionIdx = i;
                    update();
                }
            }
            return;
        }
    }

    QWidget::mousePressEvent(event);
}

void BattleScene::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_inventory) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    QPoint pos = event->pos();

    // 遗物悬停提示
    auto &relics = m_inventory->getRelics();
    int rPx = 380;
    for (size_t i = 0; i < relics.size(); ++i) {
        QRect rSlot(rPx + static_cast<int>(i) * 50, 10, 40, 26);
        if (rSlot.contains(pos)) {
            QString tip = QString("<b>%1</b><br/>%2")
                              .arg(relics[i]->getName())
                              .arg(relics[i]->getDescription());
            QToolTip::showText(event->globalPosition().toPoint(), tip, this, rSlot);
            QWidget::mouseMoveEvent(event);
            return;
        }
    }

    // 药水悬停提示
    auto &potions = m_inventory->getPotions();
    int px = 150;
    for (int i = 0; i < static_cast<int>(potions.size()); ++i) {
        QRect slot(px + i * 70, 8, 60, 30);
        if (slot.contains(pos)) {
            QString tip = QString("<b>%1</b><br/>%2")
                              .arg(potions[i]->getName())
                              .arg(potions[i]->getDescription());
            QToolTip::showText(event->globalPosition().toPoint(), tip, this, slot);
            if (m_hoveredPotionIdx != i) {
                m_hoveredPotionIdx = i;
                update();
            }
            QWidget::mouseMoveEvent(event);
            return;
        }
    }

    // 离开药水/遗物区域
    if (m_hoveredPotionIdx >= 0) {
        m_hoveredPotionIdx = -1;
        update();
    }
    QToolTip::hideText();
    QWidget::mouseMoveEvent(event);
}

void BattleScene::leaveEvent(QEvent *event)
{
    if (m_hoveredPotionIdx >= 0) {
        m_hoveredPotionIdx = -1;
        update();
    }
    QWidget::leaveEvent(event);
}

void BattleScene::drawCharacterInfo(QPainter &painter, Character *character,
                                     QRect area, bool isPlayer) const
{
    // 背景框
    QColor frameColor = isPlayer ? QColor(40, 60, 80) : QColor(80, 40, 40);
    painter.setPen(QPen(frameColor.lighter(140), 2));
    painter.setBrush(frameColor);
    painter.drawRoundedRect(area, 10, 10);

    // 角色名称
    painter.setPen(Qt::white);
    QFont nameFont;
    nameFont.setPointSize(14);
    nameFont.setBold(true);
    painter.setFont(nameFont);
    painter.drawText(QRect(area.x(), area.y() + 10, area.width(), 24),
                     Qt::AlignCenter, character->getName());

    // HP 条
    int barX = area.x() + 15;
    int barY = area.y() + 45;
    int barW = area.width() - 30;
    int barH = 18;

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(50, 50, 50));
    painter.drawRoundedRect(barX, barY, barW, barH, 4, 4);

    double hpRatio = static_cast<double>(character->getHp()) / character->getMaxHp();
    QColor hpColor = hpRatio > 0.5 ? QColor(60, 160, 60) :
                     hpRatio > 0.25 ? QColor(200, 160, 40) :
                                      QColor(200, 50, 50);
    painter.setBrush(hpColor);
    painter.drawRoundedRect(barX, barY, static_cast<int>(barW * hpRatio), barH, 4, 4);

    painter.setPen(Qt::white);
    QFont hpFont;
    hpFont.setPointSize(10);
    painter.setFont(hpFont);
    painter.drawText(QRect(barX, barY, barW, barH), Qt::AlignCenter,
                     QString("%1/%2").arg(character->getHp()).arg(character->getMaxHp()));

    // 格挡值
    if (character->getBlock() > 0) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(60, 120, 200));
        painter.drawRoundedRect(barX, barY + barH + 4, 70, 18, 4, 4);
        painter.setPen(Qt::white);
        QFont bFont;
        bFont.setPointSize(9);
        painter.setFont(bFont);
        painter.drawText(QRect(barX, barY + barH + 4, 70, 18),
                         Qt::AlignCenter,
                         QString("格挡 %1").arg(character->getBlock()));
    }

    // 敌人意图
    if (!isPlayer) {
        auto *enemy = dynamic_cast<Enemy*>(character);
        if (enemy && !enemy->isDead()) {
            const auto &intent = enemy->getCurrentIntent();
            painter.setPen(QColor(255, 200, 100));
            QFont iFont;
            iFont.setPointSize(10);
            painter.setFont(iFont);
            painter.drawText(QRect(area.x(), area.y() + 100, area.width(), 20),
                             Qt::AlignCenter, intent.description);
        }
    }

    // Buff 图标
    drawBuffIcons(painter, character->getBuffManager(),
                  area.x() + 10, area.y() + 130);
}

void BattleScene::drawBuffIcons(QPainter &painter, const BuffManager &bm,
                                 int x, int y) const
{
    auto &buffs = bm.getAllBuffs();
    int offsetX = 0;
    QFont bFont;
    bFont.setPointSize(9);
    painter.setFont(bFont);

    for (auto &buff : buffs) {
        QColor color;
        switch (buff->getType()) {
        case BuffType::VULNERABLE: color = QColor(200, 100, 50); break;
        case BuffType::WEAK:       color = QColor(100, 100, 200); break;
        case BuffType::POISON:     color = QColor(80, 180, 50); break;
        case BuffType::DOOM:       color = QColor(150, 50, 150); break;
        case BuffType::STRENGTH:   color = buff->getStacks() >= 0 ? QColor(200, 120, 30) : QColor(100, 60, 20); break;
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawRoundedRect(x + offsetX, y, 42, 22, 4, 4);
        painter.setPen(Qt::white);
        painter.drawText(QRect(x + offsetX, y, 42, 22), Qt::AlignCenter,
                         QString("%1%2").arg(buff->getName().left(1)).arg(buff->getStacks()));
        offsetX += 46;
    }
}

void BattleScene::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int handAreaY = height() - 230;

    m_energyLabel->setGeometry(15, handAreaY + 10, 80, 50);
    m_turnLabel->setGeometry(width() - 120, handAreaY + 10, 100, 20);
    m_endTurnBtn->setGeometry(width() - 130, handAreaY + 40, 100, 40);

    m_goldLabel->setGeometry(10, 10, 120, 25);
    m_floorLabel->setGeometry(width() - 200, 10, 80, 25);
    m_exitBtn->setGeometry(width() - 70, 8, 60, 30);

    m_drawPileBtn->setGeometry(width() / 2 - 150, height() - 35, 80, 28);
    m_discardPileBtn->setGeometry(width() / 2 - 40, height() - 35, 80, 28);
    m_exhaustPileBtn->setGeometry(width() / 2 + 70, height() - 35, 80, 28);

    layoutHandCards();
}

void BattleScene::onEndTurnClicked()
{
    if (m_battle && !m_battle->isBattleOver()) {
        m_selectedCard = nullptr;
        m_battle->endPlayerTurn();
    }
}

void BattleScene::onCardClicked(CardWidget *widget)
{
    if (m_selectedCard && m_selectedCard != widget) {
        m_selectedCard->setSelected(false);
    }
    m_selectedCard = widget;
    widget->setSelected(true);
}

void BattleScene::onCardDragFinished(CardWidget *widget, QPoint globalPos)
{
    if (!m_battle || m_battle->isBattleOver() || !widget->getCard()) {
        layoutHandCards();
        return;
    }

    auto card = widget->getCard();
    Character *target = nullptr;

    if (card->getTargetType() == TargetType::SELF) {
        target = m_player;
    } else if (card->getTargetType() == TargetType::ALL_ENEMIES) {
        target = nullptr;
    } else {
        auto alive = m_battle->getAliveEnemies();
        if (alive.size() == 1) {
            target = alive[0];
        } else {
            int idx = hitTestEnemy(globalPos);
            if (idx >= 0 && idx < static_cast<int>(m_enemies.size()) && !m_enemies[idx]->isDead()) {
                target = m_enemies[idx].get();
            }
        }
    }

    bool success = false;
    if (card->getTargetType() == TargetType::SELF || target != nullptr
        || card->getTargetType() == TargetType::ALL_ENEMIES) {
        success = m_battle->playCard(card, target);
    }

    if (!success) {
        layoutHandCards();
    }
}

int BattleScene::hitTestEnemy(QPoint globalPos) const
{
    QPoint localPos = mapFromGlobal(globalPos);
    for (size_t i = 0; i < m_enemyAreas.size(); ++i) {
        if (!m_enemyAreas[i].isNull() && m_enemyAreas[i].contains(localPos)) {
            return static_cast<int>(i);
        }
    }

    int minDist = INT_MAX;
    int closest = -1;
    for (size_t i = 0; i < m_enemyAreas.size(); ++i) {
        if (m_enemyAreas[i].isNull()) continue;
        if (i < m_enemies.size() && m_enemies[i]->isDead()) continue;
        int dist = (m_enemyAreas[i].center() - localPos).manhattanLength();
        if (dist < minDist) {
            minDist = dist;
            closest = static_cast<int>(i);
        }
    }
    return closest;
}

void BattleScene::onHandUpdated()
{
    refreshHandCards();
    update();
}

void BattleScene::onCharacterUpdated()
{
    updateStatusDisplay();
    update();
}

void BattleScene::onEnergyChanged(int current, int max)
{
    m_energyLabel->setText(QString("%1/%2").arg(current).arg(max));
}

void BattleScene::onBattleEnded(bool playerWon)
{
    QString msg = playerWon ? "战斗胜利！" : "战斗失败...";
    QMessageBox::information(this, "战斗结束", msg);
    emit battleFinished(playerWon);
}

void BattleScene::refreshHandCards()
{
    clearHandCards();
    if (!m_player) return;

    auto &hand = m_player->getDeck().getHand();
    int energy = m_battle ? m_battle->getEnergy() : 0;

    for (auto &card : hand) {
        auto *cw = new CardWidget(card, this);
        cw->setPlayable(card->getCost() <= energy);
        connect(cw, &CardWidget::clicked, this, &BattleScene::onCardClicked);
        connect(cw, &CardWidget::dragFinished, this, &BattleScene::onCardDragFinished);
        m_cardWidgets.push_back(cw);
        cw->show();
    }
    layoutHandCards();
}

void BattleScene::layoutHandCards()
{
    if (m_cardWidgets.empty()) return;

    int handAreaY = height() - 200;
    int totalWidth = static_cast<int>(m_cardWidgets.size()) * (GameConfig::CARD_WIDTH + 8);
    int startX = (width() - totalWidth) / 2;

    if (startX < 100) startX = 100;
    int spacing = std::min(GameConfig::CARD_WIDTH + 8,
                           (width() - 200) / static_cast<int>(m_cardWidgets.size()));

    for (size_t i = 0; i < m_cardWidgets.size(); ++i) {
        m_cardWidgets[i]->move(startX + static_cast<int>(i) * spacing, handAreaY);
    }
}

void BattleScene::clearHandCards()
{
    for (auto *cw : m_cardWidgets) {
        cw->deleteLater();
    }
    m_cardWidgets.clear();
    m_selectedCard = nullptr;
}

void BattleScene::updateStatusDisplay()
{
    if (!m_player) return;

    m_goldLabel->setText(QString("金币: %1").arg(m_player->getGold()));
    m_floorLabel->setText(QString("关卡: %1").arg(m_floor));

    if (m_battle) {
        m_energyLabel->setText(QString("%1/%2").arg(m_battle->getEnergy()).arg(m_battle->getMaxEnergy()));
        m_turnLabel->setText(QString("回合 %1").arg(m_battle->getCurrentTurn()));
    }
}
