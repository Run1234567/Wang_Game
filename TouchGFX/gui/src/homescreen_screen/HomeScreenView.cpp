#include <gui/homescreen_screen/HomeScreenView.hpp>
#include <stdlib.h> 

HomeScreenView::HomeScreenView()
{
    score = 0; 
    tickCounter = 0; // ?? 开机时，计时器清零
}

void HomeScreenView::setupScreen()
{
    HomeScreenViewBase::setupScreen();
}

void HomeScreenView::tearDownScreen()
{
    HomeScreenViewBase::tearDownScreen();
}

void HomeScreenView::handleTickEvent()
{
    tickCounter++; 

    if (tickCounter >= 30) // 大约 1 秒钟
    {
        tickCounter = 0; 

    int holesX[4] = {41, 184, 173, 91};
    int holesY[4] = {108, 108, 74, 68};

        // 2. 随机抽取一个洞的编号 (生成 0, 1, 2, 3 中的一个)
        int randomHole = rand() % 4; 

        // 3. 把选中的坐标拿出来
        int newX = holesX[randomHole];
        int newY = holesY[randomHole];

        // 4. 让地鼠移动到指定的洞
        button1.invalidate();          
        button1.setXY(newX, newY);     
        button1.invalidate();  
    }
}

// ?? 手指点击地鼠的动作
void HomeScreenView::whackMole()
{
    score++; 
    textProgress1.setValue(score); 

    tickCounter = 0; // 打中后重新倒计时

    // 1. 同样设定好 4 个洞的坐标
    int holesX[4] = {41, 184, 173, 91};
    int holesY[4] = {108, 108, 74, 68};

    // 2. 随机抽取一个洞的编号
    int randomHole = rand() % 4; 

    // 3. 把选中的坐标拿出来
    int newX = holesX[randomHole];
    int newY = holesY[randomHole];

    // 4. 让地鼠瞬间移动过去
    button1.invalidate();          
    button1.setXY(newX, newY);     
    button1.invalidate();          
}