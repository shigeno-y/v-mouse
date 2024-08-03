#ifndef SHIGENO_Y_VMOUSE_PARTS_INCLUDED
#define SHIGENO_Y_VMOUSE_PARTS_INCLUDED

#include "DigiMouse.h"

namespace shigenoy::vmouse {
void
mouse_move(const char deltaX, const int count, const int delay)
{
    for (int i = 0; i < count; ++i)
    {
        DigiMouse.moveX(deltaX);
        DigiMouse.delay(delay);
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
        mouse_move(-1, 1, delay*10);
        // Unclick
        DigiMouse.setButtons(0);
        mouse_move(1, 1, delay*10);
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
        mouse_move(-1, 1, delay*20);
        // Unclick
        DigiMouse.setButtons(0);
        mouse_move(1, 1, delay*20);
    }
    // */
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
    mouse_move(-1, 1, delay*20);
    // Unclick
    DigiMouse.setButtons(0);
    mouse_move(1, 1, delay*20);

    DigiMouse.scroll(10);
    mouse_move(1, 1, delay);
}

} // namespace shigenoy::vmouse

#endif
