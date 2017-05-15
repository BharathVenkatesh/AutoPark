#ifndef ENCODER_HAL_H_
#define ENCODER_HAL_H_	1

#include "hal_common_includes.h"

/* Pin states */
#define LOW_STATE GPIO_PIN_RESET
#define HIGH_STATE GPIO_PIN_SET

/* Initialize pins and enable interupts */
void init_encoders(void);

/* Calls the left encoder callback function */
void EXTI4_IRQHandler(void);

/* Calls the right encoder callback function */
void EXTI3_IRQHandler(void);

/* Increments encoders value to get rotation of the wheel */
void encoders_Callback(uint16_t GPIO_Pin);

/* Varaibles to store the encoders pin values in */
struct ENCODERSPINS {
	uint16_t left;
	uint16_t right;
} encodersPins;

/* Variables to store the count of the slots on the encoders */
struct ENCODERDISTANCE 
{
	long right;
	long left;
} encoders_distances;

struct ENCODERCOUNTER 
{
	int right;
	int left;
} enc_counter;

#endif