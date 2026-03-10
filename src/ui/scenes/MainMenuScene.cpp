#include "MainMenuScene.h"
#include <QPainter>
#include <QFont>

MainMenuScene::MainMenuScene(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    auto *titleLabel = new QLabel("杀戮尖塔 1.5", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(36);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #E0C068;");

    auto *subtitleLabel = new QLabel("Slay The Spire", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subFont;
    subFont.setPointSize(16);
    subtitleLabel->setFont(subFont);
    subtitleLabel->setStyleSheet("color: #AAAAAA;");

    m_startBtn = new QPushButton("开始游戏", this);
    m_startBtn->setFixedSize(200, 50);
    m_startBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #4A6B3A;"
        "  color: white;"
        "  border: 2px solid #6B8F5A;"
        "  border-radius: 8px;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #5A8B4A;"
        "}"
    );

    m_exitBtn = new QPushButton("退出游戏", this);
    m_exitBtn->setFixedSize(200, 50);
    m_exitBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #6B3A3A;"
        "  color: white;"
        "  border: 2px solid #8F5A5A;"
        "  border-radius: 8px;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #8B4A4A;"
        "}"
    );

    layout->addStretch(2);
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addWidget(subtitleLabel, 0, Qt::AlignCenter);
    layout->addSpacing(60);
    layout->addWidget(m_startBtn, 0, Qt::AlignCenter);
    layout->addWidget(m_exitBtn, 0, Qt::AlignCenter);
    layout->addStretch(3);

    connect(m_startBtn, &QPushButton::clicked, this, &MainMenuScene::startGameClicked);
    connect(m_exitBtn, &QPushButton::clicked, this, &MainMenuScene::exitGameClicked);
}

void MainMenuScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(30, 30, 40));
}
