/*
 * BoardManager.h
 *
 *  Created on: Apr 4, 2026
 *      Author: aniru
 */

#ifndef INC_BOARDMANAGER_H_
#define INC_BOARDMANAGER_H_

#include "IOPinwrapper.hpp"
#include <cstdio>

#define TS_CAL1  (*((volatile uint16_t*)0x1FFF75A8U))
#define TS_CAL2  (*((volatile uint16_t*)0x1FFF75CAU))

extern volatile uint32_t adc_raw;
extern volatile uint32_t adc_ready;
extern volatile uint16_t adc_buf[3];

enum State{
	Dot,
	Gap,
	Dash,
	Pause
};

inline float raw_to_temp(uint32_t raw)
{
    float corrected = (float)raw * (3.3f / 3.0f);
    return 100.0f * (corrected - TS_CAL1) / (TS_CAL2 - TS_CAL1) + 30.0f;
}

class BoardManager{
public:
	BoardManager();
	void init();
	void timer_manager(uint32_t time);
	void execute();
	void get_PWM(uint32_t buf[2]);
	void change_brightness(volatile int brightness);
	void state_manager();

	void read_adc();


private:
	IOPin led;
	IOPin_wrapper led_wrapper;
	uint16_t last_toggle;
	static uint32_t buf[2];
	State state;


};




#endif /* INC_BOARDMANAGER_H_ */
