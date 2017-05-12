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

void init_pwm(void);
void set_pwm(pwm_channel ch, float duty);
void adjust();
void motors_control(float PD6, float PD7, float PD3, float PD4);

#endif
