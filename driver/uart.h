/*
 * uart.h
 *
 *  Created on: 9 de jun de 2018
 *      Author: elder
 */
#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#include <msp430.h>
#include <stdint.h>

void uart_write(uint8_t *data, uint8_t length);
void uart_read(uint8_t *data, uint8_t length);
void uart_flush();
void uart_setup(uint32_t baudrate);


#endif /* DRIVER_UART_H_ */
