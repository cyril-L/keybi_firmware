#include "keybi/keymap.h"
#include "keybi/drivers/matrix.h"
#include "keybi/drivers/time.h"
#include "keybi/drivers/ws2812.h"

// Quick and dirty way to get a keymap behaving like a previous experiment
// using QMK. Needs to be cleaned up.

// TODO progmem?

#define SAFE_RANGE (0x100)
#define ____ KC_TRNS
#define XXXX KC_NO

#define OOOO CL_MOUSE_OUT

#define LAYOUT( \
e00, e01, e02, e03, e04, e05, e99,   e06, e07, e08, e09, e10, e11, e12, e13, \
d00, d01, d02, d03, d04, d05,        d06, d07, d08, d09, d10, d11, d12, d13, \
c00, c01, c02, c03, c04, c05,             c07, c08, c09, c10, c11, c12, c13, \
b00, b01, b02, b03, b04, b05,   b06,      b07, b08, b09, b10, b11, b12, b13, \
a00,      a02, a03, a04,   a05, a06, a07, a08, a09, a10,           a13  \
) \
{ \
    { e00, e01, e02, e03, e04, e05, e99, e07, e08, e09, e10, e11, e12, e13}, \
    { d00, d01, d02, d03, d04, d05, e06, d07, d08, d09, d10, d11, d12, d13}, \
    { c00, c01, c02, c03, c04, c05, d06, c07, c08, c09, c10, c11, c12, c13}, \
    { b00, b01, b02, b03, b04, b05, b06, b07, b08, b09, b10, b11, b12, b13}, \
    { a00, KC_NO, a02, a03, a04, a05, a06, a07, a08, a09, a10, KC_NO, KC_NO, a13} \
}

const uint16_t keymaps[][KEYBI_MATRIX_ROWS][KEYBI_MATRIX_COLS] = {

  /* Layer 0: Default Layer
   * ,---------------------------.      ,-----------------------------------.
   * | ` |  1|  2|  3|  4|  5|   |      |  6|  7|  8|  9|  0|  -|  =|   Bksp|
   * |---------------------------'      `-----------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|          |  Y|  U|  I|  O|  P|  [|  ]|     |
   * |--------------------------.         `---------------------------------|
   * |Esc   |  A|  S|  D|  F|  G|          |  H|  J|  K|  L|  ;|  '|   Enter|
   * |----------------------------. ,---. ,`--------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B| | ↑ | |   |  N|  M|  ,|  .|  /|    Shift|
   * |----------------------------------------------------------------------|
   * |Ctrl |     |Alt |Cmd |Spc | ← | ↓ | → |  Fn| Alt| Cmd|          | Ctrl|
   * `-----'     `-----------------------------------------'          `-----'
   */
  [L_BASE] = LAYOUT( /* Base */
    KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, XXXX, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, \
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_NUHS, \
    CL_CODE_SWITCH, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, \
    CL_SHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_UP, KC_NUBS, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, CL_SHIFT, \
    KC_LCTL, KC_LALT, CL_CMD_CTRL, KC_SPC, KC_LEFT, KC_DOWN, KC_RIGHT, CL_FN_SWITCH, CL_ALTGR, KC_RGUI, KC_RCTL \
    ),
  [L_FN] = LAYOUT( /* Fn, mostly used for navigation and window management */
  KC_PWR, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DELETE, \
  ____, ____, ____, ____, ____, ____, KC_HOME, KC_PGDN, KC_PGUP, KC_END, ____, ____, ____, ____, \
  ____, ____, ____, ____, ____, ____, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, ____, ____, KC_INS, \
  XXXX, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, XXXX, \
  XXXX, XXXX, CL_TOGGLE_CMD_CTRL, XXXX, ____, ____, ____, ____, XXXX, XXXX, XXXX \
  ),
  [L_CODE] = LAYOUT( /* Mostly used for code */
  ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, \
  ____, ____, ____, ____, ____, ____, ____, CL_SQUOTS, CL_DQUOTS, ____, ____, ____, ____, ____, \
  ____, ____, ____, ____, ____, ____, ____, CL_PARENS, CL_BRCKTS, CL_BRACES, ____, ____, ____, \
  XXXX, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, XXXX, \
  XXXX, XXXX, XXXX, ____, ____, ____, ____, ____, XXXX, XXXX, XXXX \
  ),
  [L_MOUSE] = LAYOUT( /* Mouse mode */
  ____, ____, ____, ____, ____, ____, CL_MOUSE_OUT, ____, ____, ____, ____, ____, ____, ____, ____, \
  ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, ____, \
  CL_MOUSE_OUT, ____, KC_BTN2, KC_BTN3, KC_BTN1, ____, ____, KC_BTN1, KC_BTN3, KC_BTN2, ____, ____, ____, \
  ____, ____, ____, ____, ____, CL_MOUSE_OUT, KC_BTN2, CL_MOUSE_OUT, ____, ____, ____, ____, ____, ____, \
  ____, ____, ____, CL_MOUSE_TOGGLE_SCROLL, KC_BTN1, KC_BTN3, KC_BTN1, CL_MOUSE_TOGGLE_SCROLL, ____, ____, ____ \
  ),
};

