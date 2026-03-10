#ifndef WARRIOR_H
#define WARRIOR_H

#include "Player.h"

/**
 * @brief 战士角色
 *
 * 初始卡牌组以攻击和防御为主，效果侧重易伤和虚弱。
 */
class Warrior : public Player
{
public:
    Warrior();
    void initializeDeck() override;
};

#endif // WARRIOR_H
