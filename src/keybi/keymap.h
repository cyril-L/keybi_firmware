#pragma once

#include "stm32f10x.h"
#include "keybi/drivers/matrix.h"
#include "keybi/hid_keyboard.h"

int Keybi_Keymap_EventHandler(keybi_keyboard_matrix_event_t event);

extern keybi_keyboard_event_queue_t keybi_keymap_events;
