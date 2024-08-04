// DigiMouse Mouse Wiggler
// Originally wrote by Jeff White (jwhite@white.nu)
// MIT License

// to write
// 1. Start without inserting Digispark.
// 2. When you see "Please plug in the device ... " message, insert Degispark.

// setting: set "mode" 0 or 1
// see http://digistump.com/wiki/digispark/tutorials/modelbi2c

#include "parts.hpp"

namespace {
constexpr unsigned int mode          = 1; // 1 or 0: LED is connected pin0 or pin1 depend on model
constexpr unsigned char MOUSE_DELTA  = 1;
constexpr unsigned int SLEEP_TIME_MS = 5; // ms
} // namespace

void
setup()
{
    pinMode(mode, OUTPUT);
    DigiMouse.begin();
}

void
loop()
{
    int ctx{ 10 };
    {
        shigenoy::vmouse::LEDon(mode);
        shigenoy::vmouse::prepare_medic(SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(-MOUSE_DELTA, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
        shigenoy::vmouse::LEDoff(mode);

        shigenoy::vmouse::deploy_medic(SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(MOUSE_DELTA, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
    }
    for (ctx = 9; ctx > 1; --ctx)
    {
        shigenoy::vmouse::LEDon(mode);
        shigenoy::vmouse::mouse_move(-MOUSE_DELTA, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);

        shigenoy::vmouse::LEDoff(mode);
        shigenoy::vmouse::mouse_move(MOUSE_DELTA, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
    }
    {
        ctx = 1;
        shigenoy::vmouse::prepare_supply(SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(-MOUSE_DELTA, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
        shigenoy::vmouse::LEDon(mode);
        shigenoy::vmouse::mouse_move(MOUSE_DELTA, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
        shigenoy::vmouse::deploy_supply(SLEEP_TIME_MS);
        shigenoy::vmouse::LEDoff(mode);
    }
}
