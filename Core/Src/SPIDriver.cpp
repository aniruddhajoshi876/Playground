/*
 * SPIDriver.cpp
 *
 *  Created on: Apr 25, 2026
 *      Author: aniru
 */

#include "SPIDriver.h"

SPIDriver::SPIDriver(SPI_HandleTypeDef* spi)
    : spi(spi)
{}

bool SPIDriver::read(uint8_t* buf, uint16_t len){
    return HAL_SPI_Receive(spi, buf, len, 100) == HAL_OK;
}

bool SPIDriver::write(uint8_t* data, uint16_t len){
    return HAL_SPI_Transmit(spi, data, len, 100) == HAL_OK;
}

bool SPIDriver::transfer(uint8_t* tx, uint8_t* rx, uint16_t len){
    return HAL_SPI_TransmitReceive(spi, tx, rx, len, 100) == HAL_OK;
}

bool SPIDriver::readDMA(uint8_t* buf, uint16_t len){
	if (dmaBusy){
			return false;
		}
	dmaBusy = true;
    return HAL_SPI_Receive_DMA(spi, buf, len) == HAL_OK;
}

bool SPIDriver::writeDMA(uint8_t* data, uint16_t len){
	if (dmaBusy){
			return false;
		}
	dmaBusy = true;
    return HAL_SPI_Transmit_DMA(spi, data, len) == HAL_OK;
}

bool SPIDriver::transferDMA(uint8_t* tx, uint8_t* rx, uint16_t len){
	if (dmaBusy){
		return false;
	}
	dmaBusy = true;
    return HAL_SPI_TransmitReceive_DMA(spi, tx, rx, len) == HAL_OK;
}

bool SPIDriver::isBusy(){
	return dmaBusy;
}

void SPIDriver::onTxRxComplete(SPI_HandleTypeDef* hspi)
{
    if (hspi != spi) return;

    dmaBusy = false;
}
