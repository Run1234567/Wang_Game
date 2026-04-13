#ifndef DINOSCREENVIEW_HPP
#define DINOSCREENVIEW_HPP

#include <gui_generated/dinoscreen_screen/DinoScreenViewBase.hpp>
#include <gui/dinoscreen_screen/DinoScreenPresenter.hpp>

class DinoScreenView : public DinoScreenViewBase
{
public:
    DinoScreenView();
    virtual ~DinoScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // ====== 定时器与点击事件声明 ======
    virtual void handleTickEvent();
    virtual void handleClickEvent(const touchgfx::ClickEvent& event); 
    virtual void restartGame();

protected:
    // ====== 游戏状态与物理参数 ======
    // --- 恐龙物理引擎 ---
    int dinoY;            // 恐龙当前的 Y 坐标
    float velocityY;      // 垂直速度 
    float gravity;        // 重力加速度
    bool isJumping;       // 是否正在空中
    int groundY;          // 地面的固定 Y 坐标

    // --- 树的移动管理 ---
    int treeX;            // 树当前的 X 坐标
    int treeSpeed;        // 树的移动速度
    
    // --- 游戏状态 ---
    bool isGameOver;      // 游戏结束标志
};

#endif // DINOSCREENVIEW_HPP