#include <QApplication>
#include <QMessageBox>
#include <random>
#include <chrono>

#include "ui/SceneManager.h"
#include "ui/scenes/MainMenuScene.h"
#include "ui/scenes/CharacterSelectScene.h"
#include "ui/scenes/MapScene.h"
#include "ui/scenes/BattleScene.h"
#include "ui/scenes/ShopScene.h"
#include "ui/scenes/RestScene.h"
#include "ui/scenes/RewardScene.h"
#include "core/GameManager.h"
#include "core/character/Enemy.h"
#include "core/map/MapNode.h"
#include "data/EnemyDatabase.h"
#include "data/GameConfig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Slay The Spire 1.5");

    SceneManager window;
    window.resize(GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT);
    window.setWindowTitle("杀戮尖塔 1.5");
    window.setMinimumSize(1024, 600);

    auto *mainMenu     = new MainMenuScene();
    auto *charSelect   = new CharacterSelectScene();
    auto *mapScene     = new MapScene();
    auto *battleScene  = new BattleScene();
    auto *shopScene    = new ShopScene();
    auto *restScene    = new RestScene();
    auto *rewardScene  = new RewardScene();

    window.registerScene(SceneManager::MAIN_MENU, mainMenu);
    window.registerScene(SceneManager::CHARACTER_SELECT, charSelect);
    window.registerScene(SceneManager::MAP, mapScene);
    window.registerScene(SceneManager::BATTLE, battleScene);
    window.registerScene(SceneManager::SHOP, shopScene);
    window.registerScene(SceneManager::REST, restScene);
    window.registerScene(SceneManager::REWARD, rewardScene);

    auto &gm = GameManager::instance();

    // 主菜单 -> 角色选择
    QObject::connect(mainMenu, &MainMenuScene::startGameClicked, &window, [&]() {
        window.switchTo(SceneManager::CHARACTER_SELECT);
    });
    QObject::connect(mainMenu, &MainMenuScene::exitGameClicked, &app, &QApplication::quit);

    // 角色选择 -> 返回 / 地图
    QObject::connect(charSelect, &CharacterSelectScene::backClicked, &window, [&]() {
        window.switchTo(SceneManager::MAIN_MENU);
    });
    QObject::connect(charSelect, &CharacterSelectScene::characterSelected, &window, [&](int index) {
        gm.startNewGame(index);
        mapScene->setMapManager(&gm.getMapManager());
        mapScene->setPlayer(gm.getPlayer());
        mapScene->refreshMap();
        window.switchTo(SceneManager::MAP);
    });

    // 回到地图的通用 lambda
    auto returnToMap = [&]() {
        mapScene->refreshMap();
        window.switchTo(SceneManager::MAP);
    };

    // 地图节点选择 -> 进入对应场景
    QObject::connect(mapScene, &MapScene::nodeSelected, &window, [&](int nodeId) {
        auto &mm = gm.getMapManager();
        if (!mm.selectNode(nodeId)) return;

        MapNode *node = mm.getNodeById(nodeId);
        if (!node) return;

        auto *player = gm.getPlayer();
        auto &inv = gm.getInventory();

        std::mt19937 rng(static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));

        switch (node->getType()) {
        case NodeType::COMBAT:
        case NodeType::ELITE:
        case NodeType::BOSS: {
            auto &edb = EnemyDatabase::instance();
            std::vector<std::shared_ptr<Enemy>> enemies;

            if (node->getType() == NodeType::BOSS) {
                auto bossIds = edb.getBossIds();
                if (!bossIds.empty()) {
                    std::uniform_int_distribution<int> dist(0, bossIds.size() - 1);
                    enemies.push_back(edb.createEnemy(bossIds[dist(rng)]));
                }
            } else if (node->getType() == NodeType::ELITE) {
                // 通行证遗物：跳过精英战斗，直接领取奖励
                if (inv.shouldSkipNextElite()) {
                    inv.consumeSkipElite();
                    int gold = 20 + rng() % 20;
                    rewardScene->initReward(player, &inv, gold, true);
                    window.switchTo(SceneManager::REWARD);
                    return;
                }
                auto eliteIds = edb.getEliteEnemyIds();
                if (!eliteIds.empty()) {
                    std::uniform_int_distribution<int> dist(0, eliteIds.size() - 1);
                    enemies.push_back(edb.createEnemy(eliteIds[dist(rng)]));
                }
            } else {
                double progress = static_cast<double>(mm.getCurrentLayer()) / mm.getTotalLayers();
                int tier = progress < 0.4 ? 1 : 2;
                auto normalIds = edb.getNormalEnemyIds(tier);
                if (!normalIds.empty()) {
                    std::uniform_int_distribution<int> dist(0, normalIds.size() - 1);
                    int count = (tier == 1) ? 1 : (rng() % 2 + 1);
                    for (int i = 0; i < count && !normalIds.empty(); ++i) {
                        enemies.push_back(edb.createEnemy(normalIds[dist(rng)]));
                    }
                }
            }

            if (enemies.empty()) {
                enemies.push_back(edb.createEnemy("slime"));
            }

            auto &bm = gm.getBattleManager();
            battleScene->initBattle(&bm, player, enemies, &inv, gm.getCurrentFloor());
            bm.startBattle(player, enemies, &inv);
            window.switchTo(SceneManager::BATTLE);
            break;
        }
        case NodeType::SHOP:
            shopScene->initShop(player, &inv);
            window.switchTo(SceneManager::SHOP);
            break;
        case NodeType::REST:
            restScene->initRest(player);
            window.switchTo(SceneManager::REST);
            break;
        case NodeType::REWARD: {
            int gold = 20 + rng() % 30;
            rewardScene->initReward(player, &inv, gold, false);
            window.switchTo(SceneManager::REWARD);
            break;
        }
        }
    });
    QObject::connect(mapScene, &MapScene::exitRequested, &window, [&]() {
        window.switchTo(SceneManager::MAIN_MENU);
    });

    // 战斗结束 -> 奖励 / 失败
    QObject::connect(battleScene, &BattleScene::battleFinished, &window, [&](bool won) {
        if (won) {
            auto *node = gm.getMapManager().getCurrentNode();
            bool isElite = node && node->getType() == NodeType::ELITE;
            bool isBoss = node && node->getType() == NodeType::BOSS;
            int gold = isElite ? 50 : (isBoss ? 100 : 25);
            rewardScene->initReward(gm.getPlayer(), &gm.getInventory(), gold, isElite || isBoss);
            window.switchTo(SceneManager::REWARD);

            if (isBoss) {
                gm.getMapManager().setMapComplete(true);
            }
        } else {
            QMessageBox::information(&window, "游戏结束", "你已阵亡...\n返回主菜单。");
            window.switchTo(SceneManager::MAIN_MENU);
        }
    });
    QObject::connect(battleScene, &BattleScene::exitRequested, &window, [&]() {
        window.switchTo(SceneManager::MAIN_MENU);
    });

    // 奖励完成 -> 回地图
    QObject::connect(rewardScene, &RewardScene::rewardFinished, &window, [&]() {
        if (gm.getMapManager().isMapComplete()) {
            QMessageBox::information(&window, "通关", "恭喜你通关了杀戮尖塔 1.5！");
            window.switchTo(SceneManager::MAIN_MENU);
        } else {
            returnToMap();
        }
    });

    // 商店 / 休息 -> 回地图
    QObject::connect(shopScene, &ShopScene::shopFinished, &window, returnToMap);
    QObject::connect(restScene, &RestScene::restFinished, &window, returnToMap);

    window.switchTo(SceneManager::MAIN_MENU);
    window.show();

    return app.exec();
}
//todo：在有王冠和护盾时 扣血计算有点问题
//能量上限要根据遗物来计算