static keybi_keyboard_event_t events_data[16];

keybi_keyboard_event_queue_t keybi_keymap_events = {
    .events = events_data,
    .head = 0,
    .size = 0,
    .capacity = 16
};

uint8_t keybi_keyboard_layer = L_BASE;

int keybi_mouse_is_scrolling = 0;
uint8_t keybi_mouse_buttons = 0;

uint32_t unset_mouse_layer_at = 0;

enum shift_state_t {PRESSED, ONE_SHOT, LOCKED, RELEASED};

static int shift_state = RELEASED;
static int altgr_state = RELEASED;

void Keyi_Keymap_SetLayer(layer_id_t new_layer) {
    // Called each time the trackball moves
    if (new_layer == L_MOUSE) {
        // Set mouse move mode when entering mouse layer
        if (keybi_keyboard_layer != L_MOUSE) {
            keybi_mouse_is_scrolling = 0;
        }
        unset_mouse_layer_at = Keybi_TimeMs() + 3000;
    }
    keybi_keyboard_layer = new_layer;
}

void Keybi_Keymap_Loop() {
    if (keybi_keyboard_layer == L_MOUSE) {
        if (Keybi_TimeMs() > unset_mouse_layer_at && !keybi_mouse_is_scrolling) {
            keybi_keyboard_layer = L_BASE;
        }
    }
    if (keybi_keyboard_layer == L_MOUSE) {
        Keybi_Ws2812_Set(2, 0x000a00);
    } else {
        Keybi_Ws2812_Set(2, 0x000000);
    }
    if (shift_state == PRESSED) {
        Keybi_Ws2812_Set(1, 0x000007);
    } else if (shift_state == ONE_SHOT) {
        Keybi_Ws2812_Set(1, 0x030003);
    } else if (shift_state == LOCKED) {
        Keybi_Ws2812_Set(1, 0x070000);
    } else {
        Keybi_Ws2812_Set(1, 0x000000);
    }
    if (altgr_state != RELEASED) {
        Keybi_Ws2812_Set(0, 0x070000);
    } else {
        Keybi_Ws2812_Set(0, 0x000000);
    }
}

// Keeps track of which layer has been used to register events
uint8_t pressed_on_layer[KEYBI_MATRIX_ROWS][KEYBI_MATRIX_COLS] = {0};

static uint16_t handle_layers(keybi_keyboard_matrix_event_t matrix_event) {
    uint16_t keycode;
    if (matrix_event.pressed) {
        keycode = keymaps[keybi_keyboard_layer][matrix_event.row][matrix_event.col];
        pressed_on_layer[matrix_event.row][matrix_event.col] = keybi_keyboard_layer;
    } else {
        uint8_t pressed_on = pressed_on_layer[matrix_event.row][matrix_event.col];
        keycode = keymaps[pressed_on][matrix_event.row][matrix_event.col];
    }
    return keycode;
}

