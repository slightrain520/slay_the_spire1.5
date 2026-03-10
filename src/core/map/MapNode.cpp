#include "MapNode.h"

MapNode::MapNode(int id, int layer, int index, NodeType type)
    : m_id(id)
    , m_layer(layer)
    , m_index(index)
    , m_type(type)
{
}

void MapNode::addConnection(int targetNodeId)
{
    m_connections.push_back(targetNodeId);
}

QString MapNode::getTypeName() const
{
    switch (m_type) {
    case NodeType::COMBAT:  return "战斗";
    case NodeType::ELITE:   return "精英";
    case NodeType::REWARD:  return "奖励";
    case NodeType::REST:    return "休息";
    case NodeType::SHOP:    return "商店";
    case NodeType::BOSS:    return "Boss";
    }
    return "未知";
}
