#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

Screen2Presenter::Screen2Presenter(Screen2View& v)
    : view(v)
{

}

void Screen2Presenter::activate()
{

}

void Screen2Presenter::deactivate()
{

}

void Screen2Presenter::UpdateImageX(int16_t x)
{
	model->updateX(x);
}

int16_t Screen2Presenter::GetImageX()
{
	return model->GetImageX();
}

void Screen2Presenter::UpdateImageY(int16_t y)
{
	model->updateY(y);
}

int16_t Screen2Presenter::GetImageY()
{
	return model->GetImageY();
}
