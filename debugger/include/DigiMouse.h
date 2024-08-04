/*
This file is DUMMY for plain c++ debug.
*/
#ifndef MY_DIGIMOUSE_H_INCLUDED
#define MY_DIGIMOUSE_H_INCLUDED

#include <chrono>
#include <cstdint>
#include <iostream>

#define HIGH   255
#define LOW    0
#define OUTPUT 1

typedef std::uint8_t uchar;

#define PROGMEM
#define REPORT_SIZE 4

static const uchar* rt_usbHidReportDescriptor = NULL;
static uchar rt_usbHidReportDescriptorSize    = 0;
static const uchar* rt_usbDeviceDescriptor    = NULL;
static uchar rt_usbDeviceDescriptorSize       = 0;

#define MOUSEBTN_LEFT_MASK   0x01
#define MOUSEBTN_RIGHT_MASK  0x02
#define MOUSEBTN_MIDDLE_MASK 0x04

// TODO: Work around Arduino 12 issues better.
//#include <WConstants.h>
//#undef int()

typedef uint8_t byte;

/* What was most recently read from the controller */
static unsigned char last_built_report[REPORT_SIZE];

/* What was most recently sent to the host */
static unsigned char last_sent_report[REPORT_SIZE];

uchar reportBuffer[REPORT_SIZE];

// report frequency set to default of 50hz
#define DIGIMOUSE_DEFAULT_REPORT_INTERVAL 20
static unsigned char must_report = 0;
static unsigned char idle_rate   = DIGIMOUSE_DEFAULT_REPORT_INTERVAL / 4; // in units of 4ms
// new minimum report frequency system:
static unsigned long last_report_time = 0;

char usb_hasCommed = 0;

const PROGMEM unsigned char mouse_usbHidReportDescriptor[] = {
    /* USB report descriptor */
    (unsigned char)0x05, (unsigned char)0x01, // USAGE_PAGE (Generic Desktop)
    (unsigned char)0x09, (unsigned char)0x02, // USAGE (Mouse)
    (unsigned char)0xa1, (unsigned char)0x01, // COLLECTION (Application)
    (unsigned char)0x09, (unsigned char)0x01, //		USAGE_PAGE (Pointer)
    (unsigned char)0xa1, (unsigned char)0x00, //		COLLECTION (Physical)
    (unsigned char)0x05, (unsigned char)0x09, //		USAGE_PAGE (Button)
    (unsigned char)0x19, (unsigned char)0x01, //		USAGE_MINIMUM (Button 1)
    (unsigned char)0x29, (unsigned char)0x03, //		USAGE_MAXIMUM (Button 3)
    (unsigned char)0x15, (unsigned char)0x00, //		LOGICAL_MINIMUM (0)
    (unsigned char)0x25, (unsigned char)0x01, //		LOGICAL_MAXIMUM (1)
    (unsigned char)0x95, (unsigned char)0x03, //		REPORT_COUNT (3)
    (unsigned char)0x75, (unsigned char)0x01, //		REPORT_SIZE (1)
    (unsigned char)0x81, (unsigned char)0x02, //		INPUT (Data,Var,Abs)
    (unsigned char)0x95, (unsigned char)0x01, //		REPORT_COUNT (1)
    (unsigned char)0x75, (unsigned char)0x05, //		REPORT_SIZE (5)
    (unsigned char)0x81, (unsigned char)0x01, //		Input(Cnst)
    (unsigned char)0x05, (unsigned char)0x01, //		USAGE_PAGE(Generic Desktop)
    (unsigned char)0x09, (unsigned char)0x30, //		USAGE(X)
    (unsigned char)0x09, (unsigned char)0x31, //		USAGE(Y)
    (unsigned char)0x15, (unsigned char)0x81, //		LOGICAL_MINIMUM (-127)
    (unsigned char)0x25, (unsigned char)0x7f, //		LOGICAL_MAXIMUM (127)
    (unsigned char)0x75, (unsigned char)0x08, //		REPORT_SIZE (8)
    (unsigned char)0x95, (unsigned char)0x02, //		REPORT_COUNT (2)
    (unsigned char)0x81, (unsigned char)0x06, //		INPUT (Data,Var,Rel)
    (unsigned char)0x09, (unsigned char)0x38, //	 Usage (Wheel)
    (unsigned char)0x95, (unsigned char)0x01, //	 Report Count (1),
    (unsigned char)0x81, (unsigned char)0x06, //	 Input (Data, Variable, Relative)
    (unsigned char)0xc0,                      // END_COLLECTION
    (unsigned char)0xc0                       // END_COLLECTION
};

#define USBDESCR_DEVICE 1

const unsigned char usbDescrDevice[] PROGMEM = {
    /* USB device descriptor */
    18,              /* sizeof(usbDescrDevice): length of descriptor in bytes */
    USBDESCR_DEVICE, /* descriptor type */
    0x01,
    0x01, /* USB version supported */
    0,
    0,
    0, /* protocol */
    8, /* max packet size */
    0, /* 2 bytes */
    0, /* 2 bytes */
    0, /* 2 bytes */
#if USB_CFG_VENDOR_NAME_LEN
    1, /* manufacturer string index */
#else
    0, /* manufacturer string index */
#endif
#if USB_CFG_DEVICE_NAME_LEN
    2, /* product string index */
#else
    0, /* product string index */
#endif
#if USB_CFG_SERIAL_NUMBER_LENGTH
    3, /* serial number string index */
#else
    0, /* serial number string index */
#endif
    1, /* number of configurations */
};

