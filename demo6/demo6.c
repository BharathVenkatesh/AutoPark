// C standard include(s):
#include <math.h>
#include <stdio.h>

// Custom user APIs needed for hardware access specific to this board:
#include "cpu.h"
#include "pwm_hal.h"
#include "sensor_hal.h"
#include "general.h"
#include "uart.h"
#include "queue.h"

// Custom user APIs needed for generic algorithmic libraries that are hardware-independent:
#include "foo.h"

void init_sensors_values() {
    right_triggered = 0;
    front_triggered = 0;
    left_triggered = 0;

    distances.right = 0.0f;
    distances.left = 0.0f;

    firstSense.left = 0;
    firstSense.right = 0;

    treshDist.left = 16.0f;
    treshDist.right = 16.0f;

    // extiRet.front = STRAIGHT;
    // extiRet.right = STRAIGHT;
    // extiRet.left = STRAIGHT;
}

void init_pins() {
    echosPins.right = GPIO_PIN_11;
    echosPins.front = GPIO_PIN_6;
    echosPins.left = GPIO_PIN_9;

    triggerPins.right = GPIO_PIN_10;
    triggerPins.front = GPIO_PIN_0;
    triggerPins.left = GPIO_PIN_0;
}

int main()
{
    /*
     Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
     */
    cpu_init();

    /* Debug uart port initialization and test */
    uart_debug_init();
    setvbuf(stdin,NULL,_IONBF,0);   // Sets stdin in unbuffered mode (normal for usart com)
    setvbuf(stdout,NULL,_IONBF,0);  // Sets stdin in unbuffered mode (normal for usart com)
    printf("Hello World!!\r\n");

    /* Initialize motor state */
    motorState = STRAIGHT;

    /* Initialize pwm to control motors */
    init_pwm();

    /* Initialize sensors */
    init_pins();
    init_sensors_values();
    init_triggers();
    init_echos();
    init_timers();
    init_tresh_dist();
    init_queue(); // Initialize sensor value averaging

    int i = 0;

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
                trigger_sensor(GPIOD, triggerPins.front);
                // HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_SET);
                // // Delay to simulate 10us pulse
                // cpu_sw_delay_us(10);
                // HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_RESET);
            }
            if (right_triggered == 0) {
                right_triggered = 1;
                trigger_sensor(GPIOE, triggerPins.right);
                // HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_SET);
                // // Delay to simulate 10us pulse
                // cpu_sw_delay_us(10);
                // HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_RESET);
            }
            if (left_triggered == 0) {
                left_triggered = 1;
                trigger_sensor(GPIOB, triggerPins.left);
                // HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_SET);
                // // Delay to simulate 10us pulse
                // cpu_sw_delay_us(10);
                // HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_RESET);
            }

            /* Car movement */
            // if (extiRet.front == STOP || extiRet.right == STOP || extiRet.left == STOP)
            //     motorState = STOP;
            // if (extiRet.front == STRAIGHT && extiRet.right == STRAIGHT && extiRet.left == STRAIGHT)
            //     motorState = STRAIGHT;

            if (motorState == STRAIGHT) {
                /* Make car go forward */
                motors_control(0.5f, 0.0f, 0.0f, 0.5f);
                adjust();
                // set_pwm(right_pwmPD6, 0.5f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.5f);

                // if (distances.left > distances.right) {
                //     // Read right sensor
                //     if (distances.right > treshDist.right + 10.0f || distances.right < treshDist.right - 10.0f)
                //         treshDist.right = distances.right;

                //     if (distances.right < treshDist.right) {
                //         set_pwm(right_pwmPD6, 0.7f);
                //         set_pwm(left_pwmPD4, 0.5f);
                //     }
                //     else if (distances.right > treshDist.right + 0.5f) {
                //         set_pwm(right_pwmPD6, 0.5f);
                //         set_pwm(left_pwmPD4, 0.7f);
                //     }
                // } else {
                //     // Read left sen
                //     if (distances.left > treshDist.left + 10.0f || distances.left < treshDist.left - 10.0f)
                //         treshDist.left = distances.left;

                //     if (distances.left < treshDist.left) {
                //         set_pwm(right_pwmPD6, 0.5f);
                //         set_pwm(left_pwmPD4, 0.7f);
                //     }
                //     else if (distances.left > treshDist.left + 0.5f) {
                //         set_pwm(right_pwmPD6, 0.7f);
                //         set_pwm(left_pwmPD4, 0.5f);
                //     }
                // }
                //cpu_sw_delay_us(100000);
            }
            else if (motorState == STOP) {
                motors_control(0.0f,0.0f,0.0f,0.0f);

                if (distances.left > distances.right)
                    motorState = LEFTD;
                else motorState = RIGHTD;

                // set_pwm(right_pwmPD6, 0.0f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.0f);

                // if (distances.left > distances.right)
                //     motorState = LEFTD;
                // else motorState = RIGHTD;
            }
            else if (motorState == LEFTD) {
                motors_control(0.8f, 0.0f, 0.0f, 0.0f);

                if (distances.right <= treshDist.right + 0.1f)
                    motorState = STRAIGHT;

                // set_pwm(right_pwmPD6, 0.8f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.0f);

                // cpu_sw_delay(100U);
                // motorState = STRAIGHT;
            }
            else if (motorState == RIGHTD) {
                motors_control(0.0f, 0.0f, 0.0f, 0.8f);

                if (distances.left <= treshDist.left + 0.1f)
                    motorState = STRAIGHT;

                // set_pwm(right_pwmPD6, 0.0f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.8f);
            }
        }
    }
    return 0;
}

