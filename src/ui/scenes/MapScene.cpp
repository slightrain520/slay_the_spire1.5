#include "MapScene.h"
#include "core/map/MapManager.h"
#include "core/map/MapNode.h"
#include "core/character/Player.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFont>
#include <algorithm>

MapScene::MapScene(QWidget *parent)
    : QWidget(parent)
{
    m_titleLabel = new QLabel("选择下一个节点", this);
    m_titleLabel->setStyleSheet("color: #E0C068; font-size: 20px; font-weight: bold;");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_infoLabel = new QLabel(this);
    m_infoLabel->setStyleSheet("color: #AAAAAA; font-size: 13px;");
    m_infoLabel->setAlignment(Qt::AlignLeft);

    m_exitBtn = new QPushButton("退出", this);
    m_exitBtn->setFixedSize(60, 30);
    m_exitBtn->setStyleSheet(
        "QPushButton { background-color: #6B3A3A; color: white; border-radius: 4px; font-size: 12px; }"
        "QPushButton:hover { background-color: #8B4A4A; }");
    connect(m_exitBtn, &QPushButton::clicked, this, &MapScene::exitRequested);

    m_scrollBar = new QScrollBar(Qt::Vertical, this);
    m_scrollBar->setRange(0, 0);
    m_scrollBar->setSingleStep(20);
    m_scrollBar->setPageStep(100);
    connect(m_scrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        m_scrollOffset = value;
        calculateNodePositions();
        update();
    });
}

void MapScene::refreshMap()
{
    calculateNodePositions();

    if (m_player) {
        m_infoLabel->setText(QString("HP: %1/%2  金币: %3")
                                 .arg(m_player->getHp())
                                 .arg(m_player->getMaxHp())
                                 .arg(m_player->getGold()));
    }

    // 更新滚动条范围
    if (m_scrollBar && m_mapManager) {
        int maxScroll = qMax(0, m_totalContentHeight - height() + 40);
        int oldMax = m_scrollBar->maximum();
        m_scrollBar->setRange(0, maxScroll);
        if (oldMax == 0) {
            // 初始化时将滚动位置重置到顶部
            m_scrollOffset = 0;
            m_scrollBar->setValue(0);
        }
    }

    update();
}

void MapScene::calculateNodePositions()
{
    m_nodeDisplays.clear();
    if (!m_mapManager) return;

    int totalLayers = m_mapManager->getTotalLayers();
    auto available = m_mapManager->getAvailableNodes();

    int nodeW = 80;
    int nodeH = 40;
    int layerSpacing = 60;
    int topMargin = 70;

    m_totalContentHeight = topMargin + totalLayers * layerSpacing + 60;

    for (int layer = 0; layer < totalLayers; ++layer) {
        auto nodes = m_mapManager->getNodesAtLayer(layer);
        int count = static_cast<int>(nodes.size());
        int totalW = count * nodeW + (count - 1) * 30;
        int startX = (width() - totalW) / 2;

        for (int i = 0; i < count; ++i) {
            NodeDisplay nd;
            nd.nodeId = nodes[i]->getId();
            nd.type = nodes[i]->getType();
            nd.visited = nodes[i]->isVisited();
            nd.available = false;

            for (auto *av : available) {
                if (av->getId() == nd.nodeId) {
                    nd.available = true;
                    break;
                }
            }

            int x = startX + i * (nodeW + 30);
            int y = topMargin + layer * layerSpacing - m_scrollOffset;
            nd.rect = QRect(x, y, nodeW, nodeH);
            m_nodeDisplays.push_back(nd);
        }
    }
}

void MapScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(20, 25, 35));

    m_titleLabel->setGeometry(0, 10, width() - 20, 30);
    m_infoLabel->setGeometry(15, 45, 300, 20);
    m_exitBtn->setGeometry(width() - 70, 10, 60, 30);
    if (m_scrollBar) {
        m_scrollBar->setGeometry(width() - 16, 40, 16, height() - 40);
    }

    if (!m_mapManager) return;

    // 绘制连线
    painter.setPen(QPen(QColor(80, 80, 100), 2));
    for (auto &node : m_mapManager->getAllNodes()) {
        QRect srcRect;
        for (auto &nd : m_nodeDisplays) {
            if (nd.nodeId == node->getId()) {
                srcRect = nd.rect;
                break;
            }
        }
        if (srcRect.isNull()) continue;

        for (int connId : node->getConnections()) {
            QRect dstRect;
            for (auto &nd : m_nodeDisplays) {
                if (nd.nodeId == connId) {
                    dstRect = nd.rect;
                    break;
                }
            }
            if (dstRect.isNull()) continue;

            painter.drawLine(srcRect.center().x(), srcRect.bottom(),
                             dstRect.center().x(), dstRect.top());
        }
    }

    // 绘制节点
    QFont nodeFont;
    nodeFont.setPointSize(10);
    nodeFont.setBold(true);
    painter.setFont(nodeFont);

    for (auto &nd : m_nodeDisplays) {
        QColor bgColor = getNodeColor(nd.type, nd.visited, nd.available);
        QColor borderColor = nd.available ? QColor(255, 215, 0) :
                             nd.visited   ? QColor(100, 100, 100) :
                                            QColor(80, 80, 80);

        painter.setPen(QPen(borderColor, nd.available ? 3 : 1));
        painter.setBrush(bgColor);
        painter.drawRoundedRect(nd.rect, 8, 8);

        painter.setPen(nd.visited ? QColor(120, 120, 120) : Qt::white);
        MapNode *node = m_mapManager->getNodeById(nd.nodeId);
        if (node) {
            painter.drawText(nd.rect, Qt::AlignCenter, node->getTypeName());
        }
    }
}

void MapScene::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    m_scrollOffset -= delta / 4;
    int maxScroll = qMax(0, m_totalContentHeight - height() + 40);
    m_scrollOffset = qBound(0, m_scrollOffset, maxScroll);
    if (m_scrollBar) {
        m_scrollBar->blockSignals(true);
        m_scrollBar->setValue(m_scrollOffset);
        m_scrollBar->blockSignals(false);
    }
    calculateNodePositions();
    update();
    event->accept();
}

void MapScene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;

    QPoint pos = event->pos();
    for (auto &nd : m_nodeDisplays) {
        if (nd.available && nd.rect.contains(pos)) {
            emit nodeSelected(nd.nodeId);
            return;
        }
    }
}

QColor MapScene::getNodeColor(NodeType type, bool visited, bool available) const
{
    if (visited) return QColor(50, 50, 55);

    int alpha = available ? 255 : 150;
    switch (type) {
    case NodeType::COMBAT:  return QColor(120, 50, 50, alpha);
    case NodeType::ELITE:   return QColor(160, 80, 30, alpha);
    case NodeType::REWARD:  return QColor(50, 120, 50, alpha);
    case NodeType::REST:    return QColor(50, 80, 120, alpha);
    case NodeType::SHOP:    return QColor(120, 100, 30, alpha);
    case NodeType::BOSS:    return QColor(150, 30, 30, alpha);
    }
    return QColor(60, 60, 60, alpha);
}
