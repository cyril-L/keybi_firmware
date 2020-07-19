#pragma once

#include "stm32f10x.h"

void Keybi_DelayUs(uint32_t duration_us);

// Incremented on each systick interrupt (every 10 ms)
extern uint32_t keybi_systick_counter;

uint32_t Keybi_TimeMs(void);
