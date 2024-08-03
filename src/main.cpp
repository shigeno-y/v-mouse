// DigiMouse Mouse Wiggler
// Originally wrote by Jeff White (jwhite@white.nu)
// MIT License

// to write
// 1. Start without inserting Digispark.
// 2. When you see "Please plug in the device ... " message, insert Degispark.

// setting: set "mode" 0 or 1
// see http://digistump.com/wiki/digispark/tutorials/modelbi2c

#include "parts.hpp"

using namespace shigenoy::vmouse;

void setup()
{
  pinMode(mode, OUTPUT);
  DigiMouse.begin();
}

void loop()
{
  prepare_supply(SLEEP_TIME_MS * 5);
  for (int ctx = 3; ctx > 0; --ctx)
  {
    LEDon();
    for (int i = 0; i < ctx; ++i)
    {
      mouse_move(-MOUSE_DELTA, 500 / SLEEP_TIME_MS, SLEEP_TIME_MS);
    }
    LEDoff();

    for (int i = 0; i < ctx; ++i)
    {
      mouse_move(MOUSE_DELTA, 500 / SLEEP_TIME_MS, SLEEP_TIME_MS);
    }
  }
  deploy_supply(SLEEP_TIME_MS * 5);
}