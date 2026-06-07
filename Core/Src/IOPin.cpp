/*
 * IOPin.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: aniru
 */

#include "IOPin.hpp"

IOPin::IOPin(GPIO_TypeDef *port, uint16_t pin)
	:port(port), pin(pin)
{
	}

void IOPin::init(){
	//enable clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //0b00000000 -> 0b00000001
	//ignore hardcoding of enabling gpio A.


	port->MODER &= ~(0b11 << (pin * 2)); //clears bits, resets to input.0bxxxxddxxxxxxxxxx -> 0bxxxx00xxxxxxxxxx
	port->MODER |= (1<< (pin * 2)); //set mode to output
}
void IOPin::toggle(){
	if (port->ODR & (1<<pin)){ //if the bit is high, reset
		port->BSRR = (1 << (pin+16)); //set pin low
	}
	else{
		port->BSRR = (1<<pin); //set high
	}

}



