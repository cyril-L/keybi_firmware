#pragma once

#include "stm32f10x.h"

#define NUCLEO_BTN_PERIPH     RCC_APB2Periph_GPIOC
#define NUCLEO_BTN_PIN_PORT   GPIOC
#define NUCLEO_BTN_PIN        GPIO_Pin_13

void Keybi_Keyboard_Init(void);
void Keybi_Keyboard_MainLoop(void);