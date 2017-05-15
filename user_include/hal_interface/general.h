#ifndef GENERAL_H_
#define GENERAL_H_	1

#include "cpu.h"
#include "math.h"

/*
#define NORMAL 0.6f
#define NORMAL1 0.8f
*/

/* PWM speeds */
float NORMAL;
float NORMAL1;

/* Possible motor states */
typedef enum {
	STRAIGHT,
	STOP,
	LEFTD,
	RIGHTD,
	BACK
} motor_state;

/* Sensors macros */
enum {RIGHT, FRONT, LEFT};

motor_state motorState;

/* Variables to trigger or not the sensors */
int right_triggered;
int front_triggered;
int left_triggered;

/* Indicates if searching for spot or not */
int searching;

/* Indicates if found spot or not */
int found;

/* Indicates if a delay is to be added or not */
int delay;

#endif
