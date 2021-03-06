#include "pwm_hal.h"

TIM_HandleTypeDef timWheels_init;
TIM_OC_InitTypeDef timWheels_conf;

// Initialize timers and pwm
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

// Config pwm values with func parameters
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

// Adjust car distance from right wall by checking against a threshold distance. When there is a
// large change in distance from the sensors, we change the threshold distance so it will
// stay the same distance from the new wall distance
void adjust() {
	if (searching == 0) {
        if (distances.right > treshDist.right + 12.5f || distances.right < treshDist.right - 12.5f)
            treshDist.right = distances.right;
    } else {
    	treshDist.right = 7.5f;
    	treshDist.left = 7.5f;
    }

    printf("right: %f\n", distances.right);
    if (distances.right < treshDist.right - 0.25f) {
        set_pwm(right_pwmPD6, NORMAL1);
        set_pwm(left_pwmPD4, NORMAL);
    }
    else if (distances.right > treshDist.right + 0.25f) {
        set_pwm(right_pwmPD6, NORMAL);
        set_pwm(left_pwmPD4, NORMAL1);
    }
}

// Set pwm values for all pins
void motors_control(float PD6, float PD7, float PD3, float PD4) {
	set_pwm(right_pwmPD6, PD6);
    set_pwm(right_pwmPD7, PD7);
    set_pwm(left_pwmPD3, PD3);
    set_pwm(left_pwmPD4, PD4);
}
