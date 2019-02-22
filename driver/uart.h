/*
 * uart.h
 *
 *  Created on: 9 de jun de 2018
 *      Author: elder
 */
#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#include <stdint.h>

#define FIFO_UART_MAX_LENGTH    200
#define UART_FLAG_RX            0x01

void uart0_write(uint8_t *data, uint8_t length);
void uart0_read(uint8_t *data, uint8_t length);
void uart0_flush(void);
void uart0_setup(uint32_t baudrate);


#endif /* DRIVER_UART_H_ */
