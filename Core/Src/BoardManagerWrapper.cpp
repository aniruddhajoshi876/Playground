/*
 * BoardManagerWrapper.c
 *
 *  Created on: Apr 4, 2026
 *      Author: aniru
 */
#include "BoardManagerWrapper.h"
#include "BoardManager.h"
#include <cstdio>

void (*stored_fn)(void* bm);
extern void* bm;

void* bm_create(){
	return new BoardManager();
}


void bm_execute(void* bm){
	return static_cast<BoardManager*>(bm)->execute();
}

void bm_pwm(void* bm, uint32_t buf[2]){
	return static_cast<BoardManager*>(bm)->get_PWM(buf);
}

void bm_state_manager(void* bm){
	return static_cast<BoardManager*>(bm)->state_manager();
}

void bm_init(void* bm){
	return static_cast<BoardManager*>(bm)->init();
}

void bm_timer_manager(void (*method_ptr)(void* bm), uint32_t time){
	stored_fn = method_ptr;
	return static_cast<BoardManager*>(bm)->timer_manager(time);
}

void bm_adc(void* bm){
	return static_cast<BoardManager*>(bm)->read_adc();
}


void low_priority_task(){
	printf("Low priority Task at %ld\r\n", HAL_GetTick());
}
