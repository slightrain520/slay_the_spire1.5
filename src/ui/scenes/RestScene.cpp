#include "RestScene.h"
#include "core/character/Player.h"
#include <QPainter>
#include <QVBoxLayout>
#include <cmath>

RestScene::RestScene(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    m_titleLabel = new QLabel("休息处", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #E0C068; font-size: 28px; font-weight: bold;");

    m_hpLabel = new QLabel(this);
    m_hpLabel->setAlignment(Qt::AlignCenter);
    m_hpLabel->setStyleSheet("color: #CCCCCC; font-size: 16px;");

    m_restBtn = new QPushButton("休息（恢复30%生命值）", this);
    m_restBtn->setFixedSize(280, 50);
    m_restBtn->setStyleSheet(
        "QPushButton { background-color: #3A6B3A; color: white; border-radius: 8px;"
        "font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #4A8B4A; }");

    m_leaveBtn = new QPushButton("离开", this);
    m_leaveBtn->setFixedSize(120, 40);
    m_leaveBtn->setStyleSheet(
        "QPushButton { background-color: #5A5A6A; color: white; border-radius: 6px;"
        "font-size: 14px; }"
        "QPushButton:hover { background-color: #7A7A8A; }");

    layout->addStretch(2);
    layout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_hpLabel, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(m_restBtn, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(m_leaveBtn, 0, Qt::AlignCenter);
    layout->addStretch(3);

    connect(m_restBtn, &QPushButton::clicked, this, [this]() {
        if (!m_player) return;
        int healAmount = static_cast<int>(std::floor(m_player->getMaxHp() * 0.3));
        m_player->heal(healAmount);
        m_hpLabel->setText(QString("HP: %1/%2").arg(m_player->getHp()).arg(m_player->getMaxHp()));
        m_restBtn->setEnabled(false);
        m_restBtn->setText("已休息");
    });

    connect(m_leaveBtn, &QPushButton::clicked, this, &RestScene::restFinished);
}

void RestScene::initRest(Player *player)
{
    m_player = player;
    m_hpLabel->setText(QString("HP: %1/%2").arg(player->getHp()).arg(player->getMaxHp()));
    m_restBtn->setEnabled(true);
    m_restBtn->setText("休息（恢复30%生命值）");
}

void RestScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(20, 25, 35));
}
