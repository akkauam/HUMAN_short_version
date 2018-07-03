/*
 * crc.h
 *
 *  Created on: 3 de jul de 2018
 *      Author: elder
 */
#ifndef DRIVER_CRC_H_
#define DRIVER_CRC_H_

#include <stdint.h>

#define CRC_VALID       1
#define CRC_INVALID     0

void crc_init(void);
void crc_feed(uint16_t input);
uint16_t crc_result(void);
uint8_t crc_compare(uint16_t value_to_compare);


#endif /* DRIVER_CRC_H_ */
