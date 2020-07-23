#pragma once

#include "stm32f10x.h"

// LEDs data pin
#define KEYBI_WS2812_PERIPH     RCC_APB2Periph_GPIOD
#define KEYBI_WS2812_PIN_PORT   GPIOD
#define KEYBI_WS2812_PIN        GPIO_Pin_2

#define KEYBI_WS2812_COUNT      (4)

void Keybi_Ws2812_Init(void);
void Keybi_Ws2812_Set(unsigned led_id, uint32_t color);
