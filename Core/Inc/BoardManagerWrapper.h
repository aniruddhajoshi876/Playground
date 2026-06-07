/*
 * BoardManagerWrapper.h
 *
 *  Created on: Apr 4, 2026
 *      Author: aniru
 */

#ifndef INC_BOARDMANAGERWRAPPER_H_
#define INC_BOARDMANAGERWRAPPER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif
void* bm_create();
void bm_execute(void* bm);
void pwm_brightness(void* bm, volatile int brightness);
void bm_state_manager(void* bm);
void bm_timer_manager(void (*method_ptr)(void*),uint32_t time);
void bm_init(void* bm);



void bm_adc(void* bm);
void low_priority_task();
#ifdef __cplusplus
}
#endif


#endif /* INC_BOARDMANAGERWRAPPER_H_ */
