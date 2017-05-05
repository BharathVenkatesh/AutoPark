/*
	This is the main user software application
	In this case, we have below the code necessary to toggle a set of LEDs on a	STM32 F3 Discovery PCB at 20 Hz.

	(c) Abhimanyu Ghosh, 2017
 */

//#include "cpu.h"
//#include "board_led.h"

#include "cpu.h"
#include "pwm_hal.h"
#include "sensor_hal.h"
#include "general.h"

#include "foo.h"

int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */

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

	// Forward
	set_pwm(right_pwmPD6, 0.6f);
	set_pwm(right_pwmPD7, 0.0f);
	set_pwm(left_pwmPD3, 0.0f);
	set_pwm(left_pwmPD4, 0.6f);

	cpu_sw_delay(100);

	// Stop
	set_pwm(right_pwmPD6, 0.0f);
	set_pwm(right_pwmPD7, 0.0f);
	set_pwm(left_pwmPD3, 0.0f);
	set_pwm(left_pwmPD4, 0.0f);

	cpu_sw_delay(10);
/*
	// Backwards
	set_pwm(right_pwmPD6, 0.0f);
	set_pwm(right_pwmPD7, 0.6f);
	set_pwm(left_pwmPD3, 0.6f);
	set_pwm(left_pwmPD4, 0.0f);

	cpu_sw_delay(80);
*/
	// Left
	set_pwm(right_pwmPD6, 0.0f);
	set_pwm(right_pwmPD7, 0.8f);
	set_pwm(left_pwmPD3, 0.5f);
	set_pwm(left_pwmPD4, 0.0f);

	cpu_sw_delay(80);

	// Backwards
	set_pwm(right_pwmPD6, 0.0f);
	set_pwm(right_pwmPD7, 0.6f);
	set_pwm(left_pwmPD3, 0.6f);
	set_pwm(left_pwmPD4, 0.0f);

	cpu_sw_delay(80);

	// Right
	set_pwm(right_pwmPD6, 0.0f);
	set_pwm(right_pwmPD7, 0.3f);
	set_pwm(left_pwmPD3, 0.6f);
	set_pwm(left_pwmPD4, 0.0f);

	cpu_sw_delay(80);

	// Stop
	set_pwm(right_pwmPD6, 0.0f);
	set_pwm(right_pwmPD7, 0.0f);
	set_pwm(left_pwmPD3, 0.0f);
	set_pwm(left_pwmPD4, 0.0f);



/*
  //In an infinite loop, keep toggling the LEDs in an alternating pattern:
  while(1)
  {
    //Carry out a simple unit test of foo() declared in foo.h:
    if(TEST_FOO(i, i+1) < 0)
    {
        //If the above fails there is either a hardware, code or other undefined error.
        //Now we're in an undefined state regarding processor core behavior...
      while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
    }
    else
    {
      board_led_on(LED1);
      board_led_off(LED2);

      cpu_sw_delay(50U);  // Invoke a simple software busy-wait routine to delay approximately 50 milliseconds

      board_led_off(LED1);
      board_led_on(LED2);

      cpu_sw_delay(50U);

      ++i; // Increment i for the next test iteration...
    }
  }
*/
  return 0;
}
