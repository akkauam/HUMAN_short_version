/*
 * spi.h
 *
 *  Created on: 30 de abr de 2018
 *      Author: elder
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void spi_setup(void);
void spi_write_byte(uint8_t byte);
uint8_t spi_read_byte(void);

#endif /* SPI_H_ */
