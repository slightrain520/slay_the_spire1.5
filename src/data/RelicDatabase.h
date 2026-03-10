#ifndef RELICDATABASE_H
#define RELICDATABASE_H

#include <QMap>
#include <QString>
#include <memory>
#include <vector>
#include "core/item/Relic.h"

/**
 * @brief 遗物数据库（单例）
 */
class RelicDatabase
{
public:
    static RelicDatabase& instance();

    std::shared_ptr<Relic> createRelic(const QString &id) const;
    QList<QString> getAllRelicIds() const;

private:
    RelicDatabase();
    void registerAll();

    QMap<QString, std::shared_ptr<Relic>> m_templates;
};

#endif // RELICDATABASE_H
