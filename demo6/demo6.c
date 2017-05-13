// C standard include(s):
#include <math.h>
#include <stdio.h>

// Custom user APIs needed for hardware access specific to this board:
#include "cpu.h"
#include "pwm_hal.h"
#include "sensor_hal.h"
#include "general.h"
#include "uart.h"
#include "frontQueue.h"
#include "rightQueue.h"
#include "encoder_hal.h"
#include "board_led.h"

// // Custom user APIs needed for generic algorithmic libraries that are hardware-independent:
#include "foo.h"

void parallel_park() {
    while (encoders_distances.left < 30)
        motors_control(0.3f, 0.0f, 0.0f, 1.0f);

    motors_control(0.0f,0.0f, 0.0f,0.0f);
    cpu_sw_delay(100U);
    encoders_distances.right = 0;
    encoders_distances.left = 0;

/*
    while (encoders_distances.left < 20)
        motors_control(0.0f, NORMAL, NORMAL, 0.0f);
*/
	while (encoders_distances.right < 10)
		motors_control(0.0f, 0.3, NORMAL, 0.0f);

    motors_control(0.0f,0.0f, 0.0f,0.0f);

    // while (encoders_distances.right < 25/*100*/ || encoders_distances.left < 25/*100*/)
    //     motors_control(NORMAL, 0.0f, 0.0f, NORMAL);

    // motors_control(0.0f,0.0f, 0.0f,0.0f);
    // cpu_sw_delay(100U);
    // // motors_control(0.0f,0.6f, 0.6f,0.0f);
    // // cpu_sw_delay(50U);
    // encoders_distances.right = 0;
    // encoders_distances.left = 0;

    // while (encoders_distances.right < 11/*100*/ || encoders_distances.left < 11/*100*/)
    //     motors_control(0.0f, 1.0f, 1.0f, 0.0f);

    // encoders_distances.right = 0;
    // encoders_distances.left = 0;

    // while (/*encoders_distances.right < 10100 ||*/ encoders_distances.left < 15/*100*/)
    //     motors_control(0.0f, 0.3f, 1.0f, 0.0f);

    // encoders_distances.right = 0;
    // encoders_distances.left = 0;

    // while (encoders_distances.right < 6/*100*/ || encoders_distances.left < 6/*100*/)
    //     motors_control(0.0f, 1.0f, 1.0f, 0.0f);

    // encoders_distances.right = 0;
    // encoders_distances.left = 0;

    // while (/*encoders_distances.right < 6100 ||*/ encoders_distances.left < 15/*100*/)
    //     motors_control(0.0f, 1.0f, 0.3f, 0.0f);

    // encoders_distances.right = 0;
    // encoders_distances.left = 0;

    // //motors_control(0.0f, NORMAL, NORMAL, 0.0f);
}

void perpendicular_park() {
	//Forward
    while (encoders_distances.right < 56/*100*/ || encoders_distances.left < 56/*100*/)
        motors_control(NORMAL, 0.0f, 0.0f, NORMAL1);

	//Stop
    motors_control(0.0f,0.0f, 0.0f,0.0f);
    cpu_sw_delay(100U);
    // motors_control(0.0f,0.6f, 0.6f,0.0f);
    // cpu_sw_delay(50U);
    encoders_distances.right = 0;
    encoders_distances.left = 0;

	//Backup
    while (encoders_distances.right < 15/*100*/ || encoders_distances.left < 15/*100*/)
        motors_control(0.0f, 1.0f, 1.0f, 0.0f);

    encoders_distances.right = 0;
    encoders_distances.left = 0;

	//Reverse right turn
    while (/*encoders_distances.right < 10100 ||*/ encoders_distances.left < 90/*100*/)
        motors_control(0.0f, 0.3f, 1.0f, 0.0f);

    motors_control(0.0f, NORMAL, NORMAL, 0.0f);
	cpu_sw_delay(20U);

    encoders_distances.right = 0;
    encoders_distances.left = 0;

    distances.right = 30.0f;

    //motors_control(0.0f,0.0f,0.0f,0.0f);
}

