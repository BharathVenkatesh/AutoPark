#ifndef ENCODER_HAL_H_
#define ENCODER_HAL_H_	1

#include "hal_common_includes.h"
#include "sensor_hal.h"

#define LOW_STATE GPIO_PIN_RESET
#define HIGH_STATE GPIO_PIN_SET

void init_encoders(void);
void EXTI4_IRQHandler(void);
void EXTI3_IRQHandler(void);
void encoders_Callback(uint16_t GPIO_Pin, TIM_HandleTypeDef tim_init, GPIO_TypeDef* GPIO);

struct ENCODERSPINS {
	uint16_t left;
	uint16_t right;
} encodersPins;

struct ENCODERDISTANCE 
{
	long right;
	long left;
} encoders_distances;

// long enc_diff;

struct ENCODERCOUNTER 
{
	int right;
	int left;
} enc_counter;

#endif