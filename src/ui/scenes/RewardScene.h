#ifndef REWARDSCENE_H
#define REWARDSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <memory>

class Player;
class Inventory;
class Card;

/**
 * @brief 奖励场景
 *
 * 战斗胜利后或奖励关卡中显示，提供金币、卡牌、药水、遗物奖励。
 */
class RewardScene : public QWidget
{
    Q_OBJECT

public:
    explicit RewardScene(QWidget *parent = nullptr);

    /**
     * @brief 初始化奖励界面
     * @param player     玩家
     * @param inventory  背包
     * @param goldReward 金币奖励
     * @param isElite    是否为精英战斗奖励（更好的奖励）
     */
    void initReward(Player *player, Inventory *inventory,
                    int goldReward, bool isElite = false);

signals:
    void rewardFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void generateCardChoices();

    Player *m_player = nullptr;
    Inventory *m_inventory = nullptr;
    QLabel *m_titleLabel;
    QLabel *m_goldLabel;
    QPushButton *m_continueBtn;
    std::vector<QPushButton*> m_choiceBtns;
    bool m_cardChosen = false;
};

#endif // REWARDSCENE_H
