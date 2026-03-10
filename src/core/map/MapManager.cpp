#include "MapManager.h"
#include <algorithm>

MapManager::MapManager()
{
}

void MapManager::generateNewMap()
{
    m_nodes = m_generator.generate();
    m_nodeMap.clear();
    m_currentNodeId = -1;
    m_currentLayer = -1;
    m_mapComplete = false;

    int maxLayer = 0;
    for (auto &node : m_nodes) {
        m_nodeMap[node->getId()] = node.get();
        maxLayer = std::max(maxLayer, node->getLayer());
    }
    m_totalLayers = maxLayer + 1;
}

bool MapManager::selectNode(int nodeId)
{
    auto available = getAvailableNodes();
    bool isAvailable = false;
    for (auto *node : available) {
        if (node->getId() == nodeId) {
            isAvailable = true;
            break;
        }
    }
    if (!isAvailable) return false;

    MapNode *node = getNodeById(nodeId);
    if (!node) return false;

    node->setVisited(true);
    m_currentNodeId = nodeId;
    m_currentLayer = node->getLayer();

    if (node->getType() == NodeType::BOSS) {
        m_mapComplete = true;
    }

    return true;
}

std::vector<MapNode*> MapManager::getAvailableNodes() const
{
    std::vector<MapNode*> result;

    if (m_currentLayer == -1) {
        for (auto &node : m_nodes) {
            if (node->getLayer() == 0) {
                result.push_back(node.get());
            }
        }
        return result;
    }

    MapNode *current = getNodeById(m_currentNodeId);
    if (!current) return result;

    for (int connId : current->getConnections()) {
        MapNode *next = getNodeById(connId);
        if (next && !next->isVisited()) {
            result.push_back(next);
        }
    }
    return result;
}

MapNode* MapManager::getNodeById(int id) const
{
    if (m_nodeMap.contains(id)) {
        return m_nodeMap[id];
    }
    return nullptr;
}

MapNode* MapManager::getCurrentNode() const
{
    return getNodeById(m_currentNodeId);
}

std::vector<MapNode*> MapManager::getNodesAtLayer(int layer) const
{
    std::vector<MapNode*> result;
    for (auto &node : m_nodes) {
        if (node->getLayer() == layer) {
            result.push_back(node.get());
        }
    }
    return result;
}
