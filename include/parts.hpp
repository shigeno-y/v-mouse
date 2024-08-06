#ifndef SHIGENO_Y_VMOUSE_PARTS_INCLUDED
#define SHIGENO_Y_VMOUSE_PARTS_INCLUDED

#ifdef _MSC_FULL_VER
#    include <iostream>
#    include <windows.h>
#endif

namespace shigenoy::vmouse {
void mySetup(const unsigned int mode);

void LEDon(const unsigned int mode);
void LEDoff(const unsigned int mode);

void mouse_move(const char deltaX, const char deltaY, const int count, const int delay);

void prepare_supply(const int delay);
void deploy_supply(const int delay);

void prepare_medic(const int delay);
void deploy_medic(const int delay);

} // namespace shigenoy::vmouse

#endif
