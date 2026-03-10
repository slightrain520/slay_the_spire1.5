#ifndef MAPNODE_H
#define MAPNODE_H

#include <vector>
#include <QString>

/**
 * @brief 地图节点类型
 */
enum class NodeType {
    COMBAT,   ///< 普通战斗
    ELITE,    ///< 精英战斗
    REWARD,   ///< 奖励关
    REST,     ///< 休息处
    SHOP,     ///< 商店
    BOSS      ///< Boss 战
};

/**
 * @brief 地图节点
 *
 * 代表地图上的一个位置，包含类型、层级、连接关系和状态。
 */
class MapNode
{
public:
    /**
     * @param id    节点唯一 ID
     * @param layer 所在层级（0 = 起始层）
     * @param index 层内索引
     * @param type  节点类型
     */
    MapNode(int id, int layer, int index, NodeType type);

    /** @brief 添加与下一层节点的连接 */
    void addConnection(int targetNodeId);

    // ─── Getters / Setters ──────────────────────────
    int getId() const { return m_id; }
    int getLayer() const { return m_layer; }
    int getIndex() const { return m_index; }
    NodeType getType() const { return m_type; }
    bool isVisited() const { return m_visited; }
    void setVisited(bool v) { m_visited = v; }
    const std::vector<int>& getConnections() const { return m_connections; }

    /** @brief 获取节点类型的中文名称 */
    QString getTypeName() const;

private:
    int m_id;                       ///< 唯一标识
    int m_layer;                    ///< 层级
    int m_index;                    ///< 层内索引
    NodeType m_type;                ///< 节点类型
    bool m_visited = false;         ///< 是否已访问
    std::vector<int> m_connections; ///< 连接的下一层节点 ID
};

#endif // MAPNODE_H
