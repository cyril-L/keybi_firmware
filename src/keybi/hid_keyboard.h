#pragma once

#include "stm32f10x.h"

#define KEYBI_KEYBOARD_OFF_HID_DESC (118)
#define KEYBI_KEYBOARD_SIZ_HID_DESC (0x09)
#define KEYBI_KEYBOARD_SIZ_REPORT_DESC (63)
#define KEYBI_KEYBOARD_SIZ_REPORT (8)

extern const uint8_t Keybi_Keyboard_ReportDescriptor[KEYBI_KEYBOARD_SIZ_REPORT_DESC];

void Keybi_Keyboard_SendReport(uint8_t * report);

uint8_t* Keybi_Keyboard_GetReportDescriptor(uint16_t Length);

uint8_t* Keybi_Keyboard_GetHIDDescriptor(uint16_t Length);

void Keybi_Keyboard_SendReportCompleted(void);
