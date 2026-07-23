/*
 * ring_buffer.c
 *
 *  Created on: Jul 8, 2026
 *      Author: aniru
 */

#include "ring_buffer.h"



//	producer: printf placing bytes into queue
//	consumer: uart/dma taking bytes and sending them

//	active_len:
//	see how many bytes uart sent and move tail accordingly. it can only send contiguous bytes in memory
//
//	size = 8
//
//	[ D ][ E ][ _ ][ _ ][ _ ][ A ][ B ][ C ]
//			  ^               ^
//			head             tail



void create(ring_buffer* current, uint8_t* buffer, uint8_t size_n){
//	initialize rb of size 'buffer size' as defined in macro, count = 0
//	initialize head and tail
	current->pxbuffer = buffer;
	current->count=0;
	current->head=0;
	current->tail=0;
	current->size = size_n;
}

bool push(ring_buffer* current, uint8_t value){
//	input value at head
//	increment head value, includes wrap around
//	count == size, full ; count == 0 , empty
//	count reaches a max of size, every tail increment reduces it by one, every head increment increases
	if (current->count == current->size){
		return false;
	}
	current->pxbuffer[current->head] = value;
	current->head = (current->head + 1) % current->size;
	current->count++;
	return true;
}

uint8_t pop(ring_buffer* current){
//	remove value from tail
//	increment tail
//	check if buffer is empty
//	return value popped
	if (current->count == 0){
		return 0;
	}

	uint8_t temp = current->pxbuffer[current->tail];
	current->pxbuffer[current->tail] = 0;
	current->tail = (current->tail + 1) % current->size;
	current->count --;

	return temp;
}




