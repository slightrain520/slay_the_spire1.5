# Slay the Spire 1.5

一款基于 C++ / Qt6 开发的卡牌 Roguelike 游戏，灵感来源于《杀戮尖塔》。玩家选择角色，沿随机地图挑战关卡，在回合制战斗中使用卡牌消灭敌人，收集遗物、药水与新卡牌不断成长。

---

## 功能特性

- **双角色可选**：战士（易伤/虚弱方向）、女王（中毒/灾厄方向），各有专属起始牌组
- **回合制战斗**：能量管理、手牌操作、拖拽出牌，支持点击/拖拽两种出牌方式
- **丰富卡牌关键字**：消耗、重放、固有、保留
- **完整 Buff 体系**：易伤、虚弱、中毒、灾厄、力量
- **遗物系统**：王冠、佛珠、玉米、能量棒、扇子等多种遗物，各自提供战斗加成
- **随机地图**：多条可选路径，包含普通战、精英战、休息处、商店、奖励节点
- **药水槽**：最多携带 3 瓶药水，支持靶向使用
- **存档系统**：地图进度持久化保存

---

## 技术栈

| 项目 | 版本 |
|------|------|
| 语言 | C++17 |
| UI 框架 | Qt 6（Widgets） |
| 构建系统 | CMake ≥ 3.16 |

---

## 目录结构

```
slay_1.5/
├── CMakeLists.txt
├── resources/
│   ├── resources.qrc
│   └── images/            # 图片资源（可后续补充）
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── battle/        # 战斗管理器、伤害计算器
│   │   ├── buff/          # Buff 与 BuffManager
│   │   ├── card/          # Card / AttackCard / SkillCard / Deck / CardEffect
│   │   ├── character/     # Player / Enemy / Character 基类
│   │   ├── item/          # Relic / Potion / Inventory
│   │   └── map/           # 地图节点与地图生成
│   ├── data/              # 卡牌/敌人/药水/遗物数据库，GameConfig
│   └── ui/
│       ├── SceneManager   # 场景切换管理
│       ├── scenes/        # 各场景（主菜单、选角、战斗、地图、商店…）
│       └── widgets/       # CardWidget 等复用组件
└── prd.md                 # 产品需求文档
```

---

## 构建与运行

### 依赖

- Qt 6.x（Widgets 模块）
- CMake ≥ 3.16
- 支持 C++17 的编译器（GCC 11+ / Clang 13+ / MSVC 2022+）

### 编译步骤

```bash
# 1. 克隆仓库
git clone https://github.com/<your-username>/slay_1.5.git
cd slay_1.5

# 2. 配置（将 Qt6 路径替换为本机实际路径）
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6

# 3. 编译
cmake --build build --parallel

# 4. 运行
./build/SlayTheSpire1.5        # macOS / Linux
# 或
build\SlayTheSpire1.5.exe      # Windows
```

> **macOS 提示**：如使用 Homebrew 安装 Qt6，`CMAKE_PREFIX_PATH` 通常为  
> `/opt/homebrew/opt/qt@6`（Apple Silicon）或 `/usr/local/opt/qt@6`（Intel）。

---

## 贡献指南

欢迎提交 Issue 与 Pull Request！

1. Fork 本仓库
2. 基于 `main` 创建特性分支：`git checkout -b feature/your-feature`
3. 提交改动：`git commit -m "feat: 描述你的修改"`
4. 推送到远端：`git push origin feature/your-feature`
5. 在 GitHub 创建 Pull Request，等待 Review 通过后合并

> 直接向 `main` 分支推送已被禁用，所有修改须经 PR 审核。

---

## 许可证

本项目仅供学习与交流使用。
