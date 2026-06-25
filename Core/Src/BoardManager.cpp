/*
 * BoardManager.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: aniru
 */

#include "BoardManager.h"

#include "main.h"


extern volatile int ticks;
extern volatile int light;


BoardManager::BoardManager()
	:led(GPIOA, 5), led_wrapper(led), last_toggle(0), state(Dot)
{}

void BoardManager::init(){
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; //enable tim2

	TIM2->PSC = 15999;
	TIM2->ARR = 499;
	TIM2->CNT=0;

	TIM2->EGR|=TIM_EGR_UG;    //initialzes psc,arr,and cnt config right away
	TIM2->SR &= ~TIM_SR_UIF;  //clear pending update flag set by EGR, UIF= Update Interupt Flag
	TIM2->DIER|=TIM_DIER_UIE; //generate interupt when arr overflows

	NVIC_EnableIRQ(TIM2_IRQn); //jump to tim2irq_handler

	TIM2->CR1 |= TIM_CR1_CEN; //start the timer
}


void BoardManager::timer_manager(uint32_t time){
	TIM2->ARR=time;

	}


void BoardManager::execute(){
	//if (prev - last_toggle >= 3000){
	led_wrapper.toggle_led();
	//last_toggle = prev;
	}


void BoardManager::state_manager(){
	if (ticks == 0){
		switch(state){
			case Dot:
				state = Gap;
				led_wrapper.toggle_led();
				ticks = 100;
				break;
			case Gap:
				state=Dash;
				led_wrapper.toggle_led();
				ticks=300;
				break;
			case Dash:
				state = Pause;
				led_wrapper.toggle_led();
				ticks = 1000;
				break;
			case Pause:
				state=Dot;
				led_wrapper.toggle_led();
				ticks = 100;
				break;
		}
	}
}

void BoardManager::read_adc(){
	  {
		  //adc_ready=0;

		  printf("At %ld\r\n", HAL_GetTick());
		  float temperature = raw_to_temp(adc_buf[0]);
		  printf("temp: %.2f\r\n", temperature);
		  printf("vrefint: %.2fV, vbat: %.2fV\r\n", (adc_buf[1] * 3.3f) / 4096.0f, (adc_buf[2] * 3.3f) / 4096.0f * 3.0f);
		  printf("High Priority Task");
		  printf("\r\n");
	  }
}
