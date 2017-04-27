#include "board_led.h"

void sensor_trigger_init(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void sensor_echo_init (void) {
    sensor_echo_init_gpio();
}

void sensor_echo_init_gpio(void) {
    static GPIO_InitTypeDef  GPIO_InitStruct_echo;
    
    GPIO_InitStruct_echo.Pin = GPIO_PIN_11;
    GPIO_InitStruct_echo.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct_echo.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct_echo.Speed = GPIO_SPEED_FREQ_LOW;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct_echo);
}

void set_sensor() {
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
}

void reset_sensor() {
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
}

int read_sensor() {
    return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
}

void board_led_on(led led_id)
{
	switch(led_id)
	{
		case LED1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			break;
		case LED2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			break;
	}
}

void board_led_off(led led_id)
{
	switch(led_id)
	{
		case LED1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
			break;
		case LED2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
	}
}

void board_led_toggle(led led_id)
{
	switch(led_id)
	{
		case LED1:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
			break;
		case LED2:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
			break;
	}
}
