#include "keybi/keymap.h"
#include "keybi/qmk/keycode.h"
#include "keybi/drivers/matrix.h"

// Quick and dirty way to get a keymap behaving like a previous experiment
// using QMK. Needs to be cleaned up.

// TODO progmem?

#define SAFE_RANGE (0x100)
#define ____ KC_TRNS
#define XXXX KC_NO

enum custom_keycodes {
  CL_FN_SWITCH = SAFE_RANGE,
  CL_CODE_SWITCH,
  CL_TOGGLE_DIGITS,
  CL_SQUOTS,
  CL_DQUOTS,
  CL_PARENS,
  CL_BRCKTS,
  CL_BRACES,
  CL_CMD_CTRL,
  CL_TOGGLE_CMD_CTRL,
  CL_MOUSE_OUT,
  CL_MOUSE_TOGGLE_SCROLL,
  CL_SAFE_RANGE
};

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
	KC_LSHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_UP, KC_NUBS, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSHIFT, \
    KC_LCTL, KC_LALT, CL_CMD_CTRL, KC_SPC, KC_LEFT, KC_DOWN, KC_RIGHT, CL_FN_SWITCH, KC_RALT, KC_RGUI, KC_RCTL \
  ),
  [L_FN] = LAYOUT( /* Fn, mostly used for navigation and window management */
    KC_PWR, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DELETE, \
	____, ____, ____, ____, ____, ____, KC_HOME, KC_PGDN, KC_PGUP, KC_END, ____, ____, ____, ____, \
	____, ____, ____, ____, ____, ____, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, ____, ____, ____, \
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
    OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, \
    OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, \
    OOOO, OOOO, KC_BTN2, KC_BTN3, KC_BTN1, OOOO, OOOO, KC_BTN1, KC_BTN3, KC_BTN2, OOOO, OOOO, OOOO, \
    OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, OOOO, \
    OOOO, OOOO, OOOO, CL_MOUSE_TOGGLE_SCROLL, OOOO, OOOO, OOOO, CL_MOUSE_TOGGLE_SCROLL, OOOO, OOOO, OOOO \
  ),
};

static keybi_keyboard_event_t events_data[16];

keybi_keyboard_event_queue_t keybi_keymap_events = {
    .events = events_data,
    .head = 0,
    .size = 0,
    .capacity = 16
};

int keybi_keyboard_layer = L_BASE;
static bool is_cmd_ctrl = 1;
static int key_pressed_on_code_layer = 0;

int keybi_mouse_is_scrolling = 0;
uint8_t keybi_mouse_buttons = 0;

int Keybi_Keymap_EventHandler(keybi_keyboard_matrix_event_t matrix_event) {

	// Here keycode is 16 bit to hold special values
	uint16_t keycode = keymaps[keybi_keyboard_layer][matrix_event.row][matrix_event.col];

	if (keybi_keyboard_layer == L_CODE && matrix_event.pressed) {
		key_pressed_on_code_layer++;
	}

	while (1) {
		if (keycode == CL_FN_SWITCH) {
			if (matrix_event.pressed) {
				keybi_keyboard_layer = L_FN;
			} else {
				keybi_keyboard_layer = L_BASE;
			}
			return 0;
		}
		if (keycode == CL_CODE_SWITCH) {
			if (matrix_event.pressed) {
				keybi_keyboard_layer = L_CODE;
				key_pressed_on_code_layer = 0;
			} else {
				keybi_keyboard_layer = L_BASE;
				// if no event other than layer
				if (!key_pressed_on_code_layer) {
					keybi_keyboard_event_t events[2] = {{KC_ESC, 1}, {KC_ESC, 0}};
					return Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 2);
				}
			}
			return 0;
		} else if (keycode == OOOO) {
			if (matrix_event.pressed) {
				keybi_keyboard_layer = L_BASE;
			}
			return 0;
		}
		else if (keycode == CL_CMD_CTRL) {
			if (is_cmd_ctrl) {
				keycode = KC_LCTL;
			} else {
				keycode = KC_LGUI;
			}
		} else if (keycode == CL_TOGGLE_CMD_CTRL) {
			if (matrix_event.pressed) {
				is_cmd_ctrl = !is_cmd_ctrl;
			}
			return 0;
		} else if (keycode == ____) {
			keycode = keymaps[L_BASE][matrix_event.row][matrix_event.col];
			continue; // handle this keycode if special
		} else if (keycode == CL_PARENS && matrix_event.pressed) {
			keybi_keyboard_event_t events[6] = {
					{KC_5, 1}, {KC_5, 0},
					{KC_6, 1}, {KC_6, 0},
					{KC_LEFT, 1}, {KC_LEFT, 0},
			};
			return Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 6);
		} else if (keycode == CL_BRCKTS && matrix_event.pressed) {
			keybi_keyboard_event_t events[8] = {
					{KC_RALT, 1},
					{KC_5, 1}, {KC_5, 0},
					{KC_6, 1}, {KC_6, 0},
					{KC_RALT, 0},
					{KC_LEFT, 1}, {KC_LEFT, 0},
			};
			return Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 8);
		} else if (keycode == CL_BRACES && matrix_event.pressed) {
			keybi_keyboard_event_t events[8] = {
					{KC_RALT, 1},
					{KC_T, 1}, {KC_T, 0},
					{KC_Y, 1}, {KC_Y, 0},
					{KC_RALT, 0},
					{KC_LEFT, 1}, {KC_LEFT, 0},
			};
			return Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 8);
		} else if (keycode == CL_SQUOTS && matrix_event.pressed) {
			keybi_keyboard_event_t events[6] = {
					{KC_MINS, 1}, {KC_MINS, 0},
					{KC_MINS, 1}, {KC_MINS, 0},
					{KC_LEFT, 1}, {KC_LEFT, 0},
			};
			return Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 6);
		} else if (keycode == CL_DQUOTS && matrix_event.pressed) {
			keybi_keyboard_event_t events[8] = {
					{KC_LSHIFT, 1},
					{KC_MINS, 1}, {KC_MINS, 0},
					{KC_MINS, 1}, {KC_MINS, 0},
					{KC_LSHIFT, 0},
					{KC_LEFT, 1}, {KC_LEFT, 0}
			};
			return Keybi_Keyboard_QueueEvents(&keybi_keymap_events, events, 8);
		} else if (keycode == CL_MOUSE_TOGGLE_SCROLL) {
			if (matrix_event.pressed) {
				keybi_mouse_is_scrolling = !keybi_mouse_is_scrolling;
			}
			return 0;
		} else if (keycode == KC_BTN1) {
			if (matrix_event.pressed) {
				keybi_mouse_buttons |= (1 << 0);
			} else {
				keybi_mouse_buttons &= ~(1 << 0);
			}
			return 0;
		} else if (keycode == KC_BTN2) {
			if (matrix_event.pressed) {
				keybi_mouse_buttons |= (1 << 1);
			} else {
				keybi_mouse_buttons &= ~(1 << 1);
			}
			return 0;
		} else if (keycode == KC_BTN3) {
			if (matrix_event.pressed) {
				keybi_mouse_buttons |= (1 << 2);
			} else {
				keybi_mouse_buttons &= ~(1 << 2);
			}
			return 0;
		}
		// no need to handle this keycode further
		break;
	};
	// TODO check if keycode is not special here
	keybi_keyboard_event_t key_event = {
			.keycode = keycode,
			.pressed = matrix_event.pressed
	};
	return Keybi_Keyboard_QueueEvent(&keybi_keymap_events, key_event);
}
