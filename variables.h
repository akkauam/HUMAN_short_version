/*
 * variables.h
 *
 *  Created on: 21 de fev de 2019
 *      Author: elder
 */
#ifndef VARIABLES_H_
#define VARIABLES_H_

extern volatile uint16_t i2c_buffer_index;
extern volatile payload2_downlink_t i2c_tx_buffer[7];
extern volatile payload2_uplink_t i2c_rx_buffer;
extern volatile uint8_t i2c_flag;

extern volatile uint16_t uart_buffer_index;
extern volatile uint8_t uart_buffer[];
extern volatile uint8_t uart_flag;

#endif /* VARIABLES_H_ */
