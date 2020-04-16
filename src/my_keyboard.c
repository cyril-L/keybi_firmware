#include "my_keyboard.h"
#include "keyboard.h"

void MainLoopFlagEnable(void);
void MainLoopFlagToggle(void);
void UserSwitchEnable(void);
uint8_t UserSwitchRead(void);
uint8_t UserSwitchPressedEvent(void);

void MainLoopFlagEnable()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // enable port clock
    RCC_APB2PeriphClockCmd(MAIN_LOOP_FLAG_PERIPH, ENABLE);

    // set pin modes
    GPIO_InitStructure.GPIO_Pin = MAIN_LOOP_FLAG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MAIN_LOOP_FLAG_PIN_PORT, &GPIO_InitStructure);
}

static int mainloop_flag = 0;

void MainLoopFlagToggle() {
	mainloop_flag = !mainloop_flag;
	if (mainloop_flag) {
		GPIO_SetBits(MAIN_LOOP_FLAG_PIN_PORT, MAIN_LOOP_FLAG_PIN);
	} else {
        GPIO_ResetBits(MAIN_LOOP_FLAG_PIN_PORT, MAIN_LOOP_FLAG_PIN);
    }
}

void UserSwitchEnable() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd (USER_SWITCH_PERIPH, ENABLE);

    GPIO_InitStructure.GPIO_Pin = USER_SWITCH_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USER_SWITCH_PIN_PORT, &GPIO_InitStructure);
}

uint8_t UserSwitchRead() {
    return !GPIO_ReadInputDataBit(USER_SWITCH_PIN_PORT, USER_SWITCH_PIN);
}

static uint8_t user_switch_state = 0;
uint8_t UserSwitchPressedEvent() {
	uint8_t new_state = UserSwitchRead();
	uint8_t pressed_event = 0;
	if (new_state && !user_switch_state) {
		pressed_event = 1;
	}
	user_switch_state = new_state;
	return pressed_event;
}

void MyKeyboardSetup() {
	MainLoopFlagEnable();
	UserSwitchEnable();
}

void MyKeyboardLoop() {
	//MainLoopFlagToggle();
	if (UserSwitchPressedEvent()) {
		MainLoopFlagToggle();
		sendChar('a');
	}
}
