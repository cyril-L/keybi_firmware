#include "keybi/drivers/matrix.h"
#include "stm32f10x_systick.h"

typedef struct gpio_t {
	GPIO_TypeDef * port;
	uint16_t pin;
} gpio_t;

static gpio_t rows[KEYBI_MATRIX_ROWS] = {
		{GPIOC, GPIO_Pin_3},  // R01
		{GPIOA, GPIO_Pin_0},  // R02
		{GPIOA, GPIO_Pin_1},  // R03
		{GPIOC, GPIO_Pin_4},  // R04
		{GPIOC, GPIO_Pin_5}}; // R05

static gpio_t cols[KEYBI_MATRIX_COLS] = {
		{GPIOB, GPIO_Pin_7},  // C01
		{GPIOB, GPIO_Pin_8},  // C02
		{GPIOB, GPIO_Pin_9},  // C03
		{GPIOC, GPIO_Pin_0},  // C04
		{GPIOC, GPIO_Pin_1},  // C05
		{GPIOC, GPIO_Pin_2},  // C06
		{GPIOA, GPIO_Pin_15}, // C07
		{GPIOC, GPIO_Pin_10}, // C08
		{GPIOC, GPIO_Pin_11}, // C09
		{GPIOC, GPIO_Pin_12}, // C10
		{GPIOA, GPIO_Pin_10}, // C11
		{GPIOA, GPIO_Pin_9},  // C12
		{GPIOC, GPIO_Pin_9},  // C13
		{GPIOB, GPIO_Pin_1}}; // C14

static uint8_t key_states[KEYBI_MATRIX_COLS][KEYBI_MATRIX_ROWS] = {0};

void Keybi_Matrix_Init(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	for (int r = 0; r < KEYBI_MATRIX_ROWS; ++r) {
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = rows[r].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(rows[r].port, &GPIO_InitStructure);
	}

	for (int c = 0; c < KEYBI_MATRIX_COLS; ++c) {
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = cols[c].pin;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(cols[c].port, &GPIO_InitStructure);
	}
}
static void delayMicroseconds(uint32_t delay_us);
static void delayMicroseconds(uint32_t delay_us)
{
    uint32_t remaining_ticks = delay_us * 72;

    uint32_t prev = SysTick_GetCounter();

  while (1)
  {
      uint32_t curr = SysTick_GetCounter();
      uint32_t elapsed;
      if (curr > prev) {
          elapsed = prev; // FIXME don’t know initial value
      } else {
          elapsed = prev - curr;
      }
      if (elapsed > remaining_ticks) {
          break;
      }
      remaining_ticks -= elapsed;
      prev = curr;
  }
}

void Keybi_Matrix_Scan(int (*callback)(keybi_keyboard_matrix_event_t)) {
	for (uint8_t c = 0; c < KEYBI_MATRIX_COLS; ++c) {
		GPIO_SetBits(cols[c].port, cols[c].pin);
		for (uint8_t r = 0; r < KEYBI_MATRIX_ROWS; ++r) {
			uint8_t state = GPIO_ReadInputDataBit(rows[r].port, rows[r].pin);
			if (state != key_states[c][r]) {
				keybi_keyboard_matrix_event_t event = {
						.col = c,
						.row = r,
						.pressed = state,
						.time = 0
				};
				if (callback(event) == 0) {
					// Save state if callback has been able to handle it
					key_states[c][r] = state;
				}
			}
		}
		GPIO_ResetBits(cols[c].port, cols[c].pin);
		// Adjacent columns of the first rows registered on a single key press
		// TODO investigate why and find the appropriate delay
		delayMicroseconds(100);
	}
}
