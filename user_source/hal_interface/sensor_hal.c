#include "sensor_hal.h"

TIM_HandleTypeDef tim_init_vec[3];
TIM_Base_InitTypeDef tim_conf_vec[3];

// Initializes echo pins - Left and Front pins are in the same group so we can init them together
void init_echos(void)
{
	static GPIO_InitTypeDef  Right, Left;

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

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

// Initializes trigger pins for sensors
void init_triggers(void)
{
	static GPIO_InitTypeDef  Right, Left, Front;

	Right.Pin = triggerPins.right | GPIO_PIN_9;
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

// Initialize timers used for sensors
// We determined the values for the timers by using the equations in the stm32 manual
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
		tim_init_vec[i].Init.Period = 0xFFFF; /* max value for counter*/
		tim_init_vec[i].Init.AutoReloadPreload = 0;

		HAL_TIM_Base_Init(&tim_init_vec[i]);

		tim_conf_vec[i].Prescaler = 72;
		tim_conf_vec[i].CounterMode = TIM_COUNTERMODE_UP;
		tim_conf_vec[i].Period = 0xFFFF;
		tim_conf_vec[i].ClockDivision = TIM_CLOCKDIVISION_DIV1;
		tim_conf_vec[i].RepetitionCounter = 0x00; // was 0xFF;
		tim_conf_vec[i].AutoReloadPreload = 0;
	}

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
}

// External Interrupt handlers for specific echo pins
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(echosPins.right);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(echosPins.front);
	HAL_GPIO_EXTI_IRQHandler(echosPins.left);
}

// Overwriting external interrupt callback functions
// This handles interrupts for both the sensors and the encoders
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == echosPins.right)
		sensorsCallbacks(tim_init_vec[RIGHT], tim_conf_vec[RIGHT], GPIOE, GPIO_Pin, &right_triggered, 10.0f);
	else if (GPIO_Pin == echosPins.front)
		sensorsCallbacks(tim_init_vec[FRONT], tim_conf_vec[FRONT], GPIOF, GPIO_Pin, &front_triggered, 18.5f);
	else if (GPIO_Pin == echosPins.left)
		sensorsCallbacks(tim_init_vec[LEFT], tim_conf_vec[LEFT], GPIOF, GPIO_Pin, &left_triggered, 5.0f);
	else if (GPIO_Pin == encodersPins.right) {
		encoders_Callback(encodersPins.right);
	}
	else if (GPIO_Pin == encodersPins.left)
		encoders_Callback(encodersPins.left);
}

// This function calculates the distance given by the sensors by measuring the 
// length of the echo response.  We check the value of the given GPIO pin - when it 
// is high (1), we start the timer and when it goes low we calculate the distance by
// using values found in the manual.  Depending on this distance, we then change our
// car's states.
void sensorsCallbacks(TIM_HandleTypeDef tim_init, TIM_Base_InitTypeDef tim1_conf, GPIO_TypeDef* GPIO, uint16_t GPIO_Pin, int* triggered, double dist) {
	state = HAL_GPIO_ReadPin(GPIO, GPIO_Pin);
	if (state == HIGH_STATE) {
		HAL_TIM_Base_Start(&tim_init);
		__HAL_TIM_SET_COUNTER(&tim_init, 0);
	}
	else if (state == LOW_STATE) {
		float counter = __HAL_TIM_GET_COUNTER(&tim_init);
		*triggered = 0;
		float distance = counter/58;

		if (GPIO_Pin == echosPins.front && motorState == STRAIGHT && searching == 1) {

			distances.front = distance;

			removeData();
			insert(distance);
			distance = average();
			
			if (distance <= dist) {
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
				encoders_distances.right = 0;
				motorState = LEFTD;
			} else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
		}
		else if(GPIO_Pin == echosPins.right) {
			distances.right = distance;
		}
		else if(GPIO_Pin == echosPins.left) {
			distances.left = distance;
		}
	}
}

/* Deprecated
void init_tresh_dist() {
	trigger_sensor(GPIOE, triggerPins.right);

	trigger_sensor(GPIOB, triggerPins.left);
}
*/

// Send a pulse on the trigger pin.  We simply write a 1 on the pin
// then wait for a delay before setting it to 0 again
void trigger_sensor(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIO, GPIO_Pin, GPIO_PIN_SET);
    // Delay to simulate 10us pulse
	cpu_sw_delay_us(10);
    HAL_GPIO_WritePin(GPIO, GPIO_Pin, GPIO_PIN_RESET);
}
