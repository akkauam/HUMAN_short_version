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

void uart0_set_rx_pointer(uint8_t *buffer_address);
void uart0_write(uint8_t *data, uint8_t length);
void uart0_read(uint8_t *data, uint8_t length);
void uart0_flush(void);
void uart0_setup(uint32_t baudrate);


#endif /* DRIVER_UART_H_ */