enum keycode_handler_return_code_t { CONTINUE, BREAK, FAILED };

static int handle_fn_layer(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    if (*keycode == CL_FN_SWITCH) {
        if (matrix_event.pressed) {
            keybi_keyboard_layer = L_FN;
        } else {
            keybi_keyboard_layer = L_BASE;
        }
        return BREAK;
    }

    if (keybi_keyboard_layer != L_FN) {
        return CONTINUE;
    }

    if (*keycode == ____) {
        if (!matrix_event.pressed) {
            return BREAK;
        }
        uint16_t base_keycode = keymaps[L_BASE][matrix_event.row][matrix_event.col];
        if (!IS_KEY(base_keycode)) {
            return BREAK;
        }
        // TODO only handle taps for now
        keybi_keyboard_event_t events[10] = {
            {KC_LCTRL, 1},
            {KC_LSHIFT, 1},
            {KC_LALT, 1},
            {KC_LGUI, 1},
            {base_keycode, 1},
            {base_keycode, 0},
            {KC_LCTRL, 0},
            {KC_LSHIFT, 0},
            {KC_LALT, 0},
            {KC_LGUI, 0}
        };
        if (Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 10) == 0) {
            return BREAK;
        } else {
            return FAILED;
        }
    }

    return CONTINUE;
}

static int handle_code_layer(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    static int key_pressed_on_code_layer = 0;

    if (*keycode == CL_CODE_SWITCH) {
        if (matrix_event.pressed) {
            keybi_keyboard_layer = L_CODE;
            key_pressed_on_code_layer = 0;
            return BREAK;
        } else {
            keybi_keyboard_layer = L_BASE;
            // if no event other than layer
            if (!key_pressed_on_code_layer) {
                keybi_keyboard_event_t events[2] = {{KC_ESC, 1}, {KC_ESC, 0}};
                if (Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 2) == 0) {
                    return BREAK;
                } else {
                    return FAILED;
                }
            }
            return BREAK;
        }
    }

    if (keybi_keyboard_layer != L_CODE) {
        return CONTINUE;
    }

    if (matrix_event.pressed) {
        key_pressed_on_code_layer = 1;
    }

    return CONTINUE;
}

