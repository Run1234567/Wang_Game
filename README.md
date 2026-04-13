# 🎮 Wang\_Game (STM32U575 Edition)

**Wang\_Game** 是一个专为**华清远见 STM32U575 开发板**量身打造的嵌入式 GUI 游戏合集。本项目基于 TouchGFX 框架开发，内置了四款经典的触摸屏小游戏，并且深度融入了专属的“张旺”主题元素。不管是用来验证开发板屏幕性能，还是单纯想看看“旺”的各种搞怪形态，这个项目都能完美胜任！

## 🕹️ 游戏阵容 (Game Roster)

你可以通过主菜单无缝切换体验以下四款独立开发的小游戏：


| **游戏名称**                   | **核心玩法**                             | **特色机制与物理逻辑**                                                    |
| ------------------------------ | ---------------------------------------- | ------------------------------------------------------------------------- |
| **🐹 打地鼠 (HomeScreen)**     | 经典打地鼠，只不过地鼠换成了**旺的照片** | 约 1 秒 (30 Ticks) 刷新周期，随机坐标瞬移，点击重置倒计时                 |
| **🦅 像素鸟 (BirdScreen)**     | 点击屏幕，让**旺的照片**在天空中飞跃水管 | 纯 C++ 模拟重力加速度系统，AABB 碰撞检测，无缝循环障碍物                  |
| **🧱 打砖块 (BreakoutScreen)** | 左右拖动底部挡板，接住弹球击碎所有砖块   | 15 个独立物理碰撞体，采用局部刷新技术 (`invalidateRect`) 彻底消灭拖尾残影 |
| **🦖 旺快跑 (DinoScreen)**     | 全屏任意触摸起跳，操控旺躲避迎面的树木   | 精心调优的 Hitbox（碰撞体积）瘦身容错，告别“空气墙”导致的非正常死亡     |

## 🎬 效果演示 (Demo Video)
[![Bilibili](https://img.shields.io/badge/Bilibili-观看完整演示视频-fb7299?style=for-the-badge&logo=bilibili&logoColor=white)](https://www.bilibili.com/video/BV1U3Q4BnEsa/?vd_source=4deebaab9cabc4e6662e4131d191e9a1)

## 🛠️ 硬件与技术栈 (Tech Specs)

本项目充分利用了 STM32U5 系列的超低功耗与高性能特性，并在软件层面做了大量优化：

* **目标平台**: 华清远见 STM32U575 开发板
* **GUI 框架**: STMicroelectronics TouchGFX
* **开发语言**: C++
* **架构设计**: 严格遵循 TouchGFX 的 MVP (Model-View-Presenter) 模式，逻辑与视图深度解耦。
* **物理引擎**: 完全依赖 `handleTickEvent` 实现帧级别的状态机流转与物理计算。
* **渲染优化**: 大量运用局部重绘 (`invalidate`) 与控件状态动态管理 (`setVisible`)，有效降低 MCU 刷新负载，防止死机。

## 🚀 烧录与运行指南 (How to Play)

1. 使用 TouchGFX Designer 打开项目目录下的 `.touchgfx` 工程文件，可以在 PC 端使用模拟器预览 UI。
2. 在 TouchGFX Designer 中点击 **Generate Code** 更新底层工程代码。
3. 启动 STM32CubeIDE 或 Keil MDK，打开生成的工程文件。
4. 将华清远见 STM32U575 开发板通过 ST-Link 或 DAP 仿真器连接至电脑。
5. 编译整个工程并烧录至开发板中。
6. 复位开发板，即可在屏幕上开始你的“虐旺”之旅！
