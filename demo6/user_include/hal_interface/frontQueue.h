#ifndef FRONT_QUEUE_H_
#define FRONT_QUEUE_H_	1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sensor_hal.h"
#include "general.h"

#define FRONTQUEUEMAX 3

float peek();
bool isEmpty();
bool isFull();
int size();
void insert(float data);
float removeData();
void init_queue();
float average();

#endif