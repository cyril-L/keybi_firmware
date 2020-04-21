#include "keybi/keyboard.h"

#include "usb_pwr.h"
#include "usb_mem.h"
#include "usb_regs.h"
#include "CCIDHID_usb_conf.h"

void Keybi_Keyboard_Init(void) {

    // Use the Nucleo user button as the only keyboard switch for now

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd (NUCLEO_BTN_PERIPH, ENABLE);

    GPIO_InitStructure.GPIO_Pin = NUCLEO_BTN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NUCLEO_BTN_PIN_PORT, &GPIO_InitStructure);
}

extern __IO uint8_t keybi_keyboard_prev_xfer_complete;

void Keybi_Keyboard_SendReport(uint8_t * report);

void Keybi_Keyboard_SendReport(uint8_t * report) {

    if (bDeviceState == CONFIGURED)
    {
        while (!keybi_keyboard_prev_xfer_complete);

        keybi_keyboard_prev_xfer_complete = 0;
        /* Use the memory interface function to write to the selected endpoint */
        UserToPMABufferCopy(report, CCID_ENDP5_TXADDR, 8);

        /* Update the data length in the control register */
        SetEPTxCount(ENDP5, 8);
        SetEPTxStatus(ENDP5, EP_TX_VALID);
    }
}

void Keybi_Keyboard_MainLoop(void)
{
    static uint8_t prev_switch_pressed = 0;
    uint8_t switch_pressed = !GPIO_ReadInputDataBit(NUCLEO_BTN_PIN_PORT, NUCLEO_BTN_PIN);

    static uint8_t report[8] = {0};
    if (switch_pressed && !prev_switch_pressed) {
        report[2] = 'a' - 93;
        Keybi_Keyboard_SendReport(report);
        report[2] = 0;
        Keybi_Keyboard_SendReport(report);
    }
    prev_switch_pressed = switch_pressed;
}