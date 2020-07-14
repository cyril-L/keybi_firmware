#pragma once

#include "stm32f10x.h"

#define KEYBI_MATRIX_ROWS (5)
#define KEYBI_MATRIX_COLS (14)

typedef struct {
    uint8_t col;
    uint8_t row;
    uint8_t pressed;
    uint16_t time;
} keybi_keyboard_matrix_event_t;

void Keybi_Matrix_Init(void);
void Keybi_Matrix_Scan(int (*callback)(keybi_keyboard_matrix_event_t));
