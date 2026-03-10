#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "MapNode.h"
#include "MapGenerator.h"
#include <QMap>
#include <vector>
#include <memory>

/**
 * @brief 地图管理器
 *
 * 管理当前地图实例、玩家进度和节点导航。
 */
class MapManager
{
public:
    MapManager();

    /** @brief 生成一张新地图 */
    void generateNewMap();

    /**
     * @brief 选择并前往某个节点
     * @return true 表示移动成功
     */
    bool selectNode(int nodeId);

    /** @brief 获取当前可选择的节点列表 */
    std::vector<MapNode*> getAvailableNodes() const;

    /** @brief 获取所有节点 */
    const std::vector<std::shared_ptr<MapNode>>& getAllNodes() const { return m_nodes; }

    /**
     * @brief 通过 ID 获取节点
     */
    MapNode* getNodeById(int id) const;

    /** @brief 获取当前所在节点（若未选择任何节点则为 nullptr） */
    MapNode* getCurrentNode() const;

    /** @brief 获取当前所在层级（-1 表示还未开始） */
    int getCurrentLayer() const { return m_currentLayer; }

    /** @brief 获取总层数 */
    int getTotalLayers() const { return m_totalLayers; }

    /**
     * @brief 获取指定层级的所有节点
     */
    std::vector<MapNode*> getNodesAtLayer(int layer) const;

    /** @brief 地图是否已完成（Boss 已击败） */
    bool isMapComplete() const { return m_mapComplete; }

    void setMapComplete(bool complete) { m_mapComplete = complete; }

private:
    std::vector<std::shared_ptr<MapNode>> m_nodes;  ///< 所有节点
    QMap<int, MapNode*> m_nodeMap;                   ///< ID -> 节点快速查找
    MapGenerator m_generator;

    int m_currentNodeId = -1;    ///< 当前节点 ID
    int m_currentLayer = -1;     ///< 当前层级
    int m_totalLayers = 0;       ///< 总层数
    bool m_mapComplete = false;  ///< 地图是否已通关
};

#endif // MAPMANAGER_H