static int handle_mouse_layer(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {
    if (*keycode == CL_MOUSE_OUT && matrix_event.pressed) {
        keybi_keyboard_layer = L_BASE;
        return BREAK;
    }

    if (keybi_keyboard_layer != L_MOUSE) {
        return CONTINUE;
    }

    if (*keycode == ____) {
        if (matrix_event.pressed) {
            keybi_keyboard_layer = L_BASE;
        }
        return BREAK;
    }

    return CONTINUE;
}

static int handle_cmd_ctrl(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    static bool is_cmd_ctrl = 1;

    if (*keycode == CL_TOGGLE_CMD_CTRL) {
        if (matrix_event.pressed) {
            is_cmd_ctrl = !is_cmd_ctrl;
        }
        return BREAK;
    }
    if (*keycode == CL_CMD_CTRL) {
        if (is_cmd_ctrl) {
            *keycode = KC_LCTL;
        } else {
            *keycode = KC_LGUI;
        }
    }
    return CONTINUE;
}

static int handle_macros(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    static keybi_keyboard_event_t parens_events[6] = {
        {KC_5, 1}, {KC_5, 0},
        {KC_6, 1}, {KC_6, 0},
        {KC_LEFT, 1}, {KC_LEFT, 0},
    };

    static keybi_keyboard_event_t brackets_events[8] = {
        {KC_RALT, 1},
        {KC_5, 1}, {KC_5, 0},
        {KC_6, 1}, {KC_6, 0},
        {KC_RALT, 0},
        {KC_LEFT, 1}, {KC_LEFT, 0},
    };

    static keybi_keyboard_event_t braces_events[8] = {
        {KC_RALT, 1},
        {KC_T, 1}, {KC_T, 0},
        {KC_Y, 1}, {KC_Y, 0},
        {KC_RALT, 0},
        {KC_LEFT, 1}, {KC_LEFT, 0}
    };

    static keybi_keyboard_event_t single_quotes_events[6] = {
        {KC_MINS, 1}, {KC_MINS, 0},
        {KC_MINS, 1}, {KC_MINS, 0},
        {KC_LEFT, 1}, {KC_LEFT, 0}
    };

    static keybi_keyboard_event_t double_quotes_events[8] = {
        {KC_LSHIFT, 1},
        {KC_MINS, 1}, {KC_MINS, 0},
        {KC_MINS, 1}, {KC_MINS, 0},
        {KC_LSHIFT, 0},
        {KC_LEFT, 1}, {KC_LEFT, 0}
    };

    typedef struct macro_t {
        uint16_t trigger;
        uint8_t size;
        keybi_keyboard_event_t * events;
    } macro_t;

    static macro_t macros[5] = {
        {CL_PARENS, 6, parens_events},
        {CL_BRCKTS, 8, brackets_events},
        {CL_BRACES, 8, braces_events},
        {CL_SQUOTS, 6, single_quotes_events},
        {CL_DQUOTS, 8, double_quotes_events},
    };

    for (int i = 0; i < 5; ++i) {
        macro_t macro = macros[i];
        if (*keycode == macro.trigger) {
            if (!matrix_event.pressed) {
                return BREAK;
            }
            if (Keybi_Keyboard_QueueEvents(&keybi_keymap_events, macro.events, macro.size) == 0) {
                return BREAK;
            } else {
                return FAILED;
            }
        }
    }

    return CONTINUE;
}

static int handle_mouse_buttons(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    if (*keycode == CL_MOUSE_TOGGLE_SCROLL) {
        if (matrix_event.pressed) {
            keybi_mouse_is_scrolling = !keybi_mouse_is_scrolling;
        }
        return BREAK;
    }

    if (matrix_event.pressed && (*keycode == KC_BTN1 || *keycode == KC_BTN2 || *keycode == KC_BTN3)) {
        unset_mouse_layer_at = Keybi_TimeMs() + 300;
    }

    if (*keycode == KC_BTN1) {
        if (matrix_event.pressed) {
            keybi_mouse_buttons |= (1 << 0);
        } else {
            keybi_mouse_buttons &= ~(1 << 0);
        }
        return BREAK;
    } else if (*keycode == KC_BTN2) {
        if (matrix_event.pressed) {
            keybi_mouse_buttons |= (1 << 1);
        } else {
            keybi_mouse_buttons &= ~(1 << 1);
        }
        return BREAK;
    } else if (*keycode == KC_BTN3) {
        if (matrix_event.pressed) {
            keybi_mouse_buttons |= (1 << 2);
        } else {
            keybi_mouse_buttons &= ~(1 << 2);
        }
        return BREAK;
    }

    return CONTINUE;
}

static int handle_caps(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    static int key_pressed_while_shift_pressed = 0;

    if (*keycode == CL_SHIFT && matrix_event.pressed) {
        if (shift_state == RELEASED) {
            keybi_keyboard_event_t event = {KC_LSHIFT, 1};
            if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
                key_pressed_while_shift_pressed = 0;
                shift_state = PRESSED;
                return BREAK;
            } else {
                return FAILED;
            }
        } else if (shift_state == ONE_SHOT) {
            shift_state = LOCKED;
            return BREAK;
        } else if (shift_state == LOCKED) {
            keybi_keyboard_event_t event = {KC_LSHIFT, 0};
            if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
                shift_state = RELEASED;
                return BREAK;
            } else {
                return FAILED;
            }
        }
    }

    if (*keycode == CL_SHIFT && !matrix_event.pressed) {
        if (shift_state == PRESSED) {
            if (key_pressed_while_shift_pressed) {
                keybi_keyboard_event_t event = {KC_LSHIFT, 0};
                if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
                    shift_state = RELEASED;
                    return BREAK;
                } else {
                    return FAILED;
                }
            } else {
                shift_state = ONE_SHOT;
                return BREAK;
            }
        }
        return BREAK;
    }

    if (*keycode != CL_SHIFT && shift_state == PRESSED && matrix_event.pressed) {
        key_pressed_while_shift_pressed = 1;
        return CONTINUE;
    }

    if (*keycode != CL_SHIFT && shift_state == ONE_SHOT && matrix_event.pressed) {
        return CONTINUE;
    }

    if (*keycode != CL_SHIFT && shift_state == ONE_SHOT && !matrix_event.pressed) {
        keybi_keyboard_event_t event = {KC_LSHIFT, 0};
        if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
            shift_state = RELEASED;
            return CONTINUE;
        } else {
            return FAILED;
        }
        return CONTINUE;
    }

    return CONTINUE;
}

