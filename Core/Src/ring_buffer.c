/*
 * ring_buffer.c
 *
 *  Created on: Jul 8, 2026
 *      Author: aniru
 */

#include "ring_buffer.h"



//	producer: printf placing bytes into queue
//	consumer: uart/dma taking bytes and sending them





void rb_create(ring_buffer* current, uint8_t* buffer, uint8_t size_n){
//	initialize rb of size 'buffer size' as defined in macro, count = 0
//	initialize head and tail
	current->pxbuffer = buffer;
	current->head=0;
	current->tail=0;
	current->size = size_n;
}

bool rb_push(ring_buffer* current, uint8_t value){
//	input value at head
//	increment head value, includes wrap around
//	count == size, full ; count == 0 , empty
//	count reaches a max of size, every tail increment reduces it by one, every head increment increases
	if ( ((current->head + 1)%current->size) == current->tail){
		return 0;
	}

	current->pxbuffer[current->head] = value;
	current->head = (current->head + 1) % current->size;
	return true;
}

uint8_t rb_pop(ring_buffer* current){
//	remove value from tail
//	increment tail
//	check if buffer is empty
//	return value popped
	if (current->head==current->tail){
		return false;
	}

	uint8_t temp = current->pxbuffer[current->tail];
	current->pxbuffer[current->tail] = 0;
	current->tail = (current->tail + 1) % current->size;

	return temp;
}




