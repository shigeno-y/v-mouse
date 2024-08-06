/*
This file is DUMMY for plain c++ debug.
*/
#ifdef _MSC_FULL_VER
#    ifndef MY_DIGIMOUSE_H_INCLUDED
#        define MY_DIGIMOUSE_H_INCLUDED

#        include <chrono>
#        include <cstdint>
#        include <iostream>

#        define HIGH   255
#        define LOW    0
#        define OUTPUT 1

typedef std::uint8_t uchar;
typedef uint8_t byte;

#        define PROGMEM
#        define REPORT_SIZE 4

extern const uchar* rt_usbHidReportDescriptor;
extern const uchar* rt_usbDeviceDescriptor;

extern uchar rt_usbHidReportDescriptorSize;
extern uchar rt_usbDeviceDescriptorSize;

#        define MOUSEBTN_LEFT_MASK                0x01
#        define MOUSEBTN_RIGHT_MASK               0x02
#        define MOUSEBTN_MIDDLE_MASK              0x04
#        define DIGIMOUSE_DEFAULT_REPORT_INTERVAL 20
#        define USBDESCR_DEVICE                   1

extern unsigned char last_built_report[REPORT_SIZE];
extern unsigned char last_sent_report[REPORT_SIZE];

extern uchar reportBuffer[REPORT_SIZE];
extern unsigned char must_report;
extern unsigned char idle_rate;
extern unsigned long last_report_time;
extern char usb_hasCommed;

extern const PROGMEM unsigned char mouse_usbHidReportDescriptor[56];

extern const unsigned char usbDescrDevice[15] PROGMEM;

namespace shigenoy::vmouse {
long long millis();

void usbSetInterrupt(uchar reportBuffer[REPORT_SIZE], size_t report_size);

void pinMode(const int mode, const int type);

void digitalWrite(const int mode, const int val);

void buildReport(unsigned char* reportBuf);

void clearMove();

class DigiMouseDevice
{
public:
    DigiMouseDevice();

    void begin();

    void refresh();

    void poll();

    void update();

    // delay while updating until we are finished
    void delay(long milli);

    void moveX(char deltaX);
    void moveY(char deltaY);
    void scroll(char deltaS);
    void move(char deltaX, char deltaY, char deltaS);
    void move(char deltaX, char deltaY, char deltaS, char buttons);
    void rightClick();
    void leftClick();
    void middleClick();
    void setButtons(unsigned char buttons);
    void setValues(unsigned char values[]);

    //private: TODO: Make friend?
    // what does this even mean? -- Bluebie
};

// create the global singleton DigiMouse
extern DigiMouseDevice DigiMouse;

uchar usbFunctionSetup(uchar data[8]);

uchar usbFunctionDescriptor(struct usbRequest* rq);

} // namespace shigenoy::vmouse::digimouse

#    endif // MY_DIGIMOUSE_H_INCLUDED
#endif
