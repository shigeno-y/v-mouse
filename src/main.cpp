// DigiMouse Mouse Wiggler
// Originally wrote by Jeff White (jwhite@white.nu)
// MIT License

// to write
// 1. Start without inserting Digispark.
// 2. When you see "Please plug in the device ... " message, insert Degispark.

// setting: set "mode" 0 or 1
// see http://digistump.com/wiki/digispark/tutorials/modelbi2c

#include <DigiMouse.h>

constexpr unsigned int mode = 1; // 1 or 0: LED is connected pin0 or pin1 depend on model
constexpr unsigned char MOUSE_DELTA = 1;
constexpr unsigned int SLEEP_TIME_MS = 5; // ms

void setup()
{
  pinMode(mode, OUTPUT);
  DigiMouse.begin();
}

void LEDon()
{
  digitalWrite(mode, HIGH);
}

void LEDoff()
{
  digitalWrite(mode, LOW);
}

void mouse_move(const char deltaX, const int count, const int delay)
{
  for (int i = 0; i < count; ++i)
  {
    DigiMouse.moveX(deltaX);
    DigiMouse.delay(delay);
  }
}
void prepare_supply(const int delay)
{
  DigiMouse.rightClick();
  DigiMouse.delay(delay);
}

void deploy_supply(const int delay)
{

  DigiMouse.leftClick();
  DigiMouse.delay(delay);
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