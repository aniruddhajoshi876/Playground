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
	volatile uint16_t head;
	volatile uint16_t tail;
	uint16_t size;
}ring_buffer;


void rb_create(ring_buffer* current, uint8_t* buffer, uint8_t size_n);

bool rb_push(ring_buffer* current, uint8_t value);

uint8_t rb_pop(ring_buffer* current);

//syscalls.c
//ring_buffer rb;
//uint8_t buf[200];
//
//int _write(int file, char *ptr, int len)
//{
//	for (int i =0; i < len; i++){
//		if (!rb_push(&rb, ptr[i])){
//			return i; //buffer full
//		}
//	}
//	if (rb.head == rb.tail){return 0;} //buffer is empty
//
//	HAL_UART_Transmit_DMA(&huart1, &rb.pxbuffer[rb.tail], (rb.head > rb.tail) ? (rb.head - rb.tail) : (rb.size - rb.tail));
//
//  return len;
//}

//usart.c
//extern ring_buffer rb;
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart){
//	if (huart->Instance ==USART1){
//		rb.tail = (rb.tail + huart->TxXferSize) % rb.size;
//
//		if (rb.head != rb.tail){
//			HAL_UART_Transmit_DMA(&huart1, &rb.pxbuffer[rb.tail], (rb.head > rb.tail) ? (rb.head - rb.tail) : (rb.size - rb.tail));
//		}
//
//	}
//}


//main.c
//extern ring_buffer rb;
//extern uint8_t buf[200];
//...
//rb_create(&rb, buf, 200);
#endif /* INC_RING_BUFFER_H_ */
