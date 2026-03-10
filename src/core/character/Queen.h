#ifndef QUEEN_H
#define QUEEN_H

#include "Player.h"

/**
 * @brief 女王角色
 *
 * 初始卡牌组以攻击和防御为主，效果侧重中毒和灾厄。
 */
class Queen : public Player
{
public:
    Queen();
    void initializeDeck() override;
};

#endif // QUEEN_H
