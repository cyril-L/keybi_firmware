#include "keybi/drivers/time.h"
#include "stm32f10x_systick.h"

uint32_t keybi_systick_counter = 0;

void Keybi_DelayUs(uint32_t duration_us) {
    uint32_t remaining_ticks = duration_us * 72;
    uint32_t prev = SysTick_GetCounter();

    while (1) {
        uint32_t curr = SysTick_GetCounter();
        uint32_t elapsed;
        if (curr > prev) {
            elapsed = 719999 + prev - curr;
        } else {
            elapsed = prev - curr;
        }
        if (elapsed >= remaining_ticks) {
            break;
        }
        remaining_ticks -= elapsed;
        prev = curr;
    }
}

uint32_t Keybi_TimeMs() {
    return keybi_systick_counter * 10 + SysTick_GetCounter() / 72000;
}
