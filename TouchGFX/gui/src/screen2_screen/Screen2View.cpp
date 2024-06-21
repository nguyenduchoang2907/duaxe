#include <gui/screen2_screen/Screen2View.hpp>
#include "cmsis_os.h"

extern osMessageQueueId_t Queue1Handle;
extern uint16_t JoystickX, JoystickY;
extern uint16_t scores;
Screen2View::Screen2View() : tickCount(0), flickerCount(0), flickering(false), lives(3)
{
}

void Screen2View::setupScreen()
{
    localImageX = presenter->GetImageX();
    localImageY = presenter->GetImageY();
    Screen2ViewBase::setupScreen();
    car.setX(localImageX);
    car.setY(localImageY);
    lamb.setX(14);
    lamb.setY(0);
    updatePoint(scores);
    updateHearts();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
    presenter->UpdateImageX(localImageX);
}

void Screen2View::handleTickEvent()
{
    Screen2ViewBase::handleTickEvent();

    tickCount++;
    switch (tickCount % 5)
    {
        case 0:
            track0.setVisible(true);
            track4.setVisible(false);
            break;
        case 1:
            track1.setVisible(true);
            track0.setVisible(false);
            break;
        case 2:
            track2.setVisible(true);
            track1.setVisible(false);
            break;
        case 3:
            track3.setVisible(true);
            track2.setVisible(false);
            break;
        case 4:
            track4.setVisible(true);
            track3.setVisible(false);
            break;
        default:
            break;
    }

    lamb.setY(tickCount * 2 % 320);
    lamb.setX((tickCount * 2 / 320 % 4) * 60 + 15);

    if (JoystickX > 150 && localImageX > -6)
    {
        localImageX -= 3;
        car.setX(localImageX);
    }
    if (JoystickX < 90 && localImageX < 197)
    {
        localImageX += 3;
        car.setX(localImageX);
    }
    if (JoystickY > 150 && localImageY < 234)
    {
        localImageY += 3;
        car.setY(localImageY);
    }
    if (JoystickY < 90 && localImageY > 0)
    {
        localImageY -= 3;
        car.setY(localImageY);
    }

    if (!flickering && checkCollision(car, lamb))
    {
        lives--;
        updateHearts();
        flickering = true;
        flickerCount = 0;
    }

    if (flickering)
    {
        flickerCount++;
        if (flickerCount % 2 == 0)
        {
            car.setVisible(!car.isVisible());
        }

        if (flickerCount >= 60)
        {
            flickering = false;
            car.setVisible(true);
        }
    }
    updatePoint(tickCount/20);
    invalidate();
}

void Screen2View::updatePoint(int newValue)
{
    Unicode::snprintf(txtBuffer, POINT_SIZE, "%d", newValue);
    point.setWildcard(txtBuffer);
    point.invalidate();
}

void Screen2View::updateHearts()
{
    switch (lives)
    {
        case 0:
        	resetGame();
            application().gotoScreen1ScreenNoTransition();
            break;
        case 1:
            heart1.setVisible(true);
            heart2.setVisible(false);
            heart3.setVisible(false);
            break;
        case 2:
            heart1.setVisible(true);
            heart2.setVisible(true);
            heart3.setVisible(false);
            break;
        case 3:
            heart1.setVisible(true);
            heart2.setVisible(true);
            heart3.setVisible(true);
            break;
        default:
            break;
    }
    heart1.invalidate();
    heart2.invalidate();
    heart3.invalidate();
}

void Screen2View::resetGame()
{   scores = tickCount/20;
    tickCount = 0;
    flickerCount = 0;
    flickering = false;
    lives = 3;
    localImageX = 0;
    localImageY = 0;
    car.setX(localImageX);
    car.setY(localImageY);
    lamb.setX(14);
    lamb.setY(0);
    updatePoint(0);
    updateHearts();
}

bool Screen2View::checkCollision(const Image& img1, const Image& img2)
{
    return (img1.getX() < img2.getX() + img2.getWidth() &&
            img1.getX() + img1.getWidth() > img2.getX() &&
            img1.getY() < img2.getY() + img2.getHeight() &&
            img1.getY() + img1.getHeight() > img2.getY());
}
