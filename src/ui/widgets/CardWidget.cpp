#include "CardWidget.h"
#include "data/GameConfig.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

CardWidget::CardWidget(std::shared_ptr<Card> card, QWidget *parent)
    : QWidget(parent)
    , m_card(card)
{
    setFixedSize(GameConfig::CARD_WIDTH, GameConfig::CARD_HEIGHT);
    setCursor(Qt::PointingHandCursor);
}

void CardWidget::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void CardWidget::setPlayable(bool playable)
{
    m_playable = playable;
    update();
}

void CardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor;
    if (m_card->getCardType() == CardType::ATTACK) {
        bgColor = QColor(140, 50, 50);
    } else {
        bgColor = QColor(50, 80, 140);
    }

    if (!m_playable) {
        bgColor = bgColor.darker(150);
    }

    QColor borderColor = m_selected ? QColor(255, 215, 0) :
                         m_hovered  ? QColor(200, 200, 200) :
                                      QColor(100, 100, 100);
    int borderWidth = m_selected ? 3 : 2;

    painter.setPen(QPen(borderColor, borderWidth));
    painter.setBrush(bgColor);
    painter.drawRoundedRect(1, 1, width() - 2, height() - 2, 8, 8);

    // 能量消耗（左上角圆圈）
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(30, 30, 50));
    painter.drawEllipse(6, 6, 28, 28);
    painter.setPen(QColor(200, 200, 255));
    QFont costFont;
    costFont.setPointSize(14);
    costFont.setBold(true);
    painter.setFont(costFont);
    painter.drawText(QRect(6, 6, 28, 28), Qt::AlignCenter,
                     QString::number(m_card->getCost()));

    // 卡牌名称
    painter.setPen(Qt::white);
    QFont nameFont;
    nameFont.setPointSize(11);
    nameFont.setBold(true);
    painter.setFont(nameFont);
    painter.drawText(QRect(5, 38, width() - 10, 24), Qt::AlignCenter,
                     m_card->getName());

    // 类型标签
    painter.setPen(QColor(180, 180, 180));
    QFont typeFont;
    typeFont.setPointSize(9);
    painter.setFont(typeFont);
    QString typeStr = (m_card->getCardType() == CardType::ATTACK) ? "攻击" : "技能";
    painter.drawText(QRect(5, 64, width() - 10, 18), Qt::AlignCenter, typeStr);

    // 分割线
    painter.setPen(QPen(QColor(120, 120, 120), 1));
    painter.drawLine(10, 85, width() - 10, 85);

    // 描述文字
    painter.setPen(QColor(220, 220, 220));
    QFont descFont;
    descFont.setPointSize(9);
    painter.setFont(descFont);
    painter.drawText(QRect(8, 90, width() - 16, height() - 118),
                     Qt::AlignHCenter | Qt::TextWordWrap,
                     m_card->getDescription());

    // 关键字徽章（卡牌底部）
    QStringList keywords;
    if (m_card->isExhaust())           keywords << "消耗";
    if (m_card->isInnate())            keywords << "固有";
    if (m_card->isRetain())            keywords << "保留";
    if (m_card->getRepeat() > 0)       keywords << QString("重放%1").arg(m_card->getRepeat());

    if (!keywords.isEmpty()) {
        QFont kwFont;
        kwFont.setPointSize(7);
        kwFont.setBold(true);
        painter.setFont(kwFont);

        int badgeY = height() - 22;
        int badgeX = 5;
        for (auto &kw : keywords) {
            QRect badgeRect(badgeX, badgeY, 34, 15);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(60, 60, 80));
            painter.drawRoundedRect(badgeRect, 4, 4);
            painter.setPen(QColor(220, 180, 80));
            painter.drawText(badgeRect, Qt::AlignCenter, kw);
            badgeX += 37;
        }
    }
}

void CardWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_playable) {
        m_dragStartPos = event->pos();
        m_originalPos = pos();
        emit clicked(this);
    }
}

void CardWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton) || !m_selected || !m_playable) return;

    if (!m_dragging) {
        int dist = (event->pos() - m_dragStartPos).manhattanLength();
        if (dist < QApplication::startDragDistance()) return;
        m_dragging = true;
        raise();
    }

    QPoint delta = event->pos() - m_dragStartPos;
    move(pos() + delta);
}

void CardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        emit dragFinished(this, mapToGlobal(event->pos()));
    }
}

void CardWidget::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)
    m_hovered = true;
    update();
}

void CardWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_hovered = false;
    update();
}
