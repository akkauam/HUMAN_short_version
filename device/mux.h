/*
 * mux.h
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#ifndef DEVICE_MUX_H_
#define DEVICE_MUX_H_

#include <stdint.h>

#define SELECT_BRAVE    0
#define SELECT_HUMAN    1

void mux_setup(void);
void mux_select(uint8_t selected_device);

#endif /* DEVICE_MUX_H_ */
