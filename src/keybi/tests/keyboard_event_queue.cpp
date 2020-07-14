#include <gtest/gtest.h>
#include <cstring>
#include <sstream>

extern "C" {
    #include "keybi/hid_keyboard.h"
    #include "keybi/qmk/keycode.h"
    #include "usb_core.h"

    // Stubs to avoid undefined references
    __IO uint32_t bDeviceState;
    void UserToPMABufferCopy (uint8_t *, uint16_t, uint16_t) {}
    void SetEPTxCount (uint8_t, uint16_t) {}
    void SetEPTxStatus (uint8_t, uint16_t) {}
    uint8_t* Standard_GetDescriptorData (uint16_t, ONE_DESCRIPTOR *) { return NULL; }
}

keybi_keyboard_event_queue_t MakeQueue(keybi_keyboard_event_t * events, uint32_t capacity) {
    keybi_keyboard_event_queue_t queue = {
        .events = events,
        .head = 0,
        .size = 0,
        .capacity = capacity
    };
    return queue;
}

bool ReportsEq(uint8_t expected[8], uint8_t actual[8]) {
    return 0 == std::memcmp(expected, actual, 8);
}

std::string ReportAsStr(uint8_t report[8]) {
    std::ostringstream s;
    s << "{";
    for (int i = 0; i < 8; ++i) {
        s << " " << (int) report[i];
    }
    s << " }";
    return s.str();
}

TEST(KeyboardEventQueue, HandlesASingleKeypress) {
    uint8_t report[8] = {0};
    keybi_keyboard_event_t events[8];
    keybi_keyboard_event_queue_t  queue = MakeQueue(events, 8);
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_A, .pressed = 1});
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_A, .pressed = 0});
    {
        ASSERT_TRUE(Keybi_Keyboard_QueueToReport(&queue, report));
        uint8_t report_expected[8] = {0, 0, KC_A, 0, 0, 0, 0, 0};
        EXPECT_TRUE(ReportsEq(report_expected, report)) << ReportAsStr(report);
    }
    {
        ASSERT_TRUE(Keybi_Keyboard_QueueToReport(&queue, report));
        uint8_t report_expected[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_TRUE(ReportsEq(report_expected, report)) << ReportAsStr(report);
    }
    ASSERT_FALSE(Keybi_Keyboard_QueueToReport(&queue, report));
}

TEST(KeyboardEventQueue, TellsWhenFull) {
    // A queue that can hold 2 events
    keybi_keyboard_event_t events[2];
    keybi_keyboard_event_queue_t  queue = MakeQueue(events, 2);
    ASSERT_EQ(0, Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_A, .pressed = 1}));
    ASSERT_EQ(0, Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_B, .pressed = 1}));
    ASSERT_NE(0, Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_C, .pressed = 1}));
    ASSERT_EQ(KC_A, events[0].keycode);
    ASSERT_EQ(KC_B, events[1].keycode);
    // B has not been added
}

TEST(KeyboardEventQueue, HandlesMultipleKeypresses) {
    uint8_t report[8] = {0};
    keybi_keyboard_event_t events[8];
    keybi_keyboard_event_queue_t  queue = MakeQueue(events, 8);
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_A, .pressed = 1});
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_B, .pressed = 1});
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_C, .pressed = 1});
    int generated_reports = 0;
    while (Keybi_Keyboard_QueueToReport(&queue, report)) { generated_reports++; }
    // for now 3 reports should have been generated
    // TODO just generate one
    ASSERT_EQ(3, generated_reports);

    {
        uint8_t report_expected[8] = {0, 0, KC_A, KC_B, KC_C, 0, 0, 0};
        EXPECT_TRUE(ReportsEq(report_expected, report)) << ReportAsStr(report);
    }

    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_B, .pressed = 0});
    Keybi_Keyboard_QueueToReport(&queue, report);

    {
        uint8_t report_expected[8] = {0, 0, KC_A, 0, KC_C, 0, 0, 0};
        EXPECT_TRUE(ReportsEq(report_expected, report)) << ReportAsStr(report);
    }
}

TEST(KeyboardEventQueue, HandlesModpresses) {
    uint8_t report[8] = {0};
    keybi_keyboard_event_t events[8];
    keybi_keyboard_event_queue_t  queue = MakeQueue(events, 8);
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_LCTL, .pressed = 1});
    Keybi_Keyboard_QueueEvent(&queue, {.keycode = KC_LCTL, .pressed = 0});

    {
        Keybi_Keyboard_QueueToReport(&queue, report);
        uint8_t report_expected[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_TRUE(ReportsEq(report_expected, report)) << ReportAsStr(report);
    }
    {
        Keybi_Keyboard_QueueToReport(&queue, report);
        uint8_t report_expected[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_TRUE(ReportsEq(report_expected, report)) << ReportAsStr(report);
    }
}