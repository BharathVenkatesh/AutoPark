#include "encoder_hal.h"
#include "cpu.h"
int count = 0;
int lock_right; //Semaphore for the right encoder
int lock_left;  //Semaphore for the left encoder

void init_encoders(void) {
	static GPIO_InitTypeDef  Encoders;

	lock_right = 1;
	lock_left = 1;	
	
	Encoders.Pin = encodersPins.right | encodersPins.left;
	Encoders.Mode = GPIO_MODE_IT_RISING;
	Encoders.Pull = GPIO_PULLDOWN;
	Encoders.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &Encoders);
	
	HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI4_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(encodersPins.left);
}

void EXTI3_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(encodersPins.right);
}

//Encoder Interrupt Callback
void encoders_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == encodersPins.right) {
		HAL_NVIC_DisableIRQ(EXTI3_IRQn);  //disable interrupt for the right encoder		
		enc_counter.left++;
		printf("counter %ld\n", enc_counter.left);
		if (lock_right == 1) {            
			lock_right = 0;          //get the right encoder lock
			for(int k = 0; k< 3000; k++){
				k++;
				k--;            //junk instructions to force the compiler not to optimize this loop
				if(k == 2990){
					encoders_distances.right++;  //increase the distance for the right encoder and exit he loop
					printf("distance = %ld\n", encoders_distances.right);
					break;
				}
				else
					continue;
			}
			HAL_NVIC_EnableIRQ(EXTI3_IRQn); //reenable right encoder interrupt
			enc_counter.right = 0;       
			lock_right = 1;               //release the lock
		}
		else
			return;
	}
	else if (GPIO_Pin == encodersPins.left) {
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);	//disable interrupt for the left encoder	
		enc_counter.left++;
		printf("counter %ld\n", enc_counter.left);		
		if (lock_left == 1) {
			lock_left = 0;        //get the left encoder lock

			for(long int j = 0; j< 3000; j++){
			j++;
			j--;	//junk instructions to force compiler not to optimize this loop
				if(j == 2990){
					encoders_distances.left++;   //incerase left encoder distance and exit the loop
					printf("distance = %ld\n", encoders_distances.left);
					break;
				}
			}	
			HAL_NVIC_EnableIRQ(EXTI4_IRQn);  //reenable left encoder interrupt
			lock_left = 1;                   //release the lock
		}
		else{
			printf("salamaki");
			return;
		}
	}
}
