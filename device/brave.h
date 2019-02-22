/*
 * brave.h
 *
 *  Created on: 11 de jun de 2018
 *      Author: elder
 */
#ifndef DEVICE_BRAVE_H_
#define DEVICE_BRAVE_H_

#include <stdint.h>

#define BRAVE_CONFIGURATION_UNKNOWN     0
#define BRAVE_CONFIGURATION_ERROR       1
#define BRAVE_CONFIGURATION_OK          2

void brave_send(uint8_t *data, uint16_t length);
//void brave_receive(uint8_t *data, uint16_t length);
void brave_setup(void);
uint8_t brave_status(void);

#define brave_power_on()      BRAVE_HARD_RESET_N_OUTPUT(HIGH)
#define brave_power_off()     BRAVE_HARD_RESET_N_OUTPUT(LOW)


#endif /* DEVICE_BRAVE_H_ */
