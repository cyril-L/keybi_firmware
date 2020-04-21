#include "keybi/usb.h"

#include "usb_core.h"
#include "CCIDHID_usb_desc.h"

const uint8_t Keybi_Keyboard_ReportDescriptor[KEYBI_KEYBOARD_SIZ_REPORT_DESC] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)

    0xa1, 0x01, // COLLECTION (Application)

    // Modifier byte
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)
    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)

    // Reserved byte
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs) (0x81, 0x01 constant in HID class definition example)

    // LED report
    0x95, 0x05, // REPORT_COUNT (5)
    0x75, 0x01, // REPORT_SIZE (1)
    0x05, 0x08, // USAGE_PAGE (LEDs)
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    0x29, 0x05, // USAGE_MAXIMUM (Kana)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)

    // LED report padding
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x03, // REPORT_SIZE (3)
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)

    // Key arrays (6 bytes)
    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x65, // LOGICAL_MAXIMUM (101)
    0x05, 0x07, // USAGE_PAGE (Key codes)
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)

    0xc0   // END_COLLECTION
};

// used in usb_endp.c

__IO uint8_t keybi_keyboard_prev_xfer_complete = 1;

void Keybi_Keyboard_SendReportCompleted(void) {
    keybi_keyboard_prev_xfer_complete = 1;
}

// used in CCIDHID_usb_prop.c

ONE_DESCRIPTOR Keybi_Keyboard_Report_Descriptor = {
    (uint8_t *) Keybi_Keyboard_ReportDescriptor,
    KEYBI_KEYBOARD_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR Keybi_Keyboard_Hid_Descriptor = {
    (uint8_t *) CCID_ConfigDescriptor + KEYBI_KEYBOARD_KEYBOARD_OFF_HID_DESC,
    KEYBI_KEYBOARD_SIZ_HID_DESC
};

uint8_t* Keybi_Keyboard_GetReportDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Keybi_Keyboard_Report_Descriptor);
}

uint8_t* Keybi_Keyboard_GetHIDDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Keybi_Keyboard_Hid_Descriptor);
}