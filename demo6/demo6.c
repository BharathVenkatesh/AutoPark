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

    /* Initialization state */
    right_triggered = 0;
    front_triggered = 0;
    left_triggered = 0;
    motorState = STRAIGHT;

    /* Initialize Pins definitions */
    echosPins.right = GPIO_PIN_11;
    echosPins.front = GPIO_PIN_6;
    echosPins.left = GPIO_PIN_9;

    triggerPins.right = GPIO_PIN_10;
    triggerPins.front = GPIO_PIN_0;
    triggerPins.left = GPIO_PIN_0;

    extiRet.front = STRAIGHT;
    extiRet.right = STRAIGHT;
    extiRet.left = STRAIGHT;

    distances.right = 0.0f;
    distances.left = 0.0f;

    firstSense.left = 0;
    firstSense.right = 0;

    treshDist.left = 16.0f;
    treshDist.right = 16.0f;

    init_pwm();
    init_triggers();
    init_echos();
    init_timers();
    int i = 0;

    init_tesh_dist();

    // set_pwm(right_pwmPD6, 0.6f);
    // set_pwm(right_pwmPD7, 0.0f);
    // set_pwm(left_pwmPD3, 0.0f);
    // set_pwm(left_pwmPD4, 0.6f);
    // cpu_sw_delay_us(1000000);

    motorState = STRAIGHT;
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
            /* Trigger sensors */
            if (front_triggered == 0) {
                front_triggered = 1;
                HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_SET);
                // Delay to simulate 10us pulse
                cpu_sw_delay_us(10);
                HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_RESET);
            }
            if (right_triggered == 0) {
                right_triggered = 1;
                HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_SET);
                // Delay to simulate 10us pulse
                cpu_sw_delay_us(10);
                HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_RESET);
            }
            if (left_triggered == 0) {
                left_triggered = 1;
                HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_SET);
                // Delay to simulate 10us pulse
                cpu_sw_delay_us(10);
                HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_RESET);
            }

            /* Car movement */
            // if (extiRet.front == STOP || extiRet.right == STOP || extiRet.left == STOP)
            //     motorState = STOP;
            // if (extiRet.front == STRAIGHT && extiRet.right == STRAIGHT && extiRet.left == STRAIGHT)
            //     motorState = STRAIGHT;

            if (motorState == STRAIGHT) {
                /* Make car go forward */
                set_pwm(right_pwmPD6, 0.6f);
                set_pwm(right_pwmPD7, 0.0f);
                set_pwm(left_pwmPD3, 0.0f);
                set_pwm(left_pwmPD4, 0.6f);

                // if (distances.left > distances.right) {
                //     // Read right sensor
                //     if (distances.right > treshDist.right + 10.0f || distances.right < treshDist.right - 10.0f)
                //         treshDist.right = distances.right;

                //     if (distances.right < treshDist.right) {
                //         set_pwm(right_pwmPD6, 0.8f);
                //         set_pwm(left_pwmPD4, 0.6f);
                //     }
                //     else if (distances.right > treshDist.right + 0.5f) {
                //         set_pwm(right_pwmPD6, 0.6f);
                //         set_pwm(left_pwmPD4, 0.8f);
                //     }
                // } else {
                //     // Read left sensor
                //     if (distances.left > treshDist.left + 10.0f || distances.left < treshDist.left - 10.0f)
                //         treshDist.left = distances.left;

                //     if (distances.left < treshDist.left) {
                //         set_pwm(right_pwmPD6, 0.6f);
                //         set_pwm(left_pwmPD4, 0.8f);
                //     }
                //     else if (distances.left > treshDist.left + 0.5f) {
                //         set_pwm(right_pwmPD6, 0.8f);
                //         set_pwm(left_pwmPD4, 0.6f);
                //     }
                // }
                //cpu_sw_delay_us(100000);
            }
            else if (motorState == STOP) {
                /* Stop car */
                set_pwm(right_pwmPD6, 0.0f);
                set_pwm(right_pwmPD7, 0.0f);
                set_pwm(left_pwmPD3, 0.0f);
                set_pwm(left_pwmPD4, 0.0f);

                // if (distances.left > distances.right)
                //     motorState = LEFTD;
                // else motorState = RIGHTD;
            }
            // else if (motorState == LEFTD) {
            //     set_pwm(right_pwmPD6, 1.0f);
            //     set_pwm(right_pwmPD7, 0.0f);
            //     set_pwm(left_pwmPD3, 0.0f);
            //     set_pwm(left_pwmPD4, 0.2f);

            //     cpu_sw_delay(100U);
            //     motorState = STRAIGHT;

            //     // if (distances.right <= treshDist.right + 0.1f)
            //     //     motorState = STRAIGHT;
            // }
            // else if (motorState == RIGHTD) {
            //     set_pwm(right_pwmPD6, 0.2f);
            //     set_pwm(right_pwmPD7, 0.0f);
            //     set_pwm(left_pwmPD3, 0.0f);
            //     set_pwm(left_pwmPD4, 1.0f);

            //     if (distances.left <= treshDist.left + 0.1f)
            //         motorState = STRAIGHT;
            // }
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