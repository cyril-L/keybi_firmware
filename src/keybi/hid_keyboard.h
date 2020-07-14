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

typedef struct {
    uint8_t keycode;
    uint8_t pressed;
} keybi_keyboard_event_t;

typedef struct {
	keybi_keyboard_event_t* events;
	uint32_t head;
	uint32_t size;
	uint32_t capacity;
} keybi_keyboard_event_queue_t;

int Keybi_Keyboard_QueueEvent(keybi_keyboard_event_queue_t * queue, keybi_keyboard_event_t event);

int Keybi_Keyboard_QueueToReport(keybi_keyboard_event_queue_t * queue, uint8_t * report);
