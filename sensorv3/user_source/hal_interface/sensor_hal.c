#include "sensor_hal.h"

TIM_HandleTypeDef tim_init;
TIM_Base_InitTypeDef tim1_conf;

void init_echos(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 4);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void init_triggers(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void init_timers() {
	tim_init.Instance = TIM1;
	tim_init.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
	tim_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	tim_init.Init.Prescaler = 100;
	tim_init.Init.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF; /* max value for counter*/
	tim_init.Init.AutoReloadPreload = 0;

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;

	HAL_TIM_Base_Init(&tim_init);

	tim1_conf.Prescaler = 100;
	tim1_conf.CounterMode = TIM_COUNTERMODE_UP;
	tim1_conf.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF;
	tim1_conf.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	tim1_conf.RepetitionCounter = 0x00; // was 0xFF;
	tim1_conf.AutoReloadPreload = 0;
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_11 /*&& triggered == 1*/) {
		state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
		if (state == HIGH_STATE) {
			HAL_TIM_Base_Start(&tim_init);
			__HAL_TIM_SET_COUNTER(&tim_init, 0);
		}
		else if (state == LOW_STATE) {
			HAL_TIM_Base_Stop(&tim_init);
			triggered = 0;
			double counter = __HAL_TIM_GET_COUNTER(&tim_init);
			double period = counter/((double)(CPU_TICKS_PER_S)/(tim1_conf.Prescaler+1)/**(tim1_conf.ClockDivision+1)*/);
			period = period*1000000;
			double distance = period/58;
			if (distance < 5.0f)
				motorState = STOP;
			else motorState = STRAIGHT;
		}
	}	
}



/*
void init_echos(void)
{
	tim_init.Instance = TIM1;
	tim_init.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	
	tim_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	tim_init.Init.Prescaler = 10;
	tim_init.Init.Period = TIMER_PERIOD_1KHZ;
	tim_init.Init.AutoReloadPreload = 0;

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;

	HAL_TIM_IC_Init(&tim_init);

	tim1_conf.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
	tim1_conf.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim1_conf.Prescaler = 10;
	tim1_conf.ICFilter = 0xFU;

	HAL_TIM_IC_Start_IT(&tim_init, TIM_CHANNEL_2);
}*/

/*int get_counter() {
	if (__HAL_TIM_GET_COUNTER(&tim_init) > 6000) {
		HAL_TIM_Base_Stop(&tim_init);
		return 1;
	}
	else return 0;
}*/

/*void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);

	if (state == HIGH_STATE) {
		__HAL_TIM_SET_COUNTER(&tim_init, 0);
	}
	else if (state == LOW_STATE) {
		uint32_t count = __HAL_TIM_GET_COUNTER(&tim_init);
		uint32_t period = tim_init.Instance->CCR1 / (TIM1_CLK*(tim_init.Init.Prescaler+1)*(tim1_conf.ICPolarity));
	}
}*/
