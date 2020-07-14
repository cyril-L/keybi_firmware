#include "keybi/keymap.h"
#include "keybi/qmk/keycode.h"
#include "keybi/drivers/matrix.h"

// TODO progmem?

#define SAFE_RANGE (0x100)

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
  CL_SAFE_RANGE
};

typedef enum {
  L_BASE = 0,
  L_NF_G1_L1,
  L_NF_G1_L2,
  L_NF_G2_L1,
  L_FN,
  L_CODE,
  L_RCMD,
  L_MOUSE
} layer_id_t;

#define LAYOUT( \
    e00, e01, e02, e03, e04, e05, e99, e06, e07, e08, e09, e10, e11, e12, e13, \
    d00, d01, d02, d03, d04, d05, d06, d07, d08, d09, d10, d11, d12, d13, \
    c00, c01, c02, c03, c04, c05,      c07, c08, c09, c10, c11, c12, c13, \
    b00, b01, b02, b03, b04, b05, b06, b07, b08, b09, b10, b11, b12, b13, \
    a00,      a02, a03, a04, a05, a06, a07, a08, a09, a10,           a13  \
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
    KC_GRV,        KC_1,   KC_2,    KC_3,    KC_4,   KC_5,    KC_NO, KC_6,    KC_7,     KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, \
    KC_TAB,        KC_Q,   KC_W,    KC_E,    KC_R,   KC_T,    KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NUHS, \
    CL_CODE_SWITCH,        KC_A,   KC_S,    KC_D,    KC_F,   KC_G,             KC_H,     KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
	KC_LSHIFT,  KC_Z,   KC_X,    KC_C,    KC_V,   KC_B,    KC_UP,   KC_NUBS,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSHIFT, \
    KC_LCTL,               KC_LALT, CL_CMD_CTRL, KC_SPC, KC_LEFT, KC_DOWN, KC_RIGHT, CL_FN_SWITCH,  KC_RALT, KC_RGUI,                   KC_RCTL \
  )
};

static keybi_keyboard_event_t events[8];

keybi_keyboard_event_queue_t keybi_keymap_events = {
    .events = events,
    .head = 0,
    .size = 0,
    .capacity = 8
};

int Keybi_Keymap_EventHandler(keybi_keyboard_matrix_event_t matrix_event) {

	keybi_keyboard_event_t key_event = {
			.keycode = keymaps[0][matrix_event.row][matrix_event.col],
			.pressed = matrix_event.pressed
	};
	return Keybi_Keyboard_QueueEvent(&keybi_keymap_events, key_event);
}
