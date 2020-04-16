#pragma once

#include "stm32f10x.h"
#include "delays.h"

#define BALL_SS_PERIPH     RCC_APB2Periph_GPIOC
#define BALL_SS_PIN_PORT   GPIOC
#define BALL_SS_PIN        GPIO_Pin_6

#define BALL_MT_PERIPH     RCC_APB2Periph_GPIOC
#define BALL_MT_PIN_PORT   GPIOC
#define BALL_MT_PIN        GPIO_Pin_7

void MyTrackballSetup(void);
void MyTrackballRead(void);
