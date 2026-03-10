#ifndef ENEMYDATABASE_H
#define ENEMYDATABASE_H

#include <QMap>
#include <QString>
#include <vector>
#include <memory>
#include <functional>
#include "core/character/Enemy.h"

/**
 * @brief 敌人数据库（单例）
 *
 * 注册所有敌人模板，通过 ID 创建敌人实例。
 */
class EnemyDatabase
{
public:
    static EnemyDatabase& instance();

    /**
     * @brief 通过 ID 创建一个敌人实例
     */
    std::shared_ptr<Enemy> createEnemy(const QString &id) const;

    /**
     * @brief 获取指定难度的普通怪 ID 列表
     * @param tier 难度等级 (1=简单, 2=中等, 3=困难)
     */
    std::vector<QString> getNormalEnemyIds(int tier) const;

    /**
     * @brief 获取精英怪 ID 列表
     */
    std::vector<QString> getEliteEnemyIds() const;

    /**
     * @brief 获取 Boss ID 列表
     */
    std::vector<QString> getBossIds() const;

private:
    EnemyDatabase();
    void registerAll();

    struct EnemyTemplate {
        QString name;
        int maxHp;
        std::vector<EnemyIntent> intents;
        bool isElite;
        int tier;  ///< 难度等级: 1=普通低级, 2=普通中级, 3=精英, 4=Boss
    };

    QMap<QString, EnemyTemplate> m_templates;
};

#endif // ENEMYDATABASE_H
