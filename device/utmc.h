/*
 * utmc.h
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#ifndef DEVICE_UTMC_H_
#define DEVICE_UTMC_H_

#include <stdint.h>

#define UTMC_STATE_ERROR                0
#define UTMC_STATE_OK                   1

void utmc_setup(void);
uint8_t utmc_status(void);

#define utmc_power_on()       UTMC_RESET_N_OUTPUT(HIGH)
#define utmc_power_off()      UTMC_RESET_N_OUTPUT(LOW)


#endif /* DEVICE_UTMC_H_ */
