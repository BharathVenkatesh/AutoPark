#ifndef PWM_HAL_H_
#define PWM_HAL_H_	1

#include "hal_common_includes.h"
#include "sensor_hal.h"

#define TIMER_PERIOD_1KHZ	6545

typedef enum {
	right_pwmPD6,
	right_pwmPD7,
    left_pwmPD3,
    left_pwmPD4
} pwm_channel;

/* Initialize pins and timers for PWM */
void init_pwm(void);

/* Set duty cycle for PWM */
void set_pwm(pwm_channel ch, float duty);

/* Function to adjust the car's position to go in a straight line */
void adjust();

/* Function to set the different duty cycles for the wheels */
void motors_control(float PD6, float PD7, float PD3, float PD4);

#endif
