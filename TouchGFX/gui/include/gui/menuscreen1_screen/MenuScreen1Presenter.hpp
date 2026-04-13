#ifndef MENUSCREEN1PRESENTER_HPP
#define MENUSCREEN1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MenuScreen1View;

class MenuScreen1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    MenuScreen1Presenter(MenuScreen1View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MenuScreen1Presenter() {};

private:
    MenuScreen1Presenter();

    MenuScreen1View& view;
};

#endif // MENUSCREEN1PRESENTER_HPP
