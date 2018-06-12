/*
 * i2c.h
 *
 *  Created on: 12 de jun de 2018
 *      Author: elder
 */
#ifndef DRIVER_I2C_H_
#define DRIVER_I2C_H_

#include <stdint.h>

void i2c0_prepare_to_send(uint8_t *data, uint16_t length);
void i2c0_setup(uint8_t *rx_buffer_address, uint16_t rx_max_size, uint16_t *p_rx_size);

#endif /* DRIVER_I2C_H_ */
