#ifndef ENCODER_HAL_H_
#define ENCODER_HAL_H_	1

#include "hal_common_includes.h"

#define LOW_STATE GPIO_PIN_RESET
#define HIGH_STATE GPIO_PIN_SET

void init_encoders(void);
void EXTI2_TSC_IRQHandler(void);
void EXTI3_IRQHandler(void);
void encoders_Callback(uint16_t GPIO_Pin);

struct ENCODERSPINS {
	uint16_t left;
	uint16_t right;
} encodersPins;

struct ENCODERDISTANCE 
{
	long right;
	long left;
} encoders_distances;

#endif