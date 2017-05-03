#include "sensor_hal.h"

TIM_HandleTypeDef right_tim_init;
TIM_Base_InitTypeDef right_tim1_conf;

TIM_HandleTypeDef left_tim_init;
TIM_Base_InitTypeDef left_tim1_conf;

TIM_HandleTypeDef front_tim_init;
TIM_Base_InitTypeDef front_tim1_conf;

void init_echos(void)
{
	static GPIO_InitTypeDef  Right, Left, Front;

	Right.Pin = echosPins.right;
	Right.Mode = GPIO_MODE_IT_RISING_FALLING;
	Right.Pull = GPIO_PULLDOWN;
	Right.Speed = GPIO_SPEED_FREQ_LOW;

	Left.Pin = echosPins.left;
	Left.Mode = GPIO_MODE_IT_RISING_FALLING;
	Left.Pull = GPIO_PULLDOWN;
	Left.Speed = GPIO_SPEED_FREQ_LOW;

	Front.Pin = echosPins.front;
	Front.Mode = GPIO_MODE_IT_RISING_FALLING;
	Front.Pull = GPIO_PULLDOWN;
	Front.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &Right);
	HAL_GPIO_Init(GPIOA, &Left);
	HAL_GPIO_Init(GPIOF, &Front);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 4);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 4);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void init_triggers(void)
{
	static GPIO_InitTypeDef  Right, Left; // Front uses same GPIO group as Left

	Right.Pin = triggerPins.right;
	Right.Mode = GPIO_MODE_OUTPUT_PP;
	Right.Pull = GPIO_PULLDOWN;
	Right.Speed = GPIO_SPEED_FREQ_LOW;

	Left.Pin = triggerPins.left | triggerPins.front;
	Left.Mode = GPIO_MODE_OUTPUT_PP;
	Left.Pull = GPIO_PULLDOWN;
	Left.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &Right);
	HAL_GPIO_Init(GPIOA, &Left);
}

void init_timers() {
	/* Initialize right sensor's timer */
	right_tim_init.Instance = TIM1;
	right_tim_init.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
	right_tim_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	right_tim_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	right_tim_init.Init.Prescaler = 100;
	right_tim_init.Init.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF; /* max value for counter*/
	right_tim_init.Init.AutoReloadPreload = 0;

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;

	HAL_TIM_Base_Init(&right_tim_init);

	right_tim1_conf.Prescaler = 100;
	right_tim1_conf.CounterMode = TIM_COUNTERMODE_UP;
	right_tim1_conf.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF;
	right_tim1_conf.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	right_tim1_conf.RepetitionCounter = 0x00; // was 0xFF;
	right_tim1_conf.AutoReloadPreload = 0;

	/* Initialize front sensor's timer */


	/* Initialize left sensor's timer */
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(echosPins.right);
	HAL_GPIO_EXTI_IRQHandler(echosPins.left);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(echosPins.front);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == echosPins.right) {
		state = HAL_GPIO_ReadPin(GPIOE, GPIO_Pin);
		if (state == HIGH_STATE) {
			HAL_TIM_Base_Start(&right_tim_init);
			__HAL_TIM_SET_COUNTER(&right_tim_init, 0);
		}
		else if (state == LOW_STATE) {
			HAL_TIM_Base_Stop(&right_tim_init);
			right_triggered = 0;
			double counter = __HAL_TIM_GET_COUNTER(&right_tim_init);
			double period = counter/((double)(CPU_TICKS_PER_S)/(right_tim1_conf.Prescaler+1)/**(tim1_conf.ClockDivision+1)*/);
			period = period*1000000;
			double distance = period/58;
			if (distance < 5.0f)
				motorState = STOP;
			else motorState = STRAIGHT;
		}
	}
	else if (GPIO_Pin == echosPins.front) {
		right_triggered = 1;
		front_triggered = 1;
		left_triggered = 1;

		motorState = STOP;
	}	
}