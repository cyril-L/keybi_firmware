#include <gtest/gtest.h>

extern "C" {
	#include "CCIDHID_usb_desc.h"
}

TEST(UsbDescriptors, DefinesConsistentSizes) {
    ASSERT_EQ(CCID_SIZ_CONFIG_DESC, sizeof(CCID_ConfigDescriptor));
    ASSERT_EQ(KEYBI_KEYBOARD_SIZ_REPORT_DESC, sizeof(Keybi_Keyboard_ReportDescriptor));
    ASSERT_EQ(KEYBI_MOUSE_SIZ_REPORT_DESC, sizeof(Keybi_Mouse_ReportDescriptor));

    ASSERT_EQ(KEYBOARD_SIZ_HID_DESC, CCID_ConfigDescriptor[KEYBOARD_OFF_HID_DESC]);
    ASSERT_EQ(KEYBI_KEYBOARD_SIZ_HID_DESC, CCID_ConfigDescriptor[KEYBI_KEYBOARD_OFF_HID_DESC]);
    ASSERT_EQ(KEYBI_MOUSE_SIZ_HID_DESC, CCID_ConfigDescriptor[KEYBI_MOUSE_OFF_HID_DESC]);
}
