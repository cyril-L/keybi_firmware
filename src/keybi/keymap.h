#pragma once

#include "stm32f10x.h"
#include "keybi/drivers/matrix.h"
#include "keybi/hid_keyboard.h"
#include "keybi/qmk/keycode.h"

typedef enum {
  L_BASE = 0,
  L_FN,
  L_CODE,
  L_MOUSE
} layer_id_t;

enum custom_keycodes {
  CL_FN_SWITCH = 0x100, // TODO safe range above uint8_t?
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
  CL_SHIFT,
  CL_ALTGR,
  CL_SAFE_RANGE
};

int Keybi_Keymap_EventHandler(keybi_keyboard_matrix_event_t event);
void Keyi_Keymap_SetLayer(layer_id_t new_layer);
void Keybi_Keymap_Loop(void);

// TODO used from outside for experiments, to refactor
extern keybi_keyboard_event_queue_t keybi_keymap_events;
extern uint8_t keybi_keyboard_layer;
extern int keybi_mouse_is_scrolling;
extern uint8_t keybi_mouse_buttons;
