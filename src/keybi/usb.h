#pragma once

#include "stm32f10x.h"

#define KEYBI_KEYBOARD_SIZ_HID_DESC           (0x09)
#define KEYBI_KEYBOARD_SIZ_REPORT_DESC          (63)
#define KEYBI_KEYBOARD_KEYBOARD_OFF_HID_DESC   (121)

uint8_t* Keybi_Keyboard_GetReportDescriptor(uint16_t Length);

uint8_t* Keybi_Keyboard_GetHIDDescriptor(uint16_t Length);

void Keybi_Keyboard_SendReportCompleted(void);