#ifndef GENERAL_H_
#define GENERAL_H_	1

#include "cpu.h"
#include "math.h"

#define NORMAL 0.6f
#define NORMAL1 0.8f

typedef enum {
	STRAIGHT,
	STOP,
	LEFTD,
	RIGHTD
} motor_state;

enum {RIGHT, FRONT, LEFT};

motor_state motorState;
int turn_adjustment;

int right_triggered;
int front_triggered;
int left_triggered;
int searching;
int found;
int straight;

#endif