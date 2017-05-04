#include "hal_common_includes.h"

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(htim_pwm->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
		
		__HAL_RCC_GPIOD_CLK_ENABLE();
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_pwm)
{
	if(htim_pwm->Instance == TIM1)
		__HAL_RCC_TIM1_CLK_ENABLE();

	if(htim_pwm->Instance == TIM3)
		__HAL_RCC_TIM3_CLK_ENABLE();

	if(htim_pwm->Instance == TIM4)
		__HAL_RCC_TIM4_CLK_ENABLE();
}