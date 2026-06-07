/*
 * IOPinwrapper.hpp
 *
 *  Created on: Apr 4, 2026
 *      Author: aniru
 */

#ifndef INC_IOPINWRAPPER_HPP_
#define INC_IOPINWRAPPER_HPP_
#include "IOPin.hpp"

class IOPin_wrapper{
public:
	IOPin_wrapper(IOPin& led);
	void toggle_led();
private:
	IOPin& led;
};




#endif /* INC_IOPINWRAPPER_HPP_ */
