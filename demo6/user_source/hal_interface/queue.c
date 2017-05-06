#include "queue.h"

#define MAX 5

float intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;
float sum = 0;

float peek() {
   return intArray[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void insert(float data) {

   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      sum += data;

      intArray[++rear] = data;
      itemCount++;
   }
}

float removeData() {
   float data = intArray[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   sum -= data;

   itemCount--;
   return data;
}

void init_queue() {
   int count = 0;

   queue_init = 1;

   while (count < 5) {
      if (front_triggered == 0) {
         count++;
         front_triggered = 1;

         trigger_sensor(GPIOD, triggerPins.front);
         // HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_SET);
         // // Delay to simulate 10us pulse
         // cpu_sw_delay_us(10);
         // HAL_GPIO_WritePin(GPIOD, triggerPins.front, GPIO_PIN_RESET);
      }
   }
   queue_init = 0;
}

float average() {
   return sum/MAX;
}