#include "keybi/drivers/ws2812.h"
#include "keybi/drivers/time.h"

extern void WS2812S_send_pixel(uint32_t pixel, uint32_t gpio_pin_x, uint32_t * gpiox_bsrr);

static uint32_t led_colors[KEYBI_WS2812_COUNT] = {0};

void Keybi_Ws2812_Init() {

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(KEYBI_WS2812_PERIPH, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEYBI_WS2812_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEYBI_WS2812_PIN_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEYBI_WS2812_PIN_PORT, KEYBI_WS2812_PIN);
}

void Keybi_Ws2812_Set(unsigned led_id, uint32_t color)
{
    if (led_id >= KEYBI_WS2812_COUNT) { return; }
    if (led_colors[led_id] == color) { return; }
    led_colors[led_id] = color;
    Keybi_DelayUs(100);
    __disable_irq();
    for(int i=0; i < KEYBI_WS2812_COUNT; i++) {
        WS2812S_send_pixel(led_colors[i], KEYBI_WS2812_PIN, (uint32_t *)&KEYBI_WS2812_PIN_PORT->BSRR);
    }
    __enable_irq();
}
