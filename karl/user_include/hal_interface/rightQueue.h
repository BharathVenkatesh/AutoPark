#ifndef RIGHT_QUEUE_H_
#define RIGHT_QUEUE_H_	1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sensor_hal.h"
#include "general.h"

#define RIGHTQUEUEMAX 3

// int queue_init;

float peekRight();
bool isEmptyRight();
bool isFullRight();
int sizeRight();
void insertRight(float data);
float removeDataRight();
void init_queueRight();
float averageRight();

#endif