static int handle_altgr(keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) {

    static int key_pressed_while_alt_pressed = 0;

    if (*keycode == CL_ALTGR && matrix_event.pressed) {
        if (altgr_state == RELEASED) {
            keybi_keyboard_event_t event = {KC_RALT, 1};
            if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
                key_pressed_while_alt_pressed = 0;
                altgr_state = PRESSED;
                return BREAK;
            } else {
                return FAILED;
            }
        } else if (altgr_state == ONE_SHOT) {
            keybi_keyboard_event_t event = {KC_RALT, 0};
            if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
                altgr_state = RELEASED;
                return BREAK;
            } else {
                return FAILED;
            }
        }
    }

    if (*keycode == CL_ALTGR && !matrix_event.pressed) {
        if (altgr_state == PRESSED) {
            if (key_pressed_while_alt_pressed) {
                keybi_keyboard_event_t event = {KC_RALT, 0};
                if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
                    altgr_state = RELEASED;
                    return BREAK;
                } else {
                    return FAILED;
                }
            } else {
                altgr_state = ONE_SHOT;
                return BREAK;
            }
        }
        return BREAK;
    }

    if (*keycode != CL_ALTGR && altgr_state == PRESSED && matrix_event.pressed) {
        key_pressed_while_alt_pressed = 1;
        return CONTINUE;
    }

    if (*keycode != CL_ALTGR && altgr_state == ONE_SHOT && matrix_event.pressed) {
        return CONTINUE;
    }

    if (*keycode != CL_ALTGR && altgr_state == ONE_SHOT && !matrix_event.pressed) {
        keybi_keyboard_event_t event = {KC_RALT, 0};
        if (Keybi_Keyboard_QueueEvent(&keybi_keymap_events, event) == 0) {
            altgr_state = RELEASED;
            return CONTINUE;
        } else {
            return FAILED;
        }
        return CONTINUE;
    }

    return CONTINUE;
}

int Keybi_Keymap_EventHandler(keybi_keyboard_matrix_event_t matrix_event) {

    // Here keycode is 16 bit to hold special values
    uint16_t keycode = handle_layers(matrix_event);

    int (*keycode_handlers[]) (keybi_keyboard_matrix_event_t matrix_event, uint16_t * keycode) = {
        &handle_fn_layer,
        &handle_code_layer,
        &handle_mouse_layer,
        &handle_cmd_ctrl,
        &handle_macros,
        &handle_mouse_buttons,
        &handle_caps,
        &handle_altgr,
    };

    for (unsigned i = 0; i < sizeof(keycode_handlers) / sizeof(keycode_handlers[0]); ++i) {
        int ret = keycode_handlers[i](matrix_event, &keycode);
        if (ret == BREAK) { return 0; }
        if (ret == FAILED) { return -1; }
    }

    // TODO check if keycode is not special here
    keybi_keyboard_event_t key_event = {
        .keycode = keycode,
        .pressed = matrix_event.pressed
    };
    return Keybi_Keyboard_QueueEvent(&keybi_keymap_events, key_event);
}
