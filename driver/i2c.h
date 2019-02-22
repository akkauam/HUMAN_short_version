/*
 * i2c.h
 *
 *  Created on: 12 de jun de 2018
 *      Author: elder
 */
#ifndef DRIVER_I2C_H_
#define DRIVER_I2C_H_

#include <stdint.h>

#define I2C_FLAG_RX     0x01
#define I2C_FLAG_TX     0x02
#define I2C_FLAG_STOP   0x04

void i2c0_setup();

#endif /* DRIVER_I2C_H_ */
