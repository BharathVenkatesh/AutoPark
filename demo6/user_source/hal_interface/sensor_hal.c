#include "sensor_hal.h"

// TIM_HandleTypeDef right_tim_init;
// TIM_Base_InitTypeDef right_tim1_conf;

// TIM_HandleTypeDef left_tim_init;
// TIM_Base_InitTypeDef left_tim1_conf;

// TIM_HandleTypeDef front_tim_init;
// TIM_Base_InitTypeDef front_tim1_conf;

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

	/*Front.Pin = echosPins.front;
	Front.Mode = GPIO_MODE_IT_RISING_FALLING;
	Front.Pull = GPIO_PULLDOWN;
	Front.Speed = GPIO_SPEED_FREQ_LOW;*/

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	//__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOE, &Right);
	HAL_GPIO_Init(GPIOF, &Left);
	//HAL_GPIO_Init(GPIOF, &Front);

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

	/* Initialize right sensor's timer */
	// right_tim_init.Instance = TIM1;
	// right_tim_init.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
	// right_tim_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	// right_tim_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	// right_tim_init.Init.Prescaler = 100;
	// right_tim_init.Init.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF; /* max value for counter*/
	// right_tim_init.Init.AutoReloadPreload = 0;

	// HAL_TIM_Base_Init(&right_tim_init);

	// right_tim1_conf.Prescaler = 100;
	// right_tim1_conf.CounterMode = TIM_COUNTERMODE_UP;
	// right_tim1_conf.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF;
	// right_tim1_conf.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	// right_tim1_conf.RepetitionCounter = 0x00; // was 0xFF;
	// right_tim1_conf.AutoReloadPreload = 0;

	/* Initialize front sensor's timer */
	// front_tim_init.Instance = TIM3;
	// front_tim_init.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
	// front_tim_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	// front_tim_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	// front_tim_init.Init.Prescaler = 100;
	// front_tim_init.Init.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF; /* max value for counter*/
	// front_tim_init.Init.AutoReloadPreload = 0;

	// HAL_TIM_Base_Init(&front_tim_init);

	// front_tim1_conf.Prescaler = 100;
	// front_tim1_conf.CounterMode = TIM_COUNTERMODE_UP;
	// front_tim1_conf.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF;
	// front_tim1_conf.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	// front_tim1_conf.RepetitionCounter = 0x00; // was 0xFF;
	// front_tim1_conf.AutoReloadPreload = 0;

	/* Initialize left sensor's timer */
	// left_tim_init.Instance = TIM4;
	// left_tim_init.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
	// left_tim_init.Init.CounterMode = TIM_COUNTERMODE_UP;
	// left_tim_init.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	// left_tim_init.Init.Prescaler = 100;
	// left_tim_init.Init.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF; /* max value for counter*/
	// left_tim_init.Init.AutoReloadPreload = 0;

	// HAL_TIM_Base_Init(&left_tim_init);

	// left_tim1_conf.Prescaler = 100;
	// left_tim1_conf.CounterMode = TIM_COUNTERMODE_UP;
	// left_tim1_conf.Period = /*TIMER_PERIOD_1KHZ*/0xFFFF;
	// left_tim1_conf.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	// left_tim1_conf.RepetitionCounter = 0x00; // was 0xFF;
	// left_tim1_conf.AutoReloadPreload = 0;
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
		//printf("Here\n");
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
		*triggered = 0;
		//HAL_TIM_Base_Stop(&tim_init);
		float counter = __HAL_TIM_GET_COUNTER(&tim_init);
		//float period = counter/(/*(double)(CPU_TICKS_PER_S)/(tim1_conf.Prescaler+1)*/(float)1000000f);
		//period = period*(float)1000000.0f;
		float distance = counter/58;

		if (GPIO_Pin == echosPins.front && motorState == STRAIGHT && searching == 1) {
			// if (distance < dist) 
			// 	motorState = STOP;
			// else motorState = STRAIGHT;
			// if (queue_init == 1) {
			// 	insert(distance);
			// } else {
			// 	removeData();
			// 	insert(distance);
				//distance = average();

				if (distance <= dist) {
					//printf("distance: %f\n", distance);
					// motorState = STOP;
					HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
					// if (distances.left > distances.right)
					encoders_distances.right = 0; // remove if adjusting
					motorState = LEFTD;
					// else motorState = RIGHTD;
				} else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
			// }
			// else motorState = STRAIGHT;

			/*if (motorState == LEFTD || motorState == RIGHTD)
				if (distance > 100.0f)
					motorState = STRAIGHT;*/
		}
		else if(GPIO_Pin == echosPins.right) {
			distances.right = distance;	
			//printf("distance r: %f\n", distance);
			// if (firstSense.right == 0) {
			// 	firstSense.right = 1;
			// 	//treshDist.right = distance;
			// }
			// if (distance < dist)
			// 	motorState = STOP;
			// else motorState = STRAIGHT;
		}
		else if(GPIO_Pin == echosPins.left) {
			distances.left = distance;
			// if (firstSense.left == 0) {
			// 	firstSense.left = 1;
			// 	//treshDist.left = distance;
			// }
			// if (distance < dist)
			// 	motorState = STOP;
			// else motorState = STRAIGHT;
		}
	}
}

void init_tresh_dist() {
	trigger_sensor(GPIOE, triggerPins.right);

	trigger_sensor(GPIOB, triggerPins.left);

	// HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_SET);
 //    // Delay to simulate 10us pulse
	// cpu_sw_delay_us(10);
 //    HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_RESET);

 //    HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_SET);
 //    // Delay to simulate 10us pulse
 //    cpu_sw_delay_us(10);
 //    HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_RESET);
}

void trigger_sensor(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIO, GPIO_Pin, GPIO_PIN_SET);
    // Delay to simulate 10us pulse
	cpu_sw_delay_us(10);
    HAL_GPIO_WritePin(GPIO, GPIO_Pin, GPIO_PIN_RESET);

}