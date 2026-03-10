#include "MapGenerator.h"
#include "data/GameConfig.h"
#include <algorithm>
#include <chrono>
#include <set>

MapGenerator::MapGenerator()
    : m_rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
{
}

std::vector<std::shared_ptr<MapNode>> MapGenerator::generate()
{
    m_nextId = 0;
    std::vector<std::shared_ptr<MapNode>> allNodes;

    std::uniform_int_distribution<int> layerDist(GameConfig::MAP_MIN_LAYERS, GameConfig::MAP_MAX_LAYERS);
    int totalLayers = layerDist(m_rng);

    std::vector<std::vector<std::shared_ptr<MapNode>>> layers;

    for (int layer = 0; layer < totalLayers; ++layer) {
        std::vector<std::shared_ptr<MapNode>> layerNodes;

        int nodeCount;
        if (layer == totalLayers - 1) {
            nodeCount = 1;
        } else {
            std::uniform_int_distribution<int> nodeDist(
                GameConfig::MAP_MIN_NODES_PER_LAYER,
                GameConfig::MAP_MAX_NODES_PER_LAYER);
            nodeCount = nodeDist(m_rng);
        }

        for (int i = 0; i < nodeCount; ++i) {
            NodeType type;
            if (layer == totalLayers - 1) {
                type = NodeType::BOSS;
            } else {
                type = decideNodeType(layer, totalLayers);
            }

            auto node = std::make_shared<MapNode>(m_nextId++, layer, i, type);
            layerNodes.push_back(node);
            allNodes.push_back(node);
        }

        layers.push_back(layerNodes);
    }

    for (int i = 0; i < static_cast<int>(layers.size()) - 1; ++i) {
        connectLayers(layers[i], layers[i + 1]);
    }

    return allNodes;
}

NodeType MapGenerator::decideNodeType(int layer, int totalLayers)
{
    double progress = static_cast<double>(layer) / totalLayers;

    std::uniform_real_distribution<double> roll(0.0, 1.0);
    double r = roll(m_rng);

    if (layer == 0) {
        return NodeType::COMBAT;
    }

    if (progress < 0.3) {
        if (r < 0.60) return NodeType::COMBAT;
        if (r < 0.80) return NodeType::REWARD;
        return NodeType::REST;
    } else if (progress < 0.6) {
        if (r < 0.35) return NodeType::COMBAT;
        if (r < 0.55) return NodeType::ELITE;
        if (r < 0.70) return NodeType::SHOP;
        if (r < 0.85) return NodeType::REST;
        return NodeType::REWARD;
    } else {
        if (r < 0.30) return NodeType::ELITE;
        if (r < 0.50) return NodeType::COMBAT;
        if (r < 0.65) return NodeType::SHOP;
        if (r < 0.80) return NodeType::REST;
        return NodeType::REWARD;
    }
}

void MapGenerator::connectLayers(std::vector<std::shared_ptr<MapNode>> &currentLayer,
                                  std::vector<std::shared_ptr<MapNode>> &nextLayer)
{
    int curSize = static_cast<int>(currentLayer.size());
    int nextSize = static_cast<int>(nextLayer.size());

    std::set<int> connectedNext;

    for (int i = 0; i < curSize; ++i) {
        int primaryTarget = std::min(i, nextSize - 1);
        currentLayer[i]->addConnection(nextLayer[primaryTarget]->getId());
        connectedNext.insert(primaryTarget);

        std::uniform_real_distribution<double> roll(0.0, 1.0);
        if (roll(m_rng) < 0.4 && nextSize > 1) {
            int altTarget;
            if (primaryTarget + 1 < nextSize) {
                altTarget = primaryTarget + 1;
            } else {
                altTarget = primaryTarget - 1;
            }
            currentLayer[i]->addConnection(nextLayer[altTarget]->getId());
            connectedNext.insert(altTarget);
        }
    }

    for (int j = 0; j < nextSize; ++j) {
        if (connectedNext.find(j) == connectedNext.end()) {
            int source = std::min(j, curSize - 1);
            currentLayer[source]->addConnection(nextLayer[j]->getId());
        }
    }
}
