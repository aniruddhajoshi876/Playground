/*
 * IOPinwrapper.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: aniru
 */

#include "IOPinwrapper.hpp"

IOPin_wrapper::IOPin_wrapper(IOPin& led)
	:led(led)
{
	led.init();
}

void IOPin_wrapper::toggle_led(){
	led.toggle();
}


