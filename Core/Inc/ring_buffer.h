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
	uint16_t head;
	uint16_t tail;
	uint16_t size;
	uint16_t count;
}ring_buffer;


void rb_create(ring_buffer* current, uint8_t* buffer, uint8_t size_n);

bool rb_push(ring_buffer* current, uint8_t value);

uint8_t rb_pop(ring_buffer* current);

#endif /* INC_RING_BUFFER_H_ */
