#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <memory>

class BattleManager;
class Player;
class Enemy;
class Character;
class CardWidget;
class Inventory;
class BuffManager;

/**
 * @brief 战斗场景
 *
 * 完整的战斗界面：
 * - 顶栏：金币、药水、遗物、关卡号、设置/退出按钮
 * - 中部左侧：玩家（图片+HP+格挡+Buff）
 * - 中部右侧：敌人（图片+HP+意图+Buff）
 * - 底部：能量 | 手牌区（可拖拽） | 回合/结束按钮
 * - 底栏：查看抽牌堆/弃牌堆/消耗堆
 */
class BattleScene : public QWidget
{
    Q_OBJECT

public:
    explicit BattleScene(QWidget *parent = nullptr);

    /** @brief 初始化战斗显示 */
    void initBattle(BattleManager *battle, Player *player,
                    std::vector<std::shared_ptr<Enemy>> enemies,
                    Inventory *inventory, int floor);

signals:
    void battleFinished(bool playerWon);
    void exitRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void onEndTurnClicked();
    void onCardClicked(CardWidget *widget);
    void onCardDragFinished(CardWidget *widget, QPoint globalPos);
    void onHandUpdated();
    void onCharacterUpdated();
    void onEnergyChanged(int current, int max);
    void onBattleEnded(bool playerWon);

private:
    void refreshHandCards();
    void layoutHandCards();
    void clearHandCards();
    void updateStatusDisplay();

    /** @brief 判断全局坐标落在哪个敌人区域上 */
    int hitTestEnemy(QPoint globalPos) const;

    /** @brief 绘制角色信息（HP条、Buff等） */
    void drawCharacterInfo(QPainter &painter, Character *character,
                           QRect area, bool isPlayer) const;

    /** @brief 绘制 Buff 图标 */
    void drawBuffIcons(QPainter &painter, const BuffManager &bm,
                       int x, int y) const;

    BattleManager *m_battle = nullptr;
    Player *m_player = nullptr;
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    Inventory *m_inventory = nullptr;
    int m_floor = 1;

    QPushButton *m_endTurnBtn;
    QPushButton *m_drawPileBtn;
    QPushButton *m_discardPileBtn;
    QPushButton *m_exhaustPileBtn;
    QPushButton *m_exitBtn;
    QLabel *m_energyLabel;
    QLabel *m_turnLabel;
    QLabel *m_goldLabel;
    QLabel *m_floorLabel;

    std::vector<CardWidget*> m_cardWidgets;
    CardWidget *m_selectedCard = nullptr;

    QRect m_playerArea;
    std::vector<QRect> m_enemyAreas;

    // 药水与遗物交互
    int m_pendingPotionIdx = -1;    ///< 正在等待目标选择的药水槽索引（-1 表示无）
    int m_hoveredPotionIdx = -1;    ///< 鼠标悬停的药水槽索引
};

#endif // BATTLESCENE_H
