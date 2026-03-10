#ifndef ENERGYBARRELIC_H
#define ENERGYBARRELIC_H

#include "Relic.h"

/**
 * @brief 能量棒遗物
 *
 * 每回合多获得1点能量。
 */
class EnergyBarRelic : public Relic
{
public:
    EnergyBarRelic();

    int modifyEnergy(int energy) override;
    std::shared_ptr<Relic> clone() const override;
};

#endif // ENERGYBARRELIC_H
