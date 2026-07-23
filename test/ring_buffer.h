/*
 * ring_buffer.h
 *
 *  Created on: Jul 8, 2026
 *      Author: aniru
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct{
	uint8_t* pxbuffer;
	uint8_t head;
	uint8_t tail;
	uint8_t count;
	uint8_t size;
}ring_buffer;


void create(ring_buffer* current, uint8_t* buffer, uint8_t size_n);

bool push(ring_buffer* current, uint8_t value);

uint8_t pop(ring_buffer* current);

#endif /* INC_RING_BUFFER_H_ */
