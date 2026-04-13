#ifndef DINOSCREENPRESENTER_HPP
#define DINOSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DinoScreenView;

class DinoScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DinoScreenPresenter(DinoScreenView& v);

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

    virtual ~DinoScreenPresenter() {};

private:
    DinoScreenPresenter();

    DinoScreenView& view;
};

#endif // DINOSCREENPRESENTER_HPP
