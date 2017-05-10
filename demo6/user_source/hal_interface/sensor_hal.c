#include "sensor_hal.h"

TIM_HandleTypeDef tim_init_vec[3];
TIM_Base_InitTypeDef tim_conf_vec[3];

void init_echos(void)
{
	static GPIO_InitTypeDef  Right, Left;//, Front;

	Right.Pin = echosPins.right;
	Right.Mode = GPIO_MODE_IT_RISING_FALLING;
	Right.Pull = GPIO_PULLDOWN;
	Right.Speed = GPIO_SPEED_FREQ_LOW;

	Left.Pin = echosPins.left | echosPins.front;
	Left.Mode = GPIO_MODE_IT_RISING_FALLING;
	Left.Pull = GPIO_PULLDOWN;
	Left.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &Right);
	HAL_GPIO_Init(GPIOF, &Left);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void init_triggers(void)
{
	static GPIO_InitTypeDef  Right, Left, Front;

	Right.Pin = triggerPins.right | GPIO_PIN_9 /*| GPIO_PIN_8 | GPIO_PIN_14*/;
	Right.Mode = GPIO_MODE_OUTPUT_PP;
	Right.Pull = GPIO_PULLDOWN;
	Right.Speed = GPIO_SPEED_FREQ_LOW;

	Left.Pin = triggerPins.left;
	Left.Mode = GPIO_MODE_OUTPUT_PP;
	Left.Pull = GPIO_PULLDOWN;
	Left.Speed = GPIO_SPEED_FREQ_LOW;

	Front.Pin = triggerPins.front;
	Front.Mode = GPIO_MODE_OUTPUT_PP;
	Front.Pull = GPIO_PULLDOWN;
	Front.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &Right);
	HAL_GPIO_Init(GPIOB, &Left);
	HAL_GPIO_Init(GPIOD, &Front);
}

void init_timers() {
	int i = 0;

	for (int i = 0; i < 3; ++i)
	{
		if (i == RIGHT)
			tim_init_vec[i].Instance = TIM1;
		else if (i == FRONT)
			tim_init_vec[i].Instance = TIM3;
		else if (i == LEFT)
			tim_init_vec[i].Instance = TIM4;

		tim_init_vec[i].Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
		tim_init_vec[i].Init.CounterMode = TIM_COUNTERMODE_UP;
		tim_init_vec[i].Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		tim_init_vec[i].Init.Prescaler = 72;
		tim_init_vec[i].Init.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF; /* max value for counter*/
		tim_init_vec[i].Init.AutoReloadPreload = 0;

		HAL_TIM_Base_Init(&tim_init_vec[i]);

		tim_conf_vec[i].Prescaler = 72;
		tim_conf_vec[i].CounterMode = TIM_COUNTERMODE_UP;
		tim_conf_vec[i].Period = /*TIMER_PERIOD_1KHZ*/0xFFFF;
		tim_conf_vec[i].ClockDivision = TIM_CLOCKDIVISION_DIV1;
		tim_conf_vec[i].RepetitionCounter = 0x00; // was 0xFF;
		tim_conf_vec[i].AutoReloadPreload = 0;
	}

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(echosPins.right);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(echosPins.front);
	HAL_GPIO_EXTI_IRQHandler(echosPins.left);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == echosPins.right)
		sensorsCallbacks(tim_init_vec[RIGHT], tim_conf_vec[RIGHT], GPIOE, GPIO_Pin, &right_triggered/*, &extiRet.right*/, 10.0f);
	else if (GPIO_Pin == echosPins.front)
		sensorsCallbacks(tim_init_vec[FRONT], tim_conf_vec[FRONT], GPIOF, GPIO_Pin, &front_triggered/*, &extiRet.front*/, 17.5f);
	else if (GPIO_Pin == echosPins.left)
		sensorsCallbacks(tim_init_vec[LEFT], tim_conf_vec[LEFT], GPIOF, GPIO_Pin, &left_triggered/*, &extiRet.left*/, 5.0f);
	else if (GPIO_Pin == encodersPins.right) {
		encoders_Callback(encodersPins.right);
	}
	else if (GPIO_Pin == encodersPins.left)
		encoders_Callback(encodersPins.left);
}

void sensorsCallbacks(TIM_HandleTypeDef tim_init, TIM_Base_InitTypeDef tim1_conf, GPIO_TypeDef* GPIO, uint16_t GPIO_Pin, int* triggered/*, motor_state* ret*/, double dist) {
	state = HAL_GPIO_ReadPin(GPIO, GPIO_Pin);
	if (state == HIGH_STATE) {
		HAL_TIM_Base_Start(&tim_init);
		__HAL_TIM_SET_COUNTER(&tim_init, 0);
	}
	else if (state == LOW_STATE) {
		float counter = __HAL_TIM_GET_COUNTER(&tim_init);
		*triggered = 0;
		float distance = counter/58;
		// if (front_triggered == 0)
		// 	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
		//printf("\n front sensor: %d\n", front_triggered);

		if (GPIO_Pin == echosPins.front && motorState == STRAIGHT && searching == 1) {

			distances.front = distance;
			// if (queue_init == 1)
			// 	insert(distance);
			// else {
				removeData();
				insert(distance);
				distance = average();
			
				if (distance <= dist) {
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
					encoders_distances.right = 0; // remove if adjusting
					motorState = LEFTD;
				} else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
			// }
		}
		else if(GPIO_Pin == echosPins.right) {
			// removeDataRight();
			// insertRight(distance);
			// distance = averageRight();
			distances.right = distance;
		}
		else if(GPIO_Pin == echosPins.left) {
			distances.left = distance;
		}
	}
}

void init_tresh_dist() {
	trigger_sensor(GPIOE, triggerPins.right);

	trigger_sensor(GPIOB, triggerPins.left);
}

void trigger_sensor(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIO, GPIO_Pin, GPIO_PIN_SET);
    // Delay to simulate 10us pulse
	cpu_sw_delay_us(10);
    HAL_GPIO_WritePin(GPIO, GPIO_Pin, GPIO_PIN_RESET);

}