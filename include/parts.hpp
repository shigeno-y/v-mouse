#ifndef SHIGENO_Y_VMOUSE_PARTS_INCLUDED
#define SHIGENO_Y_VMOUSE_PARTS_INCLUDED

#include "DigiMouse.h"

namespace shigenoy::vmouse {
size_t elapsed{ 0 };

void
LEDon(const unsigned int mode)
{
    digitalWrite(mode, HIGH);
}

void
LEDoff(const unsigned int mode)
{
    digitalWrite(mode, LOW);
}

/*
    55 * 1000 [ms]
    25 * 121 * 1000 * 1000 [tick]
*/

void
LEDwave(const unsigned int mode, int tick)
{
    tick /= 550;
    const float v = sin(static_cast<double>(tick * tick) / 50.);
    if (signbit(v))
    {
        LEDoff(mode);
    }
    else
    {
        LEDon(mode);
    }
}

void
mouse_move(const char deltaX, const int count, const int delay)
{
    for (int i = 0; i < count; ++i)
    {
        DigiMouse.moveX(deltaX);
        DigiMouse.delay(delay);
        elapsed += delay;
        LEDwave(1, elapsed);
    }
}

void
prepare_supply(const int delay)
{
    // DigiMouse.rightClick();
    // DigiMouse.delay(delay);
    // /*
    {
        // RIGHT Click
        DigiMouse.setButtons(2);
        mouse_move(-1, 1, delay * 10);
        // Unclick
        DigiMouse.setButtons(0);
        mouse_move(1, 1, delay * 10);
    }
    // */
}

void
deploy_supply(const int delay)
{

    // DigiMouse.leftClick();
    // DigiMouse.delay(delay);
    // /*
    {
        // Click
        DigiMouse.setButtons(1);
        mouse_move(-1, 1, delay * 20);
        // Unclick
        DigiMouse.setButtons(0);
        mouse_move(1, 1, delay * 20);
    }
    // */
    elapsed=0;
}

void
prepare_medic(const int delay)
{
    DigiMouse.scroll(-10);
    mouse_move(-1, 1, delay);
}

void
deploy_medic(const int delay)
{
    // Click
    DigiMouse.setButtons(1);
    mouse_move(-1, 1, delay * 20);
    // Unclick
    DigiMouse.setButtons(0);
    mouse_move(1, 1, delay * 20);

    DigiMouse.scroll(10);
    mouse_move(1, 1, delay);
}

} // namespace shigenoy::vmouse

#endif
