#ifndef QUEUE_H_
#define QUEUE_H_	1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sensor_hal.h"
#include "general.h"

int queue_init;

float peek();

bool isEmpty();

bool isFull();

int size();

void insert(float data);

float removeData();

void init_queue();

float average();