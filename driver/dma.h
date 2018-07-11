/*
 * dma.h
 *
 *  Created on: 3 de jul de 2018
 *      Author: elder
 */
#ifndef DRIVER_DMA_H_
#define DRIVER_DMA_H_

#include <stdint.h>

void dma0_setup(void);
void dma1_setup(void);
void dma0_start(void);
void dma1_start(void);
uint8_t dma1_ready(void);
#define wait_dma1_ready()  while(dma1_ready() == 0)


#endif /* DRIVER_DMA_H_ */
