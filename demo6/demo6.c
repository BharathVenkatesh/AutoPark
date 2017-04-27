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
#include "board_led.h"
#include "pwm_hal.h"
#include "timers.h"

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
    init_pwm();
    //sensor_trigger_init();
    
    int i = 0;
    count = 0;
    
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
            set_pwm(right_pwmPD6, 1.0f);
            set_pwm(right_pwmPD7, 0.0f);
            set_pwm(left_pwmPD3, 0.0f);
            set_pwm(left_pwmPD4, 1.0f);
            
            //set_sensor();
        }
    }
    
    return 0;
}
