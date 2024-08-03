#ifndef SHIGENO_Y_VMOUSE_PARTS_INCLUDED
#define SHIGENO_Y_VMOUSE_PARTS_INCLUDED

#include "DigiMouse.h"

namespace shigenoy::vmouse {
constexpr unsigned int mode          = 1; // 1 or 0: LED is connected pin0 or pin1 depend on model
constexpr unsigned char MOUSE_DELTA  = 1;
constexpr unsigned int SLEEP_TIME_MS = 5; // ms

void
LEDon()
{
    digitalWrite(mode, HIGH);
}

void
LEDoff()
{
    digitalWrite(mode, LOW);
}

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
    DigiMouse.rightClick();
    DigiMouse.delay(delay);
}

void
deploy_supply(const int delay)
{

    DigiMouse.leftClick();
    DigiMouse.delay(delay);
}
} // namespace shigenoy::vmouse

#endif
