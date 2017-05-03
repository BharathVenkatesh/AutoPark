#include "pwm_hal.h"

TIM_HandleTypeDef timWheels_init;
TIM_OC_InitTypeDef timWheels_conf;

void init_pwm(void)
{
	timWheels_init.Instance = TIM2;
	timWheels_init.Channel = HAL_TIM_ACTIVE_CHANNEL_1 | HAL_TIM_ACTIVE_CHANNEL_2 | HAL_TIM_ACTIVE_CHANNEL_3 | HAL_TIM_ACTIVE_CHANNEL_4;
	
	timWheels_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	timWheels_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	timWheels_init.Init.Prescaler = 10;
	timWheels_init.Init.Period = TIMER_PERIOD_1KHZ;
	timWheels_init.Init.AutoReloadPreload = 0;

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;

	HAL_TIM_PWM_Init(&timWheels_init);

	timWheels_conf.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	timWheels_conf.OCMode = TIM_OCMODE_PWM1;
	timWheels_conf.OCNIdleState = TIM_OCIDLESTATE_RESET;
	timWheels_conf.Pulse = 500;

	HAL_TIM_PWM_ConfigChannel(&timWheels_init, &timWheels_conf, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&timWheels_init, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);
}

void set_pwm(pwm_channel ch, float duty)
{
	switch(ch)
	{
		case right_pwmPD6:
			timWheels_conf.Pulse = (uint32_t)(duty * (float)TIMER_PERIOD_1KHZ);
			HAL_TIM_PWM_ConfigChannel(&timWheels_init, &timWheels_conf, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&timWheels_init, TIM_CHANNEL_3);
			break;
		case right_pwmPD7:
			timWheels_conf.Pulse = (uint32_t)(duty * (float)TIMER_PERIOD_1KHZ);
			HAL_TIM_PWM_ConfigChannel(&timWheels_init, &timWheels_conf, TIM_CHANNEL_4);
			HAL_TIM_PWM_Start(&timWheels_init, TIM_CHANNEL_4);
			break;
        case left_pwmPD3:
            timWheels_conf.Pulse = (uint32_t)(duty * (float)TIMER_PERIOD_1KHZ);
            HAL_TIM_PWM_ConfigChannel(&timWheels_init, &timWheels_conf, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&timWheels_init, TIM_CHANNEL_1);
            break;
        case left_pwmPD4:
            timWheels_conf.Pulse = (uint32_t)(duty * (float)TIMER_PERIOD_1KHZ);
            HAL_TIM_PWM_ConfigChannel(&timWheels_init, &timWheels_conf, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&timWheels_init, TIM_CHANNEL_2);
            break;
	}
}