void init_sensors_values() {
    right_triggered = 0;
    front_triggered = 0;
    left_triggered = 0;

    distances.right = 0.0f;
    distances.left = 0.0f;

    // firstSense.left = 0;
    // firstSense.right = 0;

    treshDist.left = 10.0f;
    treshDist.right = 10.0f;

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

    encodersPins.right = GPIO_PIN_3;
    encodersPins.left = GPIO_PIN_4;
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
    //printf("Hello World!!\r\n");

    /* Initialize motor state */
    motorState = STOP;
    searching = 1;
    // straight = 0;
    // found = 0;

    /* Initialize pwm to control motors */
    init_pwm();

    init_pins();

    /* Initialize sensors */
    init_sensors_values();
    init_triggers();
    init_echos();
    init_timers();
    // init_tresh_dist();
    init_queue(); // Initialize sensor value averaging
    init_queueRight();
    // cpu_sw_delay(1000U);
    // motorState = STRAIGHT;

    /* Initialize encoders */
    init_encoders();
    encoders_distances.left = 0;
    encoders_distances.right = 0;
    enc_counter.right = 0;
    enc_counter.left = 0;

    // turn_adjustment = 0;
    // enc_diff = 0;
    int firstTurn = 0;
    int count = 0;
    int countRight = 0;
    int turned_left = 0;
    delay = 0;
    found = 0;
    // float wall = 0.0f;

    /* Initialize leds for debugging */
    board_led_init();

    int i = 0;

    // motors_control(NORMAL, 0.0f, 0.0f, NORMAL);
    // cpu_sw_delay(50U);

    // encoders_distances.left = 0;

    // while (encoders_distances.left < 130)
    //     motors_control(0.3f, 0.0f, 0.0f, NORMAL);

    // motors_control(0.0f, 0.0f, 0.0f, 0.0f);

    while(1)
    {
        //continue;
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
            /* Car movement */
            // if (extiRet.front == STOP || extiRet.right == STOP || extiRet.left == STOP)
            //     motorState = STOP;
            // if (extiRet.front == STRAIGHT && extiRet.right == STRAIGHT && extiRet.left == STRAIGHT)
            //     motorState = STRAIGHT;

            if (motorState == STRAIGHT) {
                board_led_on(LED6);
                board_led_off(LED9);
                board_led_off(LED8);
               // printf("front: %f\n", distances.front);
               // printf("right: %f\n", distances.right);
               // printf("straight\n");
                //printf("%ld\n", encoders_distances.right);
                /* Make car go forward */
               // printf("STRAIGHT////////////////////////////////////////////////////////////////////////\n");
                // motors_control(NORMAL, 0.0f, 0.0f, NORMAL);
                // if (encoders_distances.right >= 20)
                //     motorState = STOP;
                adjust();

                // if (encoders_distances.left >= 20)
                //     motorState = STOP;
                if (searching == 1) {
                    if (delay == 1) {
                        if (encoders_distances.right >= 5/*100*/) {
                            delay = 0;
                            turned_left = 1;
                        }
                    } else {
                        if (distances.right > 70.0f) {
                            if (firstTurn == 0) {
                                encoders_distances.right = 0;
                                firstTurn = 1;
                            }

                            if (turned_left == 1) {
                            // if (firstTurn == 0) {
                            //     encoders_distances.right = 0;
                            //     firstTurn = 1;
                            // }
                            //if (encoders_distances.right >= 90) {
                                motorState = RIGHTD;
                                searching = 0;
                                encoders_distances.left = 0;
                                encoders_distances.right = 0;
                            } else {
                                // turn_adjustment = 0;
                                if (encoders_distances.right >= 5) {
                                    motorState = RIGHTD;
                                    searching = 0;
                                    encoders_distances.left = 0;
                                    encoders_distances.right = 0; 
                                }
                            }
                           // }
                        }
                        else if (distances.right > 25.0f) {
                            searching = 0;
                        }
                    }
                } 
                // // else if (straight == 1) {
                // //     if (encoders_distances.left >= 50 || encoders_distances.right >= 50)
                // //         straight = 0;

                // //     wall = treshDist.right;
                // // }
                // // else if (straight == 0 && searching == 0) {
                // //     if (distances.right > treshDist.right + 10.0f) {
                // //         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
                // //         found = 1;
                // //         encoders_distances.right = 0;
                // //         encoders_distances.left = 0;
                // //     }
                // //     else if ((found == 1) && (distances.right <= wall + 5.0f) && (encoders_distances.right >= 100 || encoders_distances.left >= 100))
                // //         motorState = STOP;
                // // }
                else {
                    //if (encoders_distances.left >= /*350*/25){// && encoders_distances.right >= /*350*/20 /*&& distances.right < 25.0f*/) {
                    if (encoders_distances.left >= 30/*75*//* && (distances.right <= treshDist.right + 5.0f && distances.right >= treshDist.right - 5.0f)*/) {
                        if (found == 0 && distances.right > 25.0f)
                            found = 1;
                        else if (found == 1 && distances.right < 25.0f) {
                            encoders_distances.right = 0;
                            encoders_distances.left = 0;
                            perpendicular_park();
                            //par_park = 1;
                            //parallel_park();
                           // break;
                            count = FRONTQUEUEMAX + 1;
                            motorState = BACK;
                        }
                    }
                }
                
                // if (encoders_distances.right >= 200 && encoders_distances.right <= 203)
                //     motorState = STOP;
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
                //printf("Stop\n");
                motors_control(0.0f,0.0f,0.0f,0.0f);

                board_led_off(LED6);
                board_led_on(LED9);
                board_led_off(LED8);

                if (count == FRONTQUEUEMAX && countRight == RIGHTQUEUEMAX)
                    motorState = STRAIGHT;

                // enc_diff = abs(encoders_distances.right - encoders_distances.left);

                // if (enc_diff >= 50) {
                //     turn_adjustment = 1;
                //     if (encoders_distances.left > encoders_distances.right)
                //         motorState = LEFTD;
                //     else motorState = RIGHTD;
                // }

                // if (encoders_distances.left < encoders_distances.right/* - 20*/) {
                //     turn_adjustment = 1;
                //     motorState = RIGHTD;
                //     // enc_diff = encoders_distances.right - encoders_distances.left;
                //     // encoders_distances.right = 0;
                //     // encoders_distances.left = 0;
                // }
                // else if (encoders_distances.right < encoders_distances.left/* - 20*/) {
                //     turn_adjustment = 1;
                //     motorState = LEFTD;
                //     // enc_diff = encoders_distances.left - encoders_distances.right;
                //     // encoders_distances.right = 0;
                //     // encoders_distances.left = 0;
                // }
                // else {
                //     turn_adjustment = 0;
                //     encoders_distances.right = 0;
                //     encoders_distances.left = 0;
                //     motorState = LEFTD;
                // }

                // if (distances.left > distances.right)
                //     motorState = LEFTD;
                // else motorState = RIGHTD;

                // set_pwm(right_pwmPD6, 0.0f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.0f);

                // if (distances.left > distances.right)
                //     motorState = LEFTD;
                // else motorState = RIGHTD;
            }
            else if (motorState == LEFTD) {
                //printf("left\n");
               // printf("%f\n", distances.front);
                board_led_off(LED6);
                board_led_off(LED9);
                board_led_on(LED8);

               // printf("left: %f\n", distances.front);


                // printf("left\n");
                // printf("LEFT////////////////////////////////////////////////////////////////////////\n");

                // enc_diff = abs(encoders_distances.right - encoders_distances.left);

                // if (turn_adjustment == 1) {
                //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                //     //if (encoders_distances.right == encoders_distances.left/*>= enc_diff*//*encoders_distances.left - 5 && encoders_distances.right <= encoders_distances.left + 5*/) {
                //     //     motorState = STOP;
                //     //     enc_diff = 0;
                //     // }
                //     if (enc_diff <= 20) {
                //         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                //         motorState = STOP;
                //     }
                // }
                // else if (turn_adjustment == 0) {
                    if (encoders_distances.right >= 62/*75*/ && (distances.right <= treshDist.right + 5.0f && distances.right >= treshDist.right - 5.0f)) {
                        motorState = STRAIGHT;
                        delay = 1;
                        distances.right = 0;
                        encoders_distances.right = 0;
                        encoders_distances.left = 0;
                    }

                // if (encoders_distances.right >= 20) {
                //         motorState = STOP;
                //     }
                // }

                motors_control(NORMAL, 0.0f, 0.0f, 0.0f);

                //cpu_sw_delay(100U);

                // if (distances.right <= treshDist.right + 0.1f)
                //     motorState = STRAIGHT;

                // set_pwm(right_pwmPD6, 0.8f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.0f);

                // cpu_sw_delay(100U);
                // motorState = STRAIGHT;
            }
            else if (motorState == RIGHTD) {
               // printf("right\n");
                board_led_on(LED6);
                board_led_off(LED9);
                board_led_on(LED8);

                // printf("right\n");
                // enc_diff = abs(encoders_distances.right - encoders_distances.left);

                // if (turn_adjustment == 1) {
                //     motors_control(0.0f, 0.0f, 0.0f, NORMAL);

                //     //if (encoders_distances.left == encoders_distances.right /*>= enc_diff*/ /*>= encoders_distances.right - 5 && encoders_distances.left <= encoders_distances.right + 5*/) {} 
                //         // motorState = STOP;
                //         // enc_diff = 0;

                //     if (enc_diff <= 20) {
                //         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
                //         motorState = STOP;
                //     }
                // } else {
                    motors_control(0.3f, 0.0f, 0.0f, NORMAL);

                    if (encoders_distances.left > 130 && (distances.right <= treshDist.right + 7.5f && distances.right >= treshDist.right - 2.5f)) {
                        motorState = STRAIGHT;
                        // straight = 1;
                        encoders_distances.left = 0;
                        encoders_distances.right = 0;
                    }
                // }

                // cpu_sw_delay(100U);

                // motorState = STRAIGHT;

                // if (distances.left <= treshDist.left + 0.01f)
                //     motorState = STRAIGHT;

                // set_pwm(right_pwmPD6, 0.0f);
                // set_pwm(right_pwmPD7, 0.0f);
                // set_pwm(left_pwmPD3, 0.0f);
                // set_pwm(left_pwmPD4, 0.8f);
            }
            else if (motorState == BACK) {
                motors_control(0.0f, NORMAL, NORMAL, 0.0f);

                if (distances.right <= 25.4f) {
                    motors_control(0.0f, 0.0f, 0.0f, 0.0f);
                    cpu_sw_delay(100U);
                    //parallel_park();
                    motorState = STOP;
                }
            }

            /* Trigger sensors */
            if (front_triggered == 0) {
                if (count < FRONTQUEUEMAX)
                    count++;

                front_triggered = 1;
                trigger_sensor(GPIOD, triggerPins.front);
                // HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_SET);
                // // Delay to simulate 10us pulse
                // cpu_sw_delay_us(10);
                // HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_RESET);
            }
            if (right_triggered == 0) {
                if (countRight < RIGHTQUEUEMAX)
                    countRight++;

                right_triggered = 1;
                trigger_sensor(GPIOE, triggerPins.right);
                // HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_SET);
                // // Delay to simulate 10us pulse
                // cpu_sw_delay_us(10);
                // HAL_GPIO_WritePin(GPIOE, triggerPins.right, GPIO_PIN_RESET);
            }
            // if (left_triggered == 0) {
            //     left_triggered = 1;
            //     trigger_sensor(GPIOB, triggerPins.left);
            //     // HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_SET);
            //     // // Delay to simulate 10us pulse
            //     // cpu_sw_delay_us(10);
            //     // HAL_GPIO_WritePin(GPIOB, triggerPins.left, GPIO_PIN_RESET);
            // }

            //parallel_park();
        }
    }
    return 0;
}

/*
    This is the main user software application
    In this case, we have below the code necessary to toggle a set of LEDs on a STM32 F3 Discovery PCB at 20 Hz.

    (c) Abhimanyu Ghosh, 2017
 */

// int main()
// {
//     /*
//      Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
//      */
//     cpu_init();
//     /*
//      Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
//      */
    
//     init_pwm();
//     encodersPins.right = GPIO_PIN_3;
//     encodersPins.left = GPIO_PIN_4;
//     init_encoders();
//     encoders_distances.left = 0;
//     encoders_distances.right = 0;

//     while (encoders_distances.right < 50 || encoders_distances.left < 50)
//         motors_control(NORMAL, 0.0f, 0.0f, NORMAL);
    
//     //cpu_sw_delay(70);
    
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
