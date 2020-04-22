#pragma once

#include "stm32f10x.h"

// PMW3360 motion sensor driver, currently using breakout board from
// https://www.tindie.com/products/jkicklighter/pmw3360-motion-sensor/
//
// MI -> PB14 (SPI2 MISO)
// MO -> PB15 (SPI2_MOSI)
// SS -> PC6  (Slave Select)
// SC -> PB13 (SPI2 SCK)
// MT -> PC7  (Motion interrupt)
// RS    NC   (Reset)

// SPI Slave Select pin
#define KEYBI_PMW3360_SS_PERIPH     RCC_APB2Periph_GPIOC
#define KEYBI_PMW3360_SS_PIN_PORT   GPIOC
#define KEYBI_PMW3360_SS_PIN        GPIO_Pin_6

// Motion interrupt pin
#define KEYBI_PMW3360_MT_PERIPH     RCC_APB2Periph_GPIOC
#define KEYBI_PMW3360_MT_PIN_PORT   GPIOC
#define KEYBI_PMW3360_MT_PIN        GPIO_Pin_7

typedef struct keybi_pmw3360_motion_t {
    int8_t dx;
    int8_t dy;
} keybi_pmw3360_motion_t;

int Keybi_Pmw3360_Init(void);
void Keybi_Pmw3360_Read(keybi_pmw3360_motion_t * motion);
