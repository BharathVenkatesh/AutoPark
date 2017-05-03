#ifndef SENSOR_HAL_H_
#define SENSOR_HAL_H_	1

#include "hal_common_includes.h"
#include "pwm_hal.h"
#include "general.h"

#define TIMER_PERIOD_1KHZ	6545
#define LOW_STATE GPIO_PIN_RESET
#define HIGH_STATE GPIO_PIN_SET

GPIO_PinState state;

void init_echos(void);
void init_triggers(void);
void init_timers();
void EXTI15_10_IRQHandler(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void sensorsCallbacks(TIM_HandleTypeDef tim_init, TIM_Base_InitTypeDef tim1_conf, GPIO_TypeDef* GPIO, uint16_t GPIO_Pin, int* triggered, motor_state* ret, double dist);

struct ECHOSPINS {
	uint16_t left;
	uint16_t right;
	uint16_t front;
} echosPins;

struct TRIGGERPINS {
	uint16_t left;
	uint16_t right;
	uint16_t front;
} triggerPins;

struct EXTIRETURNS
{
	motor_state left;
	motor_state right;
	motor_state front;
} extiRet;

#endif