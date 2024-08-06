/*
This file is DUMMY for plain c++ debug.
*/

#include "DigiMouse.h"

const uchar* rt_usbHidReportDescriptor = NULL;
const uchar* rt_usbDeviceDescriptor    = NULL;

uchar rt_usbHidReportDescriptorSize = 0;
uchar rt_usbDeviceDescriptorSize    = 0;

unsigned char last_built_report[REPORT_SIZE];
unsigned char last_sent_report[REPORT_SIZE];
uchar reportBuffer[REPORT_SIZE];
unsigned char must_report      = 0;
unsigned char idle_rate        = DIGIMOUSE_DEFAULT_REPORT_INTERVAL / 4; // in units of 4ms
unsigned long last_report_time = 0;
char usb_hasCommed             = 0;

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

long long
shigenoy::vmouse::millis()
{
    const std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

void
shigenoy::vmouse::usbSetInterrupt(uchar reportBuffer[REPORT_SIZE], size_t report_size)
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
shigenoy::vmouse::pinMode(const int mode, const int type)
{
    std::cout << "pinMode\t#" << mode << "\t" << type << std::endl;
}

void
shigenoy::vmouse::digitalWrite(const int mode, const int val)
{
    std::cout << "DigitalWrite\t#" << mode << "\t" << val << std::endl;
}

void
shigenoy::vmouse::buildReport(unsigned char* reportBuf)
{
    if (reportBuf != NULL)
    {
        memcpy(reportBuf, last_built_report, REPORT_SIZE);
    }

    memcpy(last_sent_report, last_built_report, REPORT_SIZE);
}

void
shigenoy::vmouse::clearMove()
{
    // because we send deltas in movement, so when we send them, we clear them
    last_built_report[1] = 0;
    last_built_report[2] = 0;
    last_built_report[3] = 0;
    last_sent_report[1]  = 0;
    last_sent_report[2]  = 0;
    last_sent_report[3]  = 0;
}

shigenoy::vmouse::DigiMouseDevice::DigiMouseDevice()
{
    rt_usbHidReportDescriptor     = mouse_usbHidReportDescriptor;
    rt_usbHidReportDescriptorSize = sizeof(mouse_usbHidReportDescriptor);
    rt_usbDeviceDescriptor        = usbDescrDevice;
    rt_usbDeviceDescriptorSize    = sizeof(usbDescrDevice);
}

void
shigenoy::vmouse::DigiMouseDevice::begin()
{
    last_report_time = millis();
}

void
shigenoy::vmouse::DigiMouseDevice::refresh()
{
    update();
}

void
shigenoy::vmouse::DigiMouseDevice::poll()
{
    update();
}

void
shigenoy::vmouse::DigiMouseDevice::update()
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
void
shigenoy::vmouse::DigiMouseDevice::delay(long milli)
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

void
shigenoy::vmouse::DigiMouseDevice::moveX(char deltaX)
{
    if (deltaX == -128)
        deltaX = -127;
    last_built_report[1] = *(reinterpret_cast<unsigned char*>(&deltaX));
}

void
shigenoy::vmouse::DigiMouseDevice::moveY(char deltaY)
{
    if (deltaY == -128)
        deltaY = -127;
    last_built_report[2] = *(reinterpret_cast<unsigned char*>(&deltaY));
}

void
shigenoy::vmouse::DigiMouseDevice::scroll(char deltaS)
{
    if (deltaS == -128)
        deltaS = -127;
    last_built_report[3] = *(reinterpret_cast<unsigned char*>(&deltaS));
}

void
shigenoy::vmouse::DigiMouseDevice::move(char deltaX, char deltaY, char deltaS)
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

void
shigenoy::vmouse::DigiMouseDevice::move(char deltaX, char deltaY, char deltaS, char buttons)
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

void
shigenoy::vmouse::DigiMouseDevice::rightClick()
{
    last_built_report[0] = MOUSEBTN_RIGHT_MASK;
}

void
shigenoy::vmouse::DigiMouseDevice::leftClick()
{
    last_built_report[0] = MOUSEBTN_LEFT_MASK;
}

void
shigenoy::vmouse::DigiMouseDevice::middleClick()
{
    last_built_report[0] = MOUSEBTN_MIDDLE_MASK;
}

void
shigenoy::vmouse::DigiMouseDevice::setButtons(unsigned char buttons)
{
    last_built_report[0] = buttons;
}

void
shigenoy::vmouse::DigiMouseDevice::setValues(unsigned char values[])
{
    memcpy(last_built_report, values, REPORT_SIZE);
}

// create the global singleton DigiMouse
shigenoy::vmouse::DigiMouseDevice shigenoy::vmouse::DigiMouse{};

uchar
shigenoy::vmouse::usbFunctionSetup(uchar data[8])
{
    return 0;
}

uchar
shigenoy::vmouse::usbFunctionDescriptor(struct usbRequest* rq)
{
    return 0;
}
