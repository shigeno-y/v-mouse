// DigiMouse Mouse Wiggler
// Originally wrote by Jeff White (jwhite@white.nu)
// MIT License

// to write
// 1. Start without inserting Digispark.
// 2. When you see "Please plug in the device ... " message, insert Degispark.

// setting: set "mode" 0 or 1
// see http://digistump.com/wiki/digispark/tutorials/modelbi2c

#include "parts.hpp"

#ifndef _MSC_FULL_VER
#    include <Arduino.h>
#endif

namespace {
constexpr unsigned int mode          = 1; // 1 or 0: LED is connected pin0 or pin1 depend on model
constexpr unsigned char MOUSE_DELTA  = 1;
constexpr unsigned int SLEEP_TIME_MS = 50; // ms
} // namespace

#ifndef abs
#    define abs(x) ((x) > 0 ? (x) : -(x))
#endif

void
setup()
{
    shigenoy::vmouse::mySetup(mode);
}

//#define USE_MAIN_BRANCH
//#define USE_SPIRAL_DIRECT
#if defined USE_MAIN_BRANCH
void
loop()
{
    int ctx{ 10 };
    {
        shigenoy::vmouse::LEDon(mode);
        shigenoy::vmouse::prepare_medic(SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(-MOUSE_DELTA, 0, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
        shigenoy::vmouse::LEDoff(mode);

        shigenoy::vmouse::deploy_medic(SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(MOUSE_DELTA, 0, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
    }
    for (ctx = 9; ctx > 1; --ctx)
    {
        shigenoy::vmouse::LEDon(mode);
        shigenoy::vmouse::mouse_move(-MOUSE_DELTA, 0, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);

        shigenoy::vmouse::LEDoff(mode);
        shigenoy::vmouse::mouse_move(MOUSE_DELTA, 0, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
    }
    {
        ctx = 1;
        shigenoy::vmouse::prepare_supply(SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(-MOUSE_DELTA, 0, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
        shigenoy::vmouse::LEDon(mode);
        shigenoy::vmouse::mouse_move(MOUSE_DELTA, 0, 500 * ctx / SLEEP_TIME_MS, SLEEP_TIME_MS);
        shigenoy::vmouse::deploy_supply(SLEEP_TIME_MS);
        shigenoy::vmouse::LEDoff(mode);
    }
}
#elif defined USE_SPIRAL_DIRECT
#    include "spiral.hpp"

void
loop()
{
    shigenoy::vmouse::Spiral s(55000, 25.);
    int dx{ 0 }, dy{ 0 };
    while (s.update(SLEEP_TIME_MS, dx, dy))
    {
        shigenoy::vmouse::mouse_move(dx, dy, 1, SLEEP_TIME_MS);
    }

    shigenoy::vmouse::LEDon(mode);
    shigenoy::vmouse::prepare_supply(SLEEP_TIME_MS);
    {
        int x{ 0 }, y{ 0 };
        s.getCurrentPos(x, y);

        shigenoy::vmouse::mouse_move((x < 0) ? 1 : -1, 0, abs(x), SLEEP_TIME_MS);
        shigenoy::vmouse::mouse_move(0, (y < 0) ? 1 : -1, abs(y), SLEEP_TIME_MS);
    }
    shigenoy::vmouse::deploy_supply(SLEEP_TIME_MS);
    shigenoy::vmouse::LEDoff(mode);
}
#else
#    include "code.hpp"

void
loop()
{
    bool prepared{ false };
    size_t tick{ 0 }, idx{ 0 };
    while (idx < 1700)
    {
#    ifdef _MSC_FULL_VER
        const char pw  = pattern_length[idx++];
        const char ctr = pattern_length[idx++];
#    else
        const char pw  = pgm_read_byte( (pattern_length+(idx++)) );
        const char ctr = pgm_read_byte( (pattern_length+(idx++)) );
#    endif
        const char dx{ pw >> 4 }, dy{ pw & 0x0F };
        shigenoy::vmouse::mouse_move(dx - 4, dy - 4, ctr, SLEEP_TIME_MS);
        tick += ctr * SLEEP_TIME_MS;
        if (!prepared && tick >= 55000)
        {
            prepared = true;
            shigenoy::vmouse::LEDon(mode);
            shigenoy::vmouse::prepare_supply(SLEEP_TIME_MS);
        }
    }

    shigenoy::vmouse::deploy_supply(SLEEP_TIME_MS);
    shigenoy::vmouse::LEDoff(mode);
}

#endif
