#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QPoint>
#include <memory>
#include "core/card/Card.h"

/**
 * @brief 卡牌 UI 组件
 *
 * 通过 QPainter 自绘卡牌外观。
 * 支持点击选中和拖拽使用的交互流程。
 */
class CardWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @param card   关联的卡牌数据
     * @param parent 父组件
     */
    CardWidget(std::shared_ptr<Card> card, QWidget *parent = nullptr);

    std::shared_ptr<Card> getCard() const { return m_card; }

    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }

    void setPlayable(bool playable);
    bool isPlayable() const { return m_playable; }

signals:
    void clicked(CardWidget *widget);
    void dragFinished(CardWidget *widget, QPoint globalPos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    std::shared_ptr<Card> m_card;
    bool m_selected = false;
    bool m_playable = true;
    bool m_hovered = false;
    bool m_dragging = false;
    QPoint m_dragStartPos;
    QPoint m_originalPos;
};

#endif // CARDWIDGET_H
