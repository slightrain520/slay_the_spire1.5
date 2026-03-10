#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "MapNode.h"
#include <vector>
#include <memory>
#include <random>

/**
 * @brief 地图生成器
 *
 * 随机生成具有分支路径的多层地图。
 * 每层 2-4 个节点，层间有交织连接，最后一层为 Boss。
 */
class MapGenerator
{
public:
    MapGenerator();

    /**
     * @brief 生成一张新地图
     * @return 所有节点的列表
     */
    std::vector<std::shared_ptr<MapNode>> generate();

private:
    /**
     * @brief 决定某一层的节点类型
     * @param layer      当前层
     * @param totalLayers 总层数
     */
    NodeType decideNodeType(int layer, int totalLayers);

    /**
     * @brief 在相邻层间建立连接
     */
    void connectLayers(std::vector<std::shared_ptr<MapNode>> &currentLayer,
                       std::vector<std::shared_ptr<MapNode>> &nextLayer);

    std::mt19937 m_rng;
    int m_nextId = 0;
};

#endif // MAPGENERATOR_H
