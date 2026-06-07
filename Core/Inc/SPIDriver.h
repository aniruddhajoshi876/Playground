/*
 * SPIDriver.h
 *
 *  Created on: Apr 23, 2026
 *      Author: aniru
 */

#ifndef INC_SPIDRIVER_H_
#define INC_SPIDRIVER_H_

#include "spi.h"

class SPIDriver{
public:
	SPIDriver(SPI_HandleTypeDef* spi);
	bool read(uint8_t* buf, uint16_t len);
	bool write(uint8_t* data, uint16_t len);
	bool transfer(uint8_t* tx, uint8_t* rx, uint16_t len);


	bool readDMA(uint8_t* buf , uint16_t len);
	bool writeDMA(uint8_t* data, uint16_t len);
	bool transferDMA(uint8_t* tx, uint8_t* rx, uint16_t len);
	bool isBusy();

	void onTxRxComplete(SPI_HandleTypeDef* hspi);
private:
	SPI_HandleTypeDef* spi;
	volatile bool dmaBusy = false;
	volatile bool dmaError = false;
};



#endif /* INC_SPIDRIVER_H_ */
