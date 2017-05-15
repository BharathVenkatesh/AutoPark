#include "board_led.h"

// This code initializes pins pe12, pe14, and pe15 for LED use
// The other functions allow turning the LEDs on/off 
void board_led_init(void)
{
	static GPIO_InitTypeDef Leds;

	Leds.Pin = GPIO_PIN_12 /* LED 9 */ | GPIO_PIN_15 /* LED 6 */ | GPIO_PIN_14 /* LED 8 */;
	Leds.Mode = GPIO_MODE_OUTPUT_PP;
	Leds.Pull = GPIO_PULLDOWN;
	Leds.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();
	HAL_GPIO_Init(GPIOE, &Leds);
}

void board_led_on(led led_id)
{
	switch(led_id)
	{
		case LED9:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
			break;
		case LED6:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
			break;
		case LED8:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
			break;
	}
}

void board_led_off(led led_id)
{
	switch(led_id)
	{
		case LED9:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			break;
		case LED6:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
			break;
		case LED8:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
			break;
	}
}

void board_led_all_off()
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
}

void board_led_toggle(led led_id)
{
	switch(led_id)
	{
		case LED9:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
			break;
		case LED6:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
			break;
		case LED8:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);
			break;
	}
}
