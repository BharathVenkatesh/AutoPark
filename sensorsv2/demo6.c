/*
	This is the main user software application
	In this case, we have below the code necessary to use PWM with LEDs on an STM32 F3 Discovery PCB.
 
	(c) Abhimanyu Ghosh, 2017
 */

// C standard include(s):
#include <math.h>
#include <stdio.h>

// Custom user APIs needed for hardware access specific to this board:
#include "cpu.h"
//#include "board_led.h"
#include "pwm_hal.h"
#include "sensor_hal.h"
#include "general.h"

// Custom user APIs needed for generic algorithmic libraries that are hardware-independent:
#include "foo.h"

int main()
{
    /*
     Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
     */
    cpu_init();
    
    /*
     WARNING: We cannot use one of the GPIOs (PE9) from board_led since it is now being used for PWM!!
     */
    
    /*
     Initialize the PWM outputs for PE9 and PE11 which are connected to LD3 and LD7:
     */
    triggered = 0;
    motorState = STRAIGHT;
    init_pwm();
    init_triggers();
    init_echos();
    init_timers();
    int i = 0;

    // set_pwm(right_pwmPD6, 0.6f);
    // set_pwm(right_pwmPD7, 0.0f);
    // set_pwm(left_pwmPD3, 0.0f);
    // set_pwm(left_pwmPD4, 0.6f);
    // cpu_sw_delay_us(1000000);
    
    /*
     In an infinite loop, keep toggling the LEDs in an alternating pattern:
     */
    while(1)
    {
        /*
         Carry out a simple unit test of foo() declared in foo.h:
         */
        if(TEST_FOO(i, i+1) < 0)
        {
            /*
             If the above fails there is either a hardware, code or other undefined error.
             Now we're in an undefined state regarding processor core behavior...
             */
            while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
        }
        else
        {
            if (motorState == STRAIGHT) {
                /* Make car go forward */
                set_pwm(right_pwmPD6, 0.6f);
                set_pwm(right_pwmPD7, 0.0f);
                set_pwm(left_pwmPD3, 0.0f);
                set_pwm(left_pwmPD4, 0.6f);
                cpu_sw_delay_us(100000);

                if (triggered == 0) {
                    triggered = 1;
                    /* Trigger sensors */
                    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                    /* Delay to simulate 10us */
                    cpu_sw_delay_us(10);
                    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
                }
            }
            else if (motorState == STOP) {
                /* Stop car */
                set_pwm(right_pwmPD6, 0.0f);
                set_pwm(right_pwmPD7, 0.0f);
                set_pwm(left_pwmPD3, 0.0f);
                set_pwm(left_pwmPD4, 0.0f);
                //cpu_sw_delay_us(100000);
                //motorState = STRAIGHT;
            }
            /*set_pwm(right_pwmPD6, 0.6f);
            set_pwm(right_pwmPD7, 0.0f);
            set_pwm(left_pwmPD3, 0.0f);
            set_pwm(left_pwmPD4, 0.6f);
            cpu_sw_delay_us(1000000);*/
            //triggered = 1;
            // HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
            // cpu_sw_delay_us(10);
            // HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
            // cpu_sw_delay_us(1000000);
        }
    }
    
    return 0;
}