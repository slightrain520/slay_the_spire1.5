#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QMainWindow>
#include <QStackedWidget>

/**
 * @brief 场景管理器 / 主窗口
 *
 * 继承 QMainWindow，内部使用 QStackedWidget 管理多个游戏场景。
 * 提供场景注册、切换的统一接口。
 */
class SceneManager : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 场景标识枚举
     */
    enum SceneId {
        MAIN_MENU = 0,       ///< 主菜单
        CHARACTER_SELECT,     ///< 角色选择
        MAP,                  ///< 地图
        BATTLE,               ///< 战斗
        SHOP,                 ///< 商店
        REST,                 ///< 休息处
        REWARD                ///< 奖励
    };

    explicit SceneManager(QWidget *parent = nullptr);
    ~SceneManager() override = default;

    /**
     * @brief 切换到指定场景
     * @param id 场景标识
     */
    void switchTo(SceneId id);

    /**
     * @brief 注册一个场景 widget
     * @param id 场景标识
     * @param widget 场景 widget 指针（所有权转移给 QStackedWidget）
     */
    void registerScene(SceneId id, QWidget *widget);

private:
    QStackedWidget *m_stack;                  ///< 场景堆叠容器
    QMap<SceneId, int> m_sceneIndexMap;       ///< 场景 ID 到 stack index 的映射
};

#endif // SCENEMANAGER_H
