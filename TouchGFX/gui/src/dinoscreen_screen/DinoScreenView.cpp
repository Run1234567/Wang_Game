#include <gui/dinoscreen_screen/DinoScreenView.hpp>

DinoScreenView::DinoScreenView()
{
    // ====== 1. 初始化物理与坐标参数 ======
    groundY = 97;              
    dinoY = groundY;           
    velocityY = 0.0f;          
    gravity = 0.6f;            // 重力
    isJumping = false;

    // 树从屏幕最右侧准备进场
    treeX = 320;               
    treeSpeed = 6;             // 树向左移动的速度

    isGameOver = false;
}

void DinoScreenView::setupScreen()
{
    DinoScreenViewBase::setupScreen();
}

void DinoScreenView::tearDownScreen()
{
    DinoScreenViewBase::tearDownScreen();
}

// ====== 2. 屏幕点击与重启逻辑 (全屏点击复活版) ======
void DinoScreenView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    // 只要侦测到屏幕被按下
    if (event.getType() == touchgfx::ClickEvent::PRESSED)
    {
        if (isGameOver)
        {
            // 如果处于卡死（游戏结束）状态，点击屏幕任意位置直接重启！
            restartGame(); 
        }
        else if (!isJumping)
        {
            // 如果游戏正常运行且没在空中，正常起跳
            isJumping = true;
            velocityY = -12.0f; 
        }
    }
}

// ====== 3. 核心动画与碰撞逻辑 ======
void DinoScreenView::handleTickEvent()
{
    // 游戏结束直接冻结画面，等待玩家点击屏幕
    if (isGameOver) return; 

    // --- A. 恐龙跳跃物理模拟 ---
    if (isJumping)
    {
        velocityY += gravity;         // 速度受重力影响递增 (负数变正数)
        dinoY += (int)velocityY;      // 更新坐标

        if (dinoY >= groundY)         // 落地检测
        {
            dinoY = groundY;          // 强行对齐地面
            velocityY = 0;            // 速度归零
            isJumping = false;        // 恢复地面状态
        }
        
        // 刷新恐龙的垂直位置
        imageDino.moveTo(imageDino.getX(), dinoY);
    }

    // --- B. 树的迎面移动 ---
    treeX -= treeSpeed;
    
    // 如果树完全移出了屏幕左侧（树宽 50），就让它回到右侧循环利用
    if (treeX < -50)
    {
        treeX = 320; 
    }
    imageTree.moveTo(treeX, imageTree.getY());

    // --- C. 碰撞检测 ---
    touchgfx::Rect rectDino = imageDino.getRect();
    touchgfx::Rect rectTree = imageTree.getRect();

    // 【Hitbox 瘦身容错】防止“空气墙”撞击
    rectDino.width -= 12;
    rectDino.height -= 10;
    rectDino.x += 6;
    rectDino.y += 5;

    // 如果撞上了
    if (rectDino.intersect(rectTree))
    {
        // 仅仅标记游戏结束。没有任何 UI 控件干预，彻底断绝死机可能！
        isGameOver = true;
    }
}

// ====== 4. 重置游戏逻辑 ======
void DinoScreenView::restartGame()
{
    // 1. 重置恐龙状态
    dinoY = groundY;
    velocityY = 0.0f;
    isJumping = false;
    imageDino.moveTo(imageDino.getX(), groundY);

    // 2. 重置树的状态
    treeX = 320;
    imageTree.moveTo(treeX, imageTree.getY());

    // 3. 强行刷新一下恐龙和树的区域，清理上一局的残影
    imageDino.invalidate();
    imageTree.invalidate();

    // 4. 解除画面冻结，重新开启游戏
    isGameOver = false;
}