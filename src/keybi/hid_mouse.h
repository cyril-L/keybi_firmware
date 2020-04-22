#pragma once

#include "stm32f10x.h"

#define KEYBI_MOUSE_OFF_HID_DESC (143)
#define KEYBI_MOUSE_SIZ_HID_DESC (0x09)
#define KEYBI_MOUSE_SIZ_REPORT_DESC (118)
#define KEYBI_MOUSE_SIZ_REPORT (5)

extern const uint8_t Keybi_Mouse_ReportDescriptor[KEYBI_MOUSE_SIZ_REPORT_DESC];

void Keybi_Mouse_SendReport(uint8_t * report);
void Keybi_Mouse_SendReportCompleted(void);

uint8_t* Keybi_Mouse_GetReportDescriptor(uint16_t Length);

uint8_t* Keybi_Mouse_GetHIDDescriptor(uint16_t Length);
