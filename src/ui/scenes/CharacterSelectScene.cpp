#include "CharacterSelectScene.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

CharacterSelectScene::CharacterSelectScene(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    auto *titleLabel = new QLabel("选择你的角色", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #E0C068;");

    auto *charLayout = new QHBoxLayout();
    charLayout->setSpacing(60);

    auto createCharBtn = [this](const QString &name, const QString &desc, const QString &color) {
        auto *frame = new QWidget(this);
        auto *layout = new QVBoxLayout(frame);
        layout->setAlignment(Qt::AlignCenter);

        auto *nameLabel = new QLabel(name, frame);
        nameLabel->setAlignment(Qt::AlignCenter);
        QFont f;
        f.setPointSize(20);
        f.setBold(true);
        nameLabel->setFont(f);
        nameLabel->setStyleSheet("color: white;");

        auto *descLabel = new QLabel(desc, frame);
        descLabel->setAlignment(Qt::AlignCenter);
        descLabel->setWordWrap(true);
        descLabel->setFixedWidth(200);
        descLabel->setStyleSheet("color: #CCCCCC; font-size: 13px;");

        auto *btn = new QPushButton("选择", frame);
        btn->setFixedSize(150, 45);
        btn->setStyleSheet(
            QString("QPushButton { background-color: %1; color: white; border: 2px solid #888;"
                    "border-radius: 8px; font-size: 16px; font-weight: bold; }"
                    "QPushButton:hover { background-color: %1; border: 2px solid white; }")
                .arg(color));

        layout->addWidget(nameLabel);
        layout->addSpacing(10);
        layout->addWidget(descLabel);
        layout->addSpacing(15);
        layout->addWidget(btn, 0, Qt::AlignCenter);

        frame->setFixedSize(250, 300);
        frame->setStyleSheet(
            QString("QWidget { background-color: rgba(60,60,80,180); border: 2px solid %1;"
                    "border-radius: 12px; }").arg(color));

        return std::make_pair(frame, btn);
    };

    auto [warriorFrame, warriorBtn] = createCharBtn(
        "战士", "HP: 80\n初始牌组以攻击防御为主\n效果侧重易伤和虚弱", "#B8860B");
    auto [queenFrame, queenBtn] = createCharBtn(
        "女王", "HP: 70\n初始牌组以攻击防御为主\n效果侧重中毒和灾厄", "#8B008B");

    m_warriorBtn = warriorBtn;
    m_queenBtn = queenBtn;

    charLayout->addWidget(warriorFrame);
    charLayout->addWidget(queenFrame);

    m_backBtn = new QPushButton("返回", this);
    m_backBtn->setFixedSize(120, 40);
    m_backBtn->setStyleSheet(
        "QPushButton { background-color: #555; color: white; border-radius: 6px; font-size: 14px; }"
        "QPushButton:hover { background-color: #777; }"
    );

    mainLayout->addStretch(1);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(charLayout);
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_backBtn, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);

    connect(m_warriorBtn, &QPushButton::clicked, this, [this]() { emit characterSelected(0); });
    connect(m_queenBtn, &QPushButton::clicked, this, [this]() { emit characterSelected(1); });
    connect(m_backBtn, &QPushButton::clicked, this, &CharacterSelectScene::backClicked);
}

void CharacterSelectScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(25, 25, 35));
}
