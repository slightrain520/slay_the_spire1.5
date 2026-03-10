#ifndef BEADSRELIC_H
#define BEADSRELIC_H

#include "Relic.h"

/**
 * @brief 佛珠遗物
 *
 * 即将死亡时恢复30%最大生命值，一次性触发后失效。
 */
class BeadsRelic : public Relic
{
public:
    BeadsRelic();

    bool onAboutToDie(Character *player) override;
    std::shared_ptr<Relic> clone() const override;

    bool isUsed() const { return m_used; }

private:
    bool m_used = false;  ///< 是否已触发
};

#endif // BEADSRELIC_H
