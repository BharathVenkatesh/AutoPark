#include "encoder_hal.h"

void init_encoders(void) {
	static GPIO_InitTypeDef  Encoders;

	Encoders.Pin = encodersPins.right | encodersPins.left;
	Encoders.Mode = GPIO_MODE_IT_RISING;
	Encoders.Pull = GPIO_PULLDOWN;
	Encoders.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &Encoders);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI4_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(encodersPins.left);
}

void EXTI3_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(encodersPins.right);
}

void encoders_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == encodersPins.right) {
		// if (HAL_GPIO_ReadPin(GPIOA, GPIO_Pin) == HIGH_STATE) {
			++encoders_distances.right;
			printf("%ld\n", encoders_distances.right);
		// }
	}
	else if (GPIO_Pin == encodersPins.left)
		// if (HAL_GPIO_ReadPin(GPIOA, GPIO_Pin) == HIGH_STATE) {
			encoders_distances.left++;
		// }
}