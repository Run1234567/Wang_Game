#include <gui/birdscreen_screen/BirdScreenView.hpp>
#include <stdlib.h> 
#include <touchgfx/Utils.hpp> // 必须引用这个头文件，才能使用 Unicode::snprintf
BirdScreenView::BirdScreenView()
{
    // 初始化数值
    birdY = 100;       
    birdVelocity = 0;  
    pipe1X = 320;             
    pipe2X = 320 + 180;       
    pipe1Y = (rand() % 100) + 40; 
    pipe2Y = (rand() % 100) + 40; 
    score = 0;
    tickCounter = 0;
    isGameOver = false;
    // ?? 开机时游戏是活着的
    isGameOver = false; 
}

void BirdScreenView::setupScreen() { BirdScreenViewBase::setupScreen(); }
void BirdScreenView::tearDownScreen() { BirdScreenViewBase::tearDownScreen(); }

void BirdScreenView::handleTickEvent()
{
    // ?? 如果游戏已经结束，直接退出这个函数，画面就会完全静止！
    if (isGameOver) 
    {
        return; 
    }
    // 每一帧计数器 +1
    tickCounter++;

    // 如果到了 60 帧 (大约 1 秒)
    if (tickCounter >= 60)
    {
        score++;          // 分数自增
        tickCounter = 0;  // 计数器归零，准备数下一秒

        // 【核心代码】将数字转化成屏幕能显示的文字
        Unicode::snprintf(textScoreBuffer, TEXTSCORE_SIZE, "%d", score);
        
        // 告诉屏幕：这一块区域脏了，请重新画一下分数
        textScore.invalidate();
    }

    // --- A. 重力系统 ---
    birdVelocity += 0.5;     
    birdY += birdVelocity; 

    if(birdY > 210 || birdY < -10) 
    { 
        isGameOver = true; 
    }

    bird.invalidate();       
    bird.setXY(BIRD_X, (int)birdY);   
    bird.invalidate();      

    // --- B. 水管移动系统 ---
    pipe1X -= PIPE_MOVE_SPEED; 
    pipe2X -= PIPE_MOVE_SPEED; 

    if(pipe1X < -80) // 给点余量，等水管彻底离开屏幕再刷新
    {
        pipe1X = pipe2X + 180; 
        pipe1Y = (rand() % 100) + 40; 
    }
    if(pipe2X < -80) 
    {
        pipe2X = pipe1X + 180; 
        pipe2Y = (rand() % 100) + 40; 
    }

    // --- C. 刷新水管位置 ---
    pipe1.invalidate();
    pipe1.setXY(pipe1X, pipe1Y + BIRD_GAP_SIZE); 
    pipe1.invalidate();

    pipe1Top.invalidate();
    pipe1Top.setXY(pipe1X, pipe1Y - PIPE_IMAGE_HEIGHT); 
    pipe1Top.invalidate();

    pipe2.invalidate();
    pipe2.setXY(pipe2X, pipe2Y + BIRD_GAP_SIZE);
    pipe2.invalidate();

    pipe2Top.invalidate();
    pipe2Top.setXY(pipe2X, pipe2Y - PIPE_IMAGE_HEIGHT);
    pipe2Top.invalidate();

// 我们定义一个标志，用来判断这一帧是否发生了碰撞
    bool currentFrameCollision = false;

    // 检查触底或飞出天际
    if(birdY > 210 || birdY < -10) 
    { 
        currentFrameCollision = true;
    }

    // 检查第 1 对水管
    if (!currentFrameCollision && BIRD_X + BIRD_WIDTH > pipe1X && BIRD_X < pipe1X + PIPE_WIDTH) 
    {
        if (birdY < pipe1Y || birdY + BIRD_HEIGHT > pipe1Y + BIRD_GAP_SIZE) 
        {
            currentFrameCollision = true;
        }
    }

    // 检查第 2 对水管 (逻辑一样)
    if (!currentFrameCollision && BIRD_X + BIRD_WIDTH > pipe2X && BIRD_X < pipe2X + PIPE_WIDTH) 
    {
        if (birdY < pipe2Y || birdY + BIRD_HEIGHT > pipe2Y + BIRD_GAP_SIZE) 
        {
            currentFrameCollision = true;
        }
    }

    // 处理碰撞
    if (currentFrameCollision)
    {
        isGameOver = true; // 标志位设为真
        
        // ?????? 新增：让文字和按钮现身！ ??????
        textGameOver.setVisible(true); // 设为可见
        textGameOver.invalidate();    // 强制刷新这块区域，不然看不见

        buttonRestart.setVisible(true); // 按钮设为可见
        buttonRestart.invalidate();
    }
}

void BirdScreenView::birdJump()
{
    // ?????? 修改：如果游戏结束，点击全屏没有任何反应，必须点那个“重试按钮” ??????
    if (isGameOver) 
    {
        return; 
    }

    // 如果活着，正常跳跃 (这里假设你已经采用了小数重力方案)
    birdVelocity = -4.0f; 
}
// ?????? 新增：专门处理重试按钮点击的函数 ??????
void BirdScreenView::restartGame()
{
    // 如果还没死，点它干嘛，直接返回
    if (!isGameOver) return; 

    // 1. 重置游戏物理数值 (假设使用的是小数重力方案)
    birdY = 100.0f;
    birdVelocity = 0.0f;
    pipe1X = 320;
    pipe2X = 320 + 180;
    pipe1Y = (rand() % 100) + 40;
    pipe2Y = (rand() % 100) + 40;
		// 重置分数
    score = 0;
    tickCounter = 0;

    // 更新屏幕上的数字为 0
    Unicode::snprintf(textScoreBuffer, TEXTSCORE_SIZE, "%d", score);
    textScore.invalidate();

    isGameOver = false;

    // 2. ?????? 极其重要：把文字和按钮再次隐身！ ??????
    textGameOver.setVisible(false); // 设为不可见
    textGameOver.invalidate();

    buttonRestart.setVisible(false); // 按钮设为不可见
    buttonRestart.invalidate();
		
    // 3. 满血复活，重新开局
    isGameOver = false; 
}