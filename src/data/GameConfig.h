#ifndef GAMECONFIG_H
#define GAMECONFIG_H

/**
 * @brief 游戏全局常量配置
 *
 * 集中管理所有游戏参数，方便调整平衡性。
 */
namespace GameConfig {

    /** 战斗相关 */
    constexpr int INITIAL_ENERGY = 3;          ///< 每回合初始能量
    constexpr int CARDS_PER_DRAW = 5;          ///< 每回合抽牌数
    constexpr double VULNERABLE_MULTIPLIER = 1.5; ///< 易伤伤害倍率
    constexpr double WEAK_MULTIPLIER = 0.5;       ///< 虚弱伤害倍率 (造成伤害 * 此值 = 实际输出)

    /** 角色初始属性 */
    constexpr int WARRIOR_MAX_HP = 80;         ///< 战士初始最大生命
    constexpr int QUEEN_MAX_HP = 70;           ///< 女王初始最大生命
    constexpr int INITIAL_GOLD = 99;           ///< 初始金币

    /** 地图相关 */
    constexpr int MAP_MIN_LAYERS = 10;         ///< 地图最少层数
    constexpr int MAP_MAX_LAYERS = 15;         ///< 地图最多层数
    constexpr int MAP_MIN_NODES_PER_LAYER = 2; ///< 每层最少节点数
    constexpr int MAP_MAX_NODES_PER_LAYER = 4; ///< 每层最多节点数

    /** 道具相关 */
    constexpr int MAX_POTIONS = 3;             ///< 药水携带上限
    constexpr double BEADS_HEAL_RATIO = 0.3;   ///< 佛珠恢复生命比例
    constexpr double HEAL_POTION_RATIO = 0.1;  ///< 恢复药水生命比例

    /** UI 相关 */
    constexpr int WINDOW_WIDTH = 1280;         ///< 窗口默认宽度
    constexpr int WINDOW_HEIGHT = 720;         ///< 窗口默认高度
    constexpr int CARD_WIDTH = 120;            ///< 卡牌绘制宽度
    constexpr int CARD_HEIGHT = 170;           ///< 卡牌绘制高度

}

#endif // GAMECONFIG_H
