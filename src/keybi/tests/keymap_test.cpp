#include <gtest/gtest.h>
#include <stdexcept>

extern "C" {
    #include "keybi/keymap.h"
    #include "keybi/qmk/keycode.h"
}

class Keymap : public ::testing::Test {

 protected:

    Keymap() {
        keybi_keymap_events.size = 0;
        keybi_keyboard_layer = L_BASE;
    }
};

struct event_to_make_t {
    uint16_t keycode;
    uint8_t pressed;
};

keybi_keyboard_matrix_event_t MakeMatrixEvent(event_to_make_t event) {
    switch (event.keycode) {
        case KC_GRV:       return {0, 0, event.pressed, 0};
        case KC_A:         return {1, 2, event.pressed, 0};
        case CL_FN_SWITCH: return {8, 4, event.pressed, 0};
    }
    throw std::invalid_argument("keycode not handled");
}

bool operator==(keybi_keyboard_event_t const& a, keybi_keyboard_event_t const& b) {
    return a.keycode == b.keycode && a.pressed == b.pressed;
}

keybi_keyboard_event_t GetQueuedElement(uint32_t i) {
    if (i >= keybi_keymap_events.size) {
        throw std::out_of_range("out of range");
    }
    i = (i + keybi_keymap_events.head) % keybi_keymap_events.capacity;
    return keybi_keymap_events.events[i];
}

std::ostream &operator<<(std::ostream &os, keybi_keyboard_event_t const &event) {
    return os << "{" << (int) event.keycode << ", " << (int) event.pressed << " }";
}

TEST_F(Keymap, HandlesBaseLayerKeys) {
    event_to_make_t events[] = {
        {KC_A, 1},
        {KC_A, 0}
    };
    for (int i = 0; i < 2; ++i) {
        auto matrix_event = MakeMatrixEvent(events[i]);
        ASSERT_EQ(0, Keybi_Keymap_EventHandler(matrix_event));
    }
    keybi_keyboard_event_t expected_events[] = {
        {KC_A, 1},
        {KC_A, 0}
    };
    for (unsigned i = 0; i < 2; ++i) {
        keybi_keyboard_event_t event = GetQueuedElement(i);
        ASSERT_TRUE(expected_events[i] == event) << i << ": " << event;
    }
}

TEST_F(Keymap, HandlesFnLayerKeys) {
    event_to_make_t events[] = {
        {CL_FN_SWITCH, 1},
        {KC_GRV, 1},
        {KC_GRV, 0},
        {CL_FN_SWITCH, 0}
    };
    for (int i = 0; i < 4; ++i) {
        auto matrix_event = MakeMatrixEvent(events[i]);
        ASSERT_EQ(0, Keybi_Keymap_EventHandler(matrix_event));
    }
    keybi_keyboard_event_t expected_events[] = {
        {KC_PWR, 1},
        {KC_PWR, 0}
    };
    for (unsigned i = 0; i < 2; ++i) {
        keybi_keyboard_event_t event = GetQueuedElement(i);
        ASSERT_TRUE(expected_events[i] == event) << i << ": " << event;
    }
}

TEST_F(Keymap, HandlesRollingFnLayerKeys) {
    event_to_make_t events[] = {
        {CL_FN_SWITCH, 1},
        {KC_GRV, 1},
        {CL_FN_SWITCH, 0},
        {KC_GRV, 0}
    };
    for (int i = 0; i < 4; ++i) {
        auto matrix_event = MakeMatrixEvent(events[i]);
        ASSERT_EQ(0, Keybi_Keymap_EventHandler(matrix_event));
    }
    keybi_keyboard_event_t expected_events[] = {
        {KC_PWR, 1},
        {KC_PWR, 0}
    };
    for (unsigned i = 0; i < 2; ++i) {
        keybi_keyboard_event_t event = GetQueuedElement(i);
        ASSERT_TRUE(expected_events[i] == event) << i << ": " << event;
    }
}

TEST_F(Keymap, HandlesTransparentFnLayerKeysAsHypr) {
    event_to_make_t events[] = {
        {CL_FN_SWITCH, 1},
        {KC_A, 1},
        {KC_A, 0},
        {CL_FN_SWITCH, 0}
    };
    for (int i = 0; i < 4; ++i) {
        auto matrix_event = MakeMatrixEvent(events[i]);
        ASSERT_EQ(0, Keybi_Keymap_EventHandler(matrix_event));
    }
    keybi_keyboard_event_t expected_events[] = {
        {KC_LCTRL, 1},
        {KC_LSHIFT, 1},
        {KC_LALT, 1},
        {KC_LGUI, 1},
        {KC_A, 1},
        {KC_A, 0},
        {KC_LCTRL, 0},
        {KC_LSHIFT, 0},
        {KC_LALT, 0},
        {KC_LGUI, 0},
    };
    for (unsigned i = 0; i < 10; ++i) {
        keybi_keyboard_event_t event = GetQueuedElement(i);
        ASSERT_TRUE(expected_events[i] == event) << i << ": " << event;
    }
    ASSERT_EQ(L_BASE, keybi_keyboard_layer);
}
