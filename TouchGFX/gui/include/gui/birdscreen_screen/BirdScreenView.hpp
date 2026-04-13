#ifndef BIRDSCREENVIEW_HPP
#define BIRDSCREENVIEW_HPP

#include <gui_generated/birdscreen_screen/BirdScreenViewBase.hpp>
#include <gui/birdscreen_screen/BirdScreenPresenter.hpp>

class BirdScreenView : public BirdScreenViewBase
{
public:
    BirdScreenView();
    virtual ~BirdScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // ?? 1. 声明心跳定时器（处理重力和水管移动）
    virtual void handleTickEvent(); 
    
    // ?? 2. 声明玩家点击屏幕时的跳跃动作
    virtual void birdJump();        
		virtual void restartGame();
protected:
private:
    float birdY;        
    float birdVelocity; 
    
    int pipe1X;       
    int pipe2X;       
    
    int pipe1Y; 
    int pipe2Y;

    // ?? 1. 新增：游戏状态标志位
    bool isGameOver;

    // ?? 2. 新增：物理体积常数 (请根据你在TouchGFX里图片的真实大小微调)
    const int BIRD_X = 82;             // 小鸟固定的X坐标
    const int BIRD_WIDTH = 30;         // 小鸟图片的宽度
    const int BIRD_HEIGHT = 25;        // 小鸟图片的高度
    
    const int PIPE_WIDTH = 24;         // 水管图片的宽度
    const int PIPE_IMAGE_HEIGHT = 120; 
    const int BIRD_GAP_SIZE = 90;      // 缺口大小稍微放大一点，降低难度
    const int PIPE_MOVE_SPEED = 3;

    int score;        // 存储当前分数
    int tickCounter;  // 存储帧数（用来计算秒）
};

#endif // BIRDSCREENVIEW_HPP