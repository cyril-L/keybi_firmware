#pragma once

#include "stm32f10x.h"

#define MAIN_LOOP_FLAG_PERIPH    RCC_APB2Periph_GPIOA
#define MAIN_LOOP_FLAG_PIN_PORT  GPIOA
#define MAIN_LOOP_FLAG_PIN       GPIO_Pin_6

#define USER_SWITCH_PERIPH     RCC_APB2Periph_GPIOC
#define USER_SWITCH_PIN_PORT   GPIOC
#define USER_SWITCH_PIN        GPIO_Pin_13

void MyKeyboardSetup(void);
void MyKeyboardLoop(void);
