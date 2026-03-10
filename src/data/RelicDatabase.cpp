#include "RelicDatabase.h"
#include "core/item/CrownRelic.h"
#include "core/item/EnergyBarRelic.h"
#include "core/item/BeadsRelic.h"
#include "core/item/CornRelic.h"
#include "core/item/GlovesRelic.h"
#include "core/item/FanRelic.h"
#include "core/item/PassRelic.h"

RelicDatabase& RelicDatabase::instance()
{
    static RelicDatabase db;
    return db;
}

RelicDatabase::RelicDatabase()
{
    registerAll();
}

std::shared_ptr<Relic> RelicDatabase::createRelic(const QString &id) const
{
    if (m_templates.contains(id)) {
        return m_templates[id]->clone();
    }
    return nullptr;
}

QList<QString> RelicDatabase::getAllRelicIds() const
{
    return m_templates.keys();
}

void RelicDatabase::registerAll()
{
    m_templates["crown"]      = std::make_shared<CrownRelic>();
    m_templates["energy_bar"] = std::make_shared<EnergyBarRelic>();
    m_templates["beads"]      = std::make_shared<BeadsRelic>();
    m_templates["corn"]       = std::make_shared<CornRelic>();
    m_templates["gloves"]     = std::make_shared<GlovesRelic>();
    m_templates["fan"]        = std::make_shared<FanRelic>();
    m_templates["pass"]       = std::make_shared<PassRelic>();
}
