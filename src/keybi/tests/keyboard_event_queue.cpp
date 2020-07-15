#include <gtest/gtest.h>
#include <cstring>

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

class KeyboardEventQueue : public ::testing::Test {

 protected:

    uint8_t report[8] = {0};
    keybi_keyboard_event_t queue_data[16];
    keybi_keyboard_event_queue_t queue;

    KeyboardEventQueue() :
        report {0},
        queue_data {0,0},
        queue({
            .events = queue_data,
            .head = 0,
            .size = 0,
            .capacity = 16
        })
    {}
};

bool ReportsEq(uint8_t expected[8], uint8_t actual[8]) {
    return 0 == std::memcmp(expected, actual, 8);
}

std::ostream &operator<<(std::ostream &os, const uint8_t (& report) [8]) {
    os << "{";
    for (int i = 0; i < 8; ++i) {
        os << " " << (int) report[i];
    }
    os << " }";
    return os;
}

TEST_F(KeyboardEventQueue, HandlesASingleKeypress) {
    keybi_keyboard_event_t events[] = {
        {KC_A, 1},
        {KC_A, 0}
    };
    Keybi_Keyboard_QueueEvents(&queue, events, 2);

    uint8_t expected_reports[2][8] = {
        {0, 0, KC_A, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    for (int i = 0; i < 2; ++i) {
        ASSERT_TRUE(Keybi_Keyboard_QueueToReport(&queue, report));
        ASSERT_TRUE(ReportsEq(expected_reports[i], report)) << i << ": " << report;
    }
    ASSERT_FALSE(Keybi_Keyboard_QueueToReport(&queue, report));
}

TEST_F(KeyboardEventQueue, TellsWhenFull) {
    for (unsigned i = 0; i < queue.capacity; ++i) {
        ASSERT_EQ(0, Keybi_Keyboard_QueueEvent(&queue, {KC_A, 1}));
    }
    ASSERT_NE(0, Keybi_Keyboard_QueueEvent(&queue, {KC_B, 1}));
    // B has not been added
    for (unsigned i = 0; i < queue.capacity; ++i) {
        ASSERT_EQ(KC_A, queue.events[i].keycode);
    }
}

TEST_F(KeyboardEventQueue, HandlesMultipleKeypresses) {
    keybi_keyboard_event_t events[] = {
        {KC_A, 1},
        {KC_B, 1},
        {KC_C, 1},
        {KC_B, 0}
    };
    Keybi_Keyboard_QueueEvents(&queue, events, 3);

    uint8_t expected_reports[4][8] = {
        {0, 0, KC_A, 0, 0, 0, 0, 0},
        {0, 0, KC_A, KC_B, 0, 0, 0, 0},
        {0, 0, KC_A, KC_B, KC_C, 0, 0, 0},
        {0, 0, KC_A, 0, KC_C, 0, 0, 0}
    };

    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(Keybi_Keyboard_QueueToReport(&queue, report));
        ASSERT_TRUE(ReportsEq(expected_reports[i], report)) << i << ": " << report;
    }
    ASSERT_FALSE(Keybi_Keyboard_QueueToReport(&queue, report));
}

TEST_F(KeyboardEventQueue, HandlesModpresses) {

    keybi_keyboard_event_t events[] = {
        {KC_LCTL, 1},
        {KC_LCTL, 0}
    };
    Keybi_Keyboard_QueueEvents(&queue, events, 2);

    uint8_t expected_reports[][8] = {
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    for (int i = 0; i < 2; ++i) {
        ASSERT_TRUE(Keybi_Keyboard_QueueToReport(&queue, report));
        ASSERT_TRUE(ReportsEq(expected_reports[i], report)) << i << ": " << report;
    }
    ASSERT_FALSE(Keybi_Keyboard_QueueToReport(&queue, report));
}

TEST_F(KeyboardEventQueue, DoNotRegisterTheSameKeycodeMultipleTimes) {
    // On keymaps, more than one key may be mapped to the same keycode
    // so the same keycode might be pressed two times in the queue

    keybi_keyboard_event_t events[] = {
        {KC_UP, 1},
        {KC_UP, 1},
        {KC_UP, 0},
        {KC_UP, 0}
    };
    Keybi_Keyboard_QueueEvents(&queue, events, 4);

    uint8_t expected_reports[4][8] = {
        {0, 0, KC_UP, 0, 0, 0, 0, 0},
        {0, 0, KC_UP, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    for (int i = 0; i < 4; ++i) {
        EXPECT_TRUE(Keybi_Keyboard_QueueToReport(&queue, report));
        EXPECT_TRUE(ReportsEq(expected_reports[i], report)) << i << ": " << report;
    }
    EXPECT_FALSE(Keybi_Keyboard_QueueToReport(&queue, report));
}