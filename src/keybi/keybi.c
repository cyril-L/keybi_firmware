#include "keybi/keybi.h"
#include "keybi/hid_keyboard.h"
#include "keybi/hid_mouse.h"
#include "keybi/tests.h"

void Keybi_Init(void) {

    Keybi_Tests_Run();

    // Use the Nucleo user button as the only keyboard switch for now

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd (NUCLEO_BTN_PERIPH, ENABLE);

    GPIO_InitStructure.GPIO_Pin = NUCLEO_BTN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NUCLEO_BTN_PIN_PORT, &GPIO_InitStructure);
}

void Keybi_MainLoop(void)
{
    static uint8_t prev_switch_pressed = 0;
    uint8_t switch_pressed = !GPIO_ReadInputDataBit(NUCLEO_BTN_PIN_PORT, NUCLEO_BTN_PIN);

    static uint8_t keyboard_report[8] = {0};
    static uint8_t mouse_report[5] = {0};
    if (switch_pressed && !prev_switch_pressed) {
        // press and release key
        keyboard_report[2] = 'a' - 93;
        Keybi_Keyboard_SendReport(keyboard_report);
        keyboard_report[2] = 0;
        Keybi_Keyboard_SendReport(keyboard_report);
        // send a mouse event
        mouse_report[1] = 10;
        Keybi_Mouse_SendReport(mouse_report);
    }
    prev_switch_pressed = switch_pressed;
}
