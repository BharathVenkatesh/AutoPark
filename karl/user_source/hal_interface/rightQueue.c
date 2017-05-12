#include "rightQueue.h"

float rightBuffer[RIGHTQUEUEMAX];
int frontR = 0;
int rearR = -1;
int itemCountR = 0;
float sumR = 0;

float peekRight() {
   return rightBuffer[frontR];
}

bool isEmptyRight() {
   return itemCountR == 0;
}

bool isFullRight() {
   return itemCountR == RIGHTQUEUEMAX;
}

int sizeRight(Right) {
   return itemCountR;
}  

void insertRight(float data) {

   if(!isFull()) {
	
      if(rearR == RIGHTQUEUEMAX-1) {
         rearR = -1;            
      }       

      sumR += data;

      rightBuffer[++rearR] = data;
      itemCountR++;
   }
}

float removeDataRight() {
   float data = rightBuffer[frontR++];
	
   if(frontR == RIGHTQUEUEMAX) {
      frontR = 0;
   }
	
   sumR -= data;

   itemCountR--;
   return data;
}

void init_queueRight() {
   int count = 0;


   for (count = 0; count < RIGHTQUEUEMAX; count++)
      rightBuffer[count] = 10.0f;

   sumR = 10.0f*RIGHTQUEUEMAX;
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

float averageRight() {
   return sumR/RIGHTQUEUEMAX;
}