namespace {
long long
millis()
{
    const std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

void
usbSetInterrupt(uchar reportBuffer[REPORT_SIZE], size_t report_size)
{
    /*
    std::cout << "usbSetInterrupt\t";
    for (size_t idx = 0; idx < report_size; ++idx)
    {
        std::cout << static_cast<int>(reportBuffer[idx]) << "\t";
    }
    std::cout << std::endl;
    // */
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

} // namespace

void
buildReport(unsigned char* reportBuf)
{
    if (reportBuf != NULL)
    {
        memcpy(reportBuf, last_built_report, REPORT_SIZE);
    }

    memcpy(last_sent_report, last_built_report, REPORT_SIZE);
}

void
clearMove()
{
    // because we send deltas in movement, so when we send them, we clear them
    last_built_report[1] = 0;
    last_built_report[2] = 0;
    last_built_report[3] = 0;
    last_sent_report[1]  = 0;
    last_sent_report[2]  = 0;
    last_sent_report[3]  = 0;
}

class DigiMouseDevice
{
public:
    DigiMouseDevice()
    {
        rt_usbHidReportDescriptor     = mouse_usbHidReportDescriptor;
        rt_usbHidReportDescriptorSize = sizeof(mouse_usbHidReportDescriptor);
        rt_usbDeviceDescriptor        = usbDescrDevice;
        rt_usbDeviceDescriptorSize    = sizeof(usbDescrDevice);
    }

    void begin() { last_report_time = millis(); }

    void refresh() { update(); }

    void poll() { update(); }

    void update()
    {
        // instead of above code, use millis arduino system to enforce minimum reporting frequency
        unsigned long time_since_last_report = millis() - last_report_time;
        if (time_since_last_report >= (idle_rate * 4 /* in units of 4ms - usb spec stuff */))
        {
            last_report_time += idle_rate * 4;
            must_report = 1;
        }

        // if the report has changed, try force an update anyway
        if (memcmp(last_built_report, last_sent_report, REPORT_SIZE))
        {
            must_report = 1;
        }

        // if we want to send a report, signal the host computer to ask us for it with a usb 'interrupt'
        if (must_report)
        {
            if (true /*usbInterruptIsReady() */)
            {
                must_report = 0;
                buildReport(reportBuffer); // put data into reportBuffer
                clearMove();               // clear deltas
                usbSetInterrupt(reportBuffer, REPORT_SIZE);
            }
        }
    }

    // delay while updating until we are finished
    void delay(long milli)
    {
        unsigned long last = millis();
        while (milli > 0)
        {
            unsigned long now = millis();
            milli -= now - last;
            last = now;
            update();
        }
    }

    void moveX(char deltaX)
    {
        if (deltaX == -128)
            deltaX = -127;
        last_built_report[1] = *(reinterpret_cast<unsigned char*>(&deltaX));
    }

    void moveY(char deltaY)
    {
        if (deltaY == -128)
            deltaY = -127;
        last_built_report[2] = *(reinterpret_cast<unsigned char*>(&deltaY));
    }

    void scroll(char deltaS)
    {
        if (deltaS == -128)
            deltaS = -127;
        last_built_report[3] = *(reinterpret_cast<unsigned char*>(&deltaS));
    }

    void move(char deltaX, char deltaY, char deltaS)
    {
        if (deltaX == -128)
            deltaX = -127;
        if (deltaY == -128)
            deltaY = -127;
        if (deltaS == -128)
            deltaS = -127;
        last_built_report[1] = *(reinterpret_cast<unsigned char*>(&deltaX));
        last_built_report[2] = *(reinterpret_cast<unsigned char*>(&deltaY));
        last_built_report[3] = *(reinterpret_cast<unsigned char*>(&deltaS));
    }

    void move(char deltaX, char deltaY, char deltaS, char buttons)
    {
        if (deltaX == -128)
            deltaX = -127;
        if (deltaY == -128)
            deltaY = -127;
        if (deltaS == -128)
            deltaS = -127;
        last_built_report[0] = buttons;
        last_built_report[1] = *(reinterpret_cast<unsigned char*>(&deltaX));
        last_built_report[2] = *(reinterpret_cast<unsigned char*>(&deltaY));
        last_built_report[3] = *(reinterpret_cast<unsigned char*>(&deltaS));
    }

    void rightClick() { last_built_report[0] = MOUSEBTN_RIGHT_MASK; }

    void leftClick() { last_built_report[0] = MOUSEBTN_LEFT_MASK; }

    void middleClick() { last_built_report[0] = MOUSEBTN_MIDDLE_MASK; }

    void setButtons(unsigned char buttons) { last_built_report[0] = buttons; }

    void setValues(unsigned char values[]) { memcpy(last_built_report, values, REPORT_SIZE); }

    //private: TODO: Make friend?
    // what does this even mean? -- Bluebie
};

// create the global singleton DigiMouse
DigiMouseDevice DigiMouse = DigiMouseDevice();

#ifdef __cplusplus
extern "C" {
#endif
// USB_PUBLIC uchar usbFunctionSetup

uchar
usbFunctionSetup(uchar data[8])
{
    return 0;
}

uchar
usbFunctionDescriptor(struct usbRequest* rq)
{
    return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MY_DIGIMOUSE_H_INCLUDED
