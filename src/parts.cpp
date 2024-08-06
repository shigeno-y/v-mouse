#include "parts.hpp"

#include "DigiMouse.h"

#ifdef _MSC_FULL_VER
#    include "temporary_directory.hpp"
#    include <chrono>
#    include <iostream>
#    include <windows.h>

long long
millis()
{
    const std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

void
usbSetInterrupt(unsigned char reportBuffer[REPORT_SIZE], size_t report_size)
{
}

void
pinMode(const int mode, const int type)
{
    std::cout << "pinMode\t#" << mode << "\t" << type << std::endl;
}

void
digitalWrite(const int mode, const int val)
{
    std::cout << "DigitalWrite\t#" << mode << "\t" << val << std::endl;
}
#endif


void shigenoy::vmouse::mySetup(const unsigned int mode)
{
    pinMode(mode, OUTPUT);
    DigiMouse.begin();
}

void
shigenoy::vmouse::LEDon(const unsigned int mode)
{
    digitalWrite(mode, HIGH);
}

void
shigenoy::vmouse::LEDoff(const unsigned int mode)
{
    digitalWrite(mode, LOW);
}

void
shigenoy::vmouse::mouse_move(const char deltaX, const char deltaY, const int count, const int delay)
{
    for (int i = 0; i < count; ++i)
    {
        DigiMouse.move(deltaX, deltaY, 0);
#ifdef _MSC_FULL_VER
        /*
        {
            usdRemoteViewer::messaging::tempfile::logfile << static_cast<int>(deltaX) << "\t"
                                                          << static_cast<int>(deltaY) << std::endl;
        }
        /*
        {
            POINT pos{};
            GetCursorPos(&pos);
            SetCursorPos(pos.x + deltaX, pos.y + deltaY);
        }
        // */

#endif
        DigiMouse.delay(delay);
    }
}

void
shigenoy::vmouse::prepare_supply(const int delay)
{
    // DigiMouse.rightClick();
    // DigiMouse.delay(delay);
    // /*
    {
        // RIGHT Click
        DigiMouse.setButtons(2);
        mouse_move(-1, 0, 1, delay * 10);
        // Unclick
        DigiMouse.setButtons(0);
        mouse_move(1, 0, 1, delay * 10);
    }
    // */
}

void
shigenoy::vmouse::deploy_supply(const int delay)
{

    // DigiMouse.leftClick();
    // DigiMouse.delay(delay);
    // /*
    {
        // Click
        DigiMouse.setButtons(1);
        mouse_move(-1, 0, 1, delay * 20);
        // Unclick
        DigiMouse.setButtons(0);
        mouse_move(1, 0, 1, delay * 20);
    }
    // */
}

void
shigenoy::vmouse::prepare_medic(const int delay)
{
    DigiMouse.scroll(-10);
    mouse_move(-1, 0, 1, delay);
}

void
shigenoy::vmouse::deploy_medic(const int delay)
{
    // Click
    DigiMouse.setButtons(1);
    mouse_move(-1, 0, 1, delay * 20);
    // Unclick
    DigiMouse.setButtons(0);
    mouse_move(1, 0, 1, delay * 20);

    DigiMouse.scroll(10);
    mouse_move(1, 0, 1, delay);
}
