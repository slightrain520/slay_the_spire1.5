#ifndef POTIONDATABASE_H
#define POTIONDATABASE_H

#include <QMap>
#include <QString>
#include <memory>
#include "core/item/Potion.h"

/**
 * @brief 药水数据库（单例）
 */
class PotionDatabase
{
public:
    static PotionDatabase& instance();

    std::shared_ptr<Potion> createPotion(const QString &id) const;
    QList<QString> getAllPotionIds() const;

private:
    PotionDatabase();
    void registerAll();

    QMap<QString, std::shared_ptr<Potion>> m_templates;
};

#endif // POTIONDATABASE_H
