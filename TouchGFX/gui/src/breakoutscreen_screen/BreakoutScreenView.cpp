#include <gui/breakoutscreen_screen/BreakoutScreenView.hpp>

BreakoutScreenView::BreakoutScreenView()
{
    // --- 1. 初始化球与挡板 ---
    ballX = 140; 
    ballY = 150; 
    ballSpeedX = 3;  
    ballSpeedY = 3; 
    paddleX = 120; 

    // --- 2. 绑定 15 个砖块指针 ---
    bricks[0]=&brick0; bricks[1]=&brick1; bricks[2]=&brick2; bricks[3]=&brick3; bricks[4]=&brick4;
    bricks[5]=&brick5; bricks[6]=&brick6; bricks[7]=&brick7; bricks[8]=&brick8; bricks[9]=&brick9;
    bricks[10]=&brick10; bricks[11]=&brick11; bricks[12]=&brick12; bricks[13]=&brick13; bricks[14]=&brick14;

    // --- 3. 静态网格排布 (去掉随机，确保整齐) ---
    for (int i = 0; i < NUM_BRICKS; i++)
    {
        isBrickAlive[i] = true;
        
        int row = i / 5; // 第 0, 1, 2 行
        int col = i % 5; // 第 0, 1, 2, 3, 4 列

        // 计算固定位置 (每列60像素宽)
        int baseX = col * 60 + 15;  
        int baseY = row * 25 + 30;

        // 【核心】强行统一尺寸，防止 Designer 里的设置干扰碰撞
        bricks[i]->setWidth(40);  
        bricks[i]->setHeight(15);
        bricks[i]->setXY(baseX, baseY);
        
        bricks[i]->setVisible(true);
        bricks[i]->invalidate(); // 初始刷一下
			isGameOver = false;
    brokenBricks = 0;
    }
}

void BreakoutScreenView::setupScreen() { BreakoutScreenViewBase::setupScreen(); }
void BreakoutScreenView::tearDownScreen() { BreakoutScreenViewBase::tearDownScreen(); }

void BreakoutScreenView::handleTickEvent()
{
    // ====== 0. 游戏状态检查 ======
    // 如果游戏已经结束（赢了或输了），直接冻结画面，不再执行任何物理计算！
    if (isGameOver)
    {
        return; 
    }

    // 动态获取球的尺寸
    int ballWidth = ball.getWidth();
    int ballHeight = ball.getHeight();

    // --- A. 更新球的坐标 ---
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // --- B. 碰墙检测 (防卡墙版，适配 320 宽屏) ---
    if (ballX <= 0) 
    {
        ballX = 0;
        ballSpeedX = -ballSpeedX; 
    }
    else if (ballX >= (320 - ballWidth)) 
    {
        ballX = 320 - ballWidth;
        ballSpeedX = -ballSpeedX; 
    }
    
    if (ballY <= 0)
    {
        ballY = 0;
        ballSpeedY = -ballSpeedY; 
    }

    // --- C. 碰挡板检测 ---
    int currentPaddleY = paddle.getY();
    int paddleWidth = paddle.getWidth();
    int paddleHeight = paddle.getHeight();

    if ((ballY + ballHeight) >= currentPaddleY && ballY <= (currentPaddleY + paddleHeight))
    {
        if ((ballX + ballWidth) >= paddleX && ballX <= (paddleX + paddleWidth)) 
        {
            if (ballSpeedY > 0) // 确保球是往下掉才反弹，防止球在挡板里疯狂鬼畜
            {
                ballSpeedY = -ballSpeedY;        
                ballY = currentPaddleY - ballHeight; // 强制把球拉回挡板上方
            }
        }
    }

    // --- D. 碰砖块检测 (强制去残影 + 胜负判定) ---
    for (int i = 0; i < NUM_BRICKS; i++)
    {
        if (isBrickAlive[i]) 
        {
            int bX = bricks[i]->getX();
            int bY = bricks[i]->getY();
            // 直接使用我们之前强行统一的尺寸 40x15
            int bW = 40; 
            int bH = 15;

            // 标准 AABB 判断重叠
            if ((ballX + ballWidth) >= bX && ballX <= (bX + bW) &&
                (ballY + ballHeight) >= bY && ballY <= (bY + bH))
            {
                ballSpeedY = -ballSpeedY; // 球反弹
                
                // ====== 消灭残影的最强魔法 ======
                touchgfx::Rect dirtyRect = bricks[i]->getRect();
                // 稍微扩大刷新区域，彻底抹除可能存在的1像素残影边框
                dirtyRect.y -= 3;
                dirtyRect.height += 6;
                this->invalidateRect(dirtyRect); 
                
                // 彻底宣告砖块死亡
                bricks[i]->setVisible(false); 
                isBrickAlive[i] = false;      

                // ====== 胜利判定逻辑 ======
                brokenBricks++; // 击碎数量 +1
                if (brokenBricks >= NUM_BRICKS) // 全打碎了
                {
                    isGameOver = true; 
                    
                    // 显示文字 (你可以提前在 TouchGFX Designer 里把文字设为 "YOU WIN!")
                    textGameOver.setVisible(true);
                    textGameOver.invalidate();
                    
                    // 显示重新开始按钮
                    buttonRestart.setVisible(true);
                    buttonRestart.invalidate();
                }
                
                break; // 每次只碎一块，防止一球穿透多块
            }
        }
    }

    // --- E. 漏球判定 (掉出屏幕底部 = 游戏失败) ---
    if (ballY > 240) // 如果你的屏幕高度是 320，把这里的 240 改成 320
    {
        isGameOver = true; // 触发失败状态
        
        // 显示 GAME OVER 文字
        textGameOver.setVisible(true);
        textGameOver.invalidate();
        
        // 显示重新开始按钮
        buttonRestart.setVisible(true);
        buttonRestart.invalidate();
    }

    // --- F. 刷新球的位置 ---
    ball.moveTo(ballX, ballY);
}
void BreakoutScreenView::handleDragEvent(const touchgfx::DragEvent& event)
{
    int16_t x = event.getNewX();
    paddleX = x - (paddle.getWidth() / 2);

    if (paddleX < 0) paddleX = 0;
    if (paddleX > (320 - paddle.getWidth())) paddleX = (320 - paddle.getWidth()); 

    paddle.moveTo(paddleX, paddle.getY());
}


void BreakoutScreenView::restartGame()
{
    // 1. 重置球和挡板位置
    ballX = 140; 
    ballY = 150; 
    ballSpeedX = 3;  
    ballSpeedY = 3; 
    paddleX = 120;
    ball.moveTo(ballX, ballY);
    paddle.moveTo(paddleX, paddle.getY());

    // 2. 恢复所有砖块
    brokenBricks = 0;
    for (int i = 0; i < NUM_BRICKS; i++)
    {
        isBrickAlive[i] = true;
        bricks[i]->setVisible(true);
        bricks[i]->invalidate();
    }

    // 3. 隐藏 UI 元素
    textGameOver.setVisible(false);
    textGameOver.invalidate();
    buttonRestart.setVisible(false);
    buttonRestart.invalidate();

    // 4. 重开逻辑开关
    isGameOver = false;
}
