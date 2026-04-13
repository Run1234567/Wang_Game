#ifndef BREAKOUTSCREENVIEW_HPP
#define BREAKOUTSCREENVIEW_HPP

#include <gui_generated/breakoutscreen_screen/BreakoutScreenViewBase.hpp>
#include <gui/breakoutscreen_screen/BreakoutScreenPresenter.hpp>
#include <stdlib.h> // 必须引入，用于生成随机数

class BreakoutScreenView : public BreakoutScreenViewBase
{
public:
    BreakoutScreenView();
    virtual ~BreakoutScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void handleDragEvent(const touchgfx::DragEvent& event);

protected:
    // --- 物理引擎 ---
    int ballX, ballY, ballSpeedX, ballSpeedY, paddleX;

    // --- 砖块管理 ---
    static const int NUM_BRICKS = 15;      // 增加到 15 个
    bool isBrickAlive[NUM_BRICKS];         // 存活状态
    touchgfx::Box* bricks[NUM_BRICKS];     // 砖块指针数组

    // --- 动态下落控制 ---
    int fallCounter;                       // 下落计时器，用来控制下落速度

    // ====== 新增状态变量 ======
    bool isGameOver;      // 游戏是否结束
    int brokenBricks;     // 已击碎的砖块数

    // ====== 新增函数声明 ======
    virtual void restartGame(); // 对应 Designer 里的函数名
    void resetGameLogic();      // 封装重置逻辑
};

#endif