#include "EnergyBarRelic.h"

EnergyBarRelic::EnergyBarRelic()
    : Relic("energy_bar", "能量棒", "每回合多获得1点能量")
{
}

int EnergyBarRelic::modifyEnergy(int energy)
{
    return energy + 1;
}

std::shared_ptr<Relic> EnergyBarRelic::clone() const
{
    return std::make_shared<EnergyBarRelic>();
}
