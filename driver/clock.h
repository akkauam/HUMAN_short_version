/*
 * clock.h
 *
 *  Created on: 11 de jun de 2018
 *      Author: elder
 */
#ifndef DRIVER_CLOCK_H_
#define DRIVER_CLOCK_H_

#include <stdint.h>

#define SETUP_CLOCK_FAIL    0
#define SETUP_CLOCK_SUCESS  1

uint8_t clock_setup(void);

#endif /* DRIVER_CLOCK_H_ */
