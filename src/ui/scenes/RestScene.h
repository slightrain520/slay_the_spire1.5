#ifndef RESTSCENE_H
#define RESTSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Player;

/**
 * @brief 休息处场景
 *
 * 提供恢复生命和升级卡牌（当前简化为额外恢复）功能。
 */
class RestScene : public QWidget
{
    Q_OBJECT

public:
    explicit RestScene(QWidget *parent = nullptr);

    void initRest(Player *player);

signals:
    void restFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Player *m_player = nullptr;
    QLabel *m_titleLabel;
    QLabel *m_hpLabel;
    QPushButton *m_restBtn;
    QPushButton *m_leaveBtn;
};

#endif // RESTSCENE_H
