#ifndef HOMESCREENVIEW_HPP
#define HOMESCREENVIEW_HPP

#include <gui_generated/homescreen_screen/HomeScreenViewBase.hpp>
#include <gui/homescreen_screen/HomeScreenPresenter.hpp>

class HomeScreenView : public HomeScreenViewBase
{
public:
    HomeScreenView();
    virtual ~HomeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // 手指点击的动作
    virtual void whackMole(); 

    // ?? 添加这一行：申请开启“系统心跳”定时器
    virtual void handleTickEvent();

protected:
private:
    int score; 
    
    // ?? 添加这一行：弄一个专门用来数数的计时器
    int tickCounter; 
};




#endif // HOMESCREENVIEW_HPP