/*
    This is the main user software application
    In this case, we have below the code necessary to toggle a set of LEDs on a STM32 F3 Discovery PCB at 20 Hz.

    (c) Abhimanyu Ghosh, 2017
 */

// #include "cpu.h"
// #include "pwm_hal.h"
// #include "sensor_hal.h"
// #include "general.h"

// #include "foo.h"

// int main()
// {
//     /*
//      Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
//      */
//     cpu_init();
//     /*
//      Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
//      */
    
//     echosPins.right = GPIO_PIN_11;
//     echosPins.front = GPIO_PIN_6;
//     echosPins.left = GPIO_PIN_9;
    
//     triggerPins.right = GPIO_PIN_10;
//     triggerPins.front = GPIO_PIN_0;
//     triggerPins.left = GPIO_PIN_0;
    
//     extiRet.front = STRAIGHT;
//     extiRet.right = STRAIGHT;
//     extiRet.left = STRAIGHT;
    
//     distances.right = 0.0f;
//     distances.left = 0.0f;
    
//     firstSense.left = 0;
//     firstSense.right = 0;
    
//     treshDist.left = 16.0f;
//     treshDist.right = 16.0f;
    
//     init_pwm();
//     init_triggers();
//     init_echos();
//     init_timers();
//     int i = 0;
    
//     init_tresh_dist();
    
//     // Forward
//     set_pwm(right_pwmPD6, 0.6f);
//     set_pwm(right_pwmPD7, 0.0f);
//     set_pwm(left_pwmPD3, 0.0f);
//     set_pwm(left_pwmPD4, 0.6f);
    
//     cpu_sw_delay(70);
    
//     // Stop
//     set_pwm(right_pwmPD6, 0.0f);
//     set_pwm(right_pwmPD7, 0.0f);
//     set_pwm(left_pwmPD3, 0.0f);
//     set_pwm(left_pwmPD4, 0.0f);
    
//     cpu_sw_delay(10);
    
//      // Backwards
//      set_pwm(right_pwmPD6, 0.0f);
//      set_pwm(right_pwmPD7, 0.6f);
//      set_pwm(left_pwmPD3, 0.6f);
//      set_pwm(left_pwmPD4, 0.0f);
     
//      cpu_sw_delay(10);
     
//     // Left
//     set_pwm(right_pwmPD6, 0.0f);
//     set_pwm(right_pwmPD7, 1.0f);
//     set_pwm(left_pwmPD3, 0.5f);
//     set_pwm(left_pwmPD4, 0.0f);
    
//     cpu_sw_delay(60);
    
//     // Backwards
//     set_pwm(right_pwmPD6, 0.0f);
//     set_pwm(right_pwmPD7, 0.6f);
//     set_pwm(left_pwmPD3, 0.6f);
//     set_pwm(left_pwmPD4, 0.0f);
    
//     cpu_sw_delay(80);
    
//     // Right
//     set_pwm(right_pwmPD6, 0.0f);
//     set_pwm(right_pwmPD7, 0.3f);
//     set_pwm(left_pwmPD3, 0.6f);
//     set_pwm(left_pwmPD4, 0.0f);
    
//     cpu_sw_delay(50);
    
//     // Stop
//     set_pwm(right_pwmPD6, 0.0f);
//     set_pwm(right_pwmPD7, 0.0f);
//     set_pwm(left_pwmPD3, 0.0f);
//     set_pwm(left_pwmPD4, 0.0f);
    
//     cpu_sw_delay(80);
    
//     // Left
//     set_pwm(right_pwmPD6, 0.8f);
//     set_pwm(right_pwmPD7, 0.0f);
//     set_pwm(left_pwmPD3, 0.0f);
//     set_pwm(left_pwmPD4, 0.5f);
    
//     cpu_sw_delay(50);
    
//     // Stop
//     set_pwm(right_pwmPD6, 0.0f);
//     set_pwm(right_pwmPD7, 0.0f);
//     set_pwm(left_pwmPD3, 0.0f);
//     set_pwm(left_pwmPD4, 0.0f);
    
    
//     /*
//      //In an infinite loop, keep toggling the LEDs in an alternating pattern:
//      while(1)
//      {
//      //Carry out a simple unit test of foo() declared in foo.h:
//      if(TEST_FOO(i, i+1) < 0)
//      {
//      //If the above fails there is either a hardware, code or other undefined error.
//      //Now we're in an undefined state regarding processor core behavior...
//      while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
//      }
//      else
//      {
//      board_led_on(LED1);
//      board_led_off(LED2);
     
//      cpu_sw_delay(50U);  // Invoke a simple software busy-wait routine to delay approximately 50 milliseconds
     
//      board_led_off(LED1);
//      board_led_on(LED2);
     
//      cpu_sw_delay(50U);
     
//      ++i; // Increment i for the next test iteration...
//      }
//      }
//      */
//     return 0;
// }
