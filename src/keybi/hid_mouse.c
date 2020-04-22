#include "keybi/hid_mouse.h"

#include "CCIDHID_usb_conf.h"
#include "CCIDHID_usb_desc.h"

#include "usb_core.h"
#include "usb_pwr.h"
#include "usb_mem.h"
#include "usb_regs.h"

// TODO Wireshark reports a malformed packet error at push (0xa4)
// Trying to fetch an unsigned integer with length 0
//
// Wheel Mouse - simplified version
//
// Input report - 5 bytes
//
// Byte | D7      D6      D5      D4      D3      D2      D1      D0
//------+---------------------------------------------------------------------
//  0   |  0       0       0    Forward  Back    Middle  Right   Left (Button)
//  1   |                             X
//  2   |                             Y
//  3   |                       Vertical Wheel
//  4   |                    Horizontal (Tilt) Wheel
//
// Feature report - 1 byte
//
//  Byte | D7      D6      D5      D4   |  D3      D2  |   D1      D0
// ------+------------------------------+--------------+----------------
//   0   |  0       0       0       0   |  Horizontal  |    Vertical
//                                          (Resolution multiplier)
// Reference
//    Tsuneo on https://www.microchip.com/forums/tm.aspx?m=391435
//    Wheel.docx in "Enhanced Wheel Support in Windows Vista" on MS WHDC
//    http://www.microsoft.com/whdc/device/input/wheel.mspx

const uint8_t Keybi_Mouse_ReportDescriptor[KEYBI_MOUSE_SIZ_REPORT_DESC] = {
    0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,        // USAGE (Mouse)
    0xa1, 0x01,        // COLLECTION (Application)
    0x09, 0x02,        //   USAGE (Mouse)
    0xa1, 0x02,        //   COLLECTION (Logical)
    0x09, 0x01,        //     USAGE (Pointer)
    0xa1, 0x00,        //     COLLECTION (Physical)

    // Buttons
    0x05, 0x09,        //       USAGE_PAGE (Button)
    0x19, 0x01,        //       USAGE_MINIMUM (Button 1)
    0x29, 0x05,        //       USAGE_MAXIMUM (Button 5)
    0x15, 0x00,        //       LOGICAL_MINIMUM (0)
    0x25, 0x01,        //       LOGICAL_MAXIMUM (1)
    0x75, 0x01,        //       REPORT_SIZE (1)
    0x95, 0x05,        //       REPORT_COUNT (5)
    0x81, 0x02,        //       INPUT (Data,Var,Abs)

    // Padding
    0x75, 0x03,        //       REPORT_SIZE (3)
    0x95, 0x01,        //       REPORT_COUNT (1)
    0x81, 0x03,        //       INPUT (Cnst,Var,Abs)

    // X,Y position
    0x05, 0x01,        //       USAGE_PAGE (Generic Desktop)
    0x09, 0x30,        //       USAGE (X)
    0x09, 0x31,        //       USAGE (Y)
    0x15, 0x81,        //       LOGICAL_MINIMUM (-127)
    0x25, 0x7f,        //       LOGICAL_MAXIMUM (127)
    0x75, 0x08,        //       REPORT_SIZE (8)
    0x95, 0x02,        //       REPORT_COUNT (2)
    0x81, 0x06,        //       INPUT (Data,Var,Rel)
    0xa1, 0x02,        //       COLLECTION (Logical)

    // Vertical wheel res multiplier
    0x09, 0x48,        //         USAGE (Resolution Multiplier)
    0x15, 0x00,        //         LOGICAL_MINIMUM (0)
    0x25, 0x01,        //         LOGICAL_MAXIMUM (1)
    0x35, 0x01,        //         PHYSICAL_MINIMUM (1)
    0x45, 0x04,        //         PHYSICAL_MAXIMUM (4)
    0x75, 0x02,        //         REPORT_SIZE (2)
    0x95, 0x01,        //         REPORT_COUNT (1)
    0xa4,              //         PUSH
    0xb1, 0x02,        //         FEATURE (Data,Var,Abs)

    // Vertical wheel
    0x09, 0x38,        //         USAGE (Wheel)
    0x15, 0x81,        //         LOGICAL_MINIMUM (-127)
    0x25, 0x7f,        //         LOGICAL_MAXIMUM (127)
    0x35, 0x00,        //         PHYSICAL_MINIMUM (0)        - reset physical
    0x45, 0x00,        //         PHYSICAL_MAXIMUM (0)
    0x75, 0x08,        //         REPORT_SIZE (8)
    0x81, 0x06,        //         INPUT (Data,Var,Rel)
    0xc0,              //       END_COLLECTION
    0xa1, 0x02,        //       COLLECTION (Logical)

    // Horizontal wheel res multiplier
    0x09, 0x48,        //         USAGE (Resolution Multiplier)
    0xb4,              //         POP
    0xb1, 0x02,        //         FEATURE (Data,Var,Abs)

    // Padding for Feature report
    0x35, 0x00,        //         PHYSICAL_MINIMUM (0)        - reset physical
    0x45, 0x00,        //         PHYSICAL_MAXIMUM (0)
    0x75, 0x04,        //         REPORT_SIZE (4)
    0xb1, 0x03,        //         FEATURE (Cnst,Var,Abs)

    // Horizontal wheel
    0x05, 0x0c,        //         USAGE_PAGE (Consumer Devices)
    0x0a, 0x38, 0x02,  //         USAGE (AC Pan)
    0x15, 0x81,        //         LOGICAL_MINIMUM (-127)
    0x25, 0x7f,        //         LOGICAL_MAXIMUM (127)
    0x75, 0x08,        //         REPORT_SIZE (8)
    0x81, 0x06,        //         INPUT (Data,Var,Rel)
    0xc0,              //       END_COLLECTION
    0xc0,              //     END_COLLECTION
    0xc0,              //   END_COLLECTION
    0xc0               // END_COLLECTION
 };


static __IO uint8_t prev_xfer_complete = 1;

void Keybi_Mouse_SendReport(uint8_t * report) {

    if (bDeviceState == CONFIGURED)
    {
        while (!prev_xfer_complete);

        prev_xfer_complete = 0;
        UserToPMABufferCopy(report, CCID_ENDP6_TXADDR, KEYBI_MOUSE_SIZ_REPORT);

        SetEPTxCount(ENDP6, KEYBI_MOUSE_SIZ_REPORT);
        SetEPTxStatus(ENDP6, EP_TX_VALID);
    }
}

// called in usb_endp.c

void Keybi_Mouse_SendReportCompleted(void) {
    prev_xfer_complete = 1;
}

// called in CCIDHID_usb_prop.c

ONE_DESCRIPTOR Keybi_Mouse_Report_Descriptor = {
    (uint8_t *) Keybi_Mouse_ReportDescriptor,
    KEYBI_MOUSE_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR Keybi_Mouse_Hid_Descriptor = {
    (uint8_t *) CCID_ConfigDescriptor + KEYBI_MOUSE_OFF_HID_DESC,
    KEYBI_MOUSE_SIZ_HID_DESC
};

uint8_t* Keybi_Mouse_GetReportDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Keybi_Mouse_Report_Descriptor);
}

uint8_t* Keybi_Mouse_GetHIDDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Keybi_Mouse_Hid_Descriptor);
}
