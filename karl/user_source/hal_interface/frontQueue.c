#include "frontQueue.h"

float frontBuffer[FRONTQUEUEMAX];
int front = 0;
int rear = -1;
int itemCount = 0;
float sum = 0;

float peek() {
   return frontBuffer[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == FRONTQUEUEMAX;
}

int size() {
   return itemCount;
}  

void insert(float data) {

   if(!isFull()) {
	
      if(rear == FRONTQUEUEMAX-1) {
         rear = -1;            
      }       

      sum += data;

      frontBuffer[++rear] = data;
      itemCount++;
   }
}

float removeData() {
   float data = frontBuffer[front++];
	
   if(front == FRONTQUEUEMAX) {
      front = 0;
   }
	
   sum -= data;

   itemCount--;
   return data;
}

void init_queue() {
   int count = 0;


   for (count = 0; count < FRONTQUEUEMAX; count++)
      frontBuffer[count] = 30.0f;

   sum = 30.0f*FRONTQUEUEMAX;
   // queue_init = 1;

   // while (count < 5) {
   //    if (front_triggered == 0) {
   //       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
   //       front_triggered = 1;
   //       printf("trigerred: %d\n", front_triggered);
   //       printf("count: %d\n", count);
   //       count++;

   //       trigger_sensor(GPIOD, triggerPins.front);
   //    }
   // }
   /* Wait for last echo to come back */
   //while (front_triggered == 1);

   // queue_init = 0;
}

float average() {
   return sum/FRONTQUEUEMAX;
}