#ifndef MENUSCREEN1VIEW_HPP
#define MENUSCREEN1VIEW_HPP

#include <gui_generated/menuscreen1_screen/MenuScreen1ViewBase.hpp>
#include <gui/menuscreen1_screen/MenuScreen1Presenter.hpp>

class MenuScreen1View : public MenuScreen1ViewBase
{
public:
    MenuScreen1View();
    virtual ~MenuScreen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MENUSCREEN1VIEW_HPP
