// DigiMouse Mouse Wiggler
// Originally wrote by Jeff White (jwhite@white.nu)
// MIT License

// to write
// 1. Start without inserting Digispark.
// 2. When you see "Please plug in the device ... " message, insert Degispark.

// setting: set "mode" 0 or 1
// see http://digistump.com/wiki/digispark/tutorials/modelbi2c

#include "parts.hpp"

#include <Arduino.h>

#ifdef _MSC_FULL_VER
#    include "temporary_directory.hpp"
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
#ifdef USE_MAIN_BRANCH
void
loop()
{
    using namespace shigenoy::vmouse;
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
#else
#    include "spiral.hpp"

void
loop()
{
#    ifdef _MSC_FULL_VER
    usdRemoteViewer::messaging::tempfile::TemporaryDirectory tmp{ {}, {}, {}, false };
    usdRemoteViewer::messaging::tempfile::logfile = std::ofstream{ tmp.tmpfile() };
#    endif

    shigenoy::vmouse::Spiral s(55000, 50.);
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
#endif
