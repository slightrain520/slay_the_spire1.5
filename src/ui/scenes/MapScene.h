#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>
#include <vector>
#include "core/map/MapNode.h"

class MapManager;
class Player;

/**
 * @brief 地图场景
 *
 * 可视化展示多层地图，显示节点类型和路径连线。
 * 玩家通过点击可选节点推进游戏。
 */
class MapScene : public QWidget
{
    Q_OBJECT

public:
    explicit MapScene(QWidget *parent = nullptr);

    void setMapManager(MapManager *mgr) { m_mapManager = mgr; }
    void setPlayer(Player *player) { m_player = player; }
    void refreshMap();

signals:
    void nodeSelected(int nodeId);
    void exitRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    struct NodeDisplay {
        int nodeId;
        QRect rect;
        NodeType type;
        bool visited;
        bool available;
    };

    void calculateNodePositions();
    QColor getNodeColor(NodeType type, bool visited, bool available) const;

    MapManager *m_mapManager = nullptr;
    Player *m_player = nullptr;
    std::vector<NodeDisplay> m_nodeDisplays;
    QLabel *m_titleLabel;
    QLabel *m_infoLabel;
    QPushButton *m_exitBtn;
    QScrollBar *m_scrollBar = nullptr;
    int m_scrollOffset = 0;
    int m_totalContentHeight = 0;
};

#endif // MAPSCENE_H
