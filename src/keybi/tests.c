#include "keybi/tests.h"
#include "CCIDHID_usb_desc.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

void Keybi_Test_Failed(char *file, int line, const char * expr) {
    while (1);
}

void Keybi_Test_ConfigDescriptor(void);

void Keybi_Tests_Run() {
    Keybi_Test_ConfigDescriptor();
}

void Keybi_Test_ConfigDescriptor() {
    KEYBI_ASSERT(sizeof(CCID_ConfigDescriptor) == CCID_SIZ_CONFIG_DESC);
    KEYBI_ASSERT(sizeof(Keybi_Keyboard_ReportDescriptor) == KEYBI_KEYBOARD_SIZ_REPORT_DESC);
    KEYBI_ASSERT(sizeof(Keybi_Mouse_ReportDescriptor) == KEYBI_MOUSE_SIZ_REPORT_DESC);

    KEYBI_ASSERT(CCID_ConfigDescriptor[KEYBOARD_OFF_HID_DESC] == KEYBOARD_SIZ_HID_DESC);
    KEYBI_ASSERT(CCID_ConfigDescriptor[KEYBI_KEYBOARD_OFF_HID_DESC] == KEYBI_KEYBOARD_SIZ_HID_DESC);
    KEYBI_ASSERT(CCID_ConfigDescriptor[KEYBI_MOUSE_OFF_HID_DESC] == KEYBI_MOUSE_SIZ_HID_DESC);
}

#pragma GCC pop_options
