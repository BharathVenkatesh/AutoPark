#ifndef SENSOR_HAL_H_
#define SENSOR_HAL_H_	1

#include "hal_common_includes.h"
#include "pwm_hal.h"
#include "general.h"
#include "frontQueue.h"
#include "rightQueue.h"
#include "encoder_hal.h"

#define TIMER_PERIOD_1KHZ	6545
#define LOW_STATE GPIO_PIN_RESET
#define HIGH_STATE GPIO_PIN_SET

GPIO_PinState state;

/* Initialize pins and enable interrupts for echo signal from sensors */
void init_echos(void);

/* Initialize pins for trigger signal for sensors */
void init_triggers(void);

/* Initialize timers used to get echo signal width */
void init_timers();

/* Calls right sensor callback function */
void EXTI15_10_IRQHandler(void);

/* Calls appropriate callback depending on pin firing the interrupt. Calls callbacks for sensors and encoders */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* Callback function for sensors */
void sensorsCallbacks(TIM_HandleTypeDef tim_init, TIM_Base_InitTypeDef tim1_conf, GPIO_TypeDef* GPIO, uint16_t GPIO_Pin, int* triggered/*, motor_state* ret*/, double dist);

/* Set the trigger pin on the sensor to 1 for 10 us */
void trigger_sensor(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin);

/* Varaibles to store the sensor's echo pins values in */
struct ECHOSPINS {
	uint16_t left;
	uint16_t right;
	uint16_t front;
} echosPins;

/* Varaibles to store the sensor's trigger pins values in */
struct TRIGGERPINS {
	uint16_t left;
	uint16_t right;
	uint16_t front;
} triggerPins;

/* Variables to store distances got by the sensors */
struct DISTANCE 
{
	float front;
	float right;
	float left;
} distances;

/* Treshhold values for adjusting when going straight */
struct TRESHHOLD
{
	float left;
	float right;
} treshDist;

#endif
