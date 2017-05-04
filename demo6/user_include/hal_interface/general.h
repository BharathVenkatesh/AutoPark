#ifndef GENERAL_H_
#define GENERAL_H_	1

#include "cpu.h"

typedef enum {
	STRAIGHT,
	STOP,
	LEFTD,
	RIGHTD
} motor_state;

enum {RIGHT, FRONT, LEFT};

motor_state motorState;

int right_triggered;
int front_triggered;
int left_triggered;

#endif