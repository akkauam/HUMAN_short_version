/*
 * dma.c
 *
 *  Created on: 3 de jul de 2018
 *      Author: elder
 */

#include <msp430.h>
#include "../misc/project_definitions.h"
#include "dma.h"

#define DMA0_SOURCE_ADDRESS         (0x00064C)      //&UCB0RXBUF

#define DMA0_received_data_length() (DMA0_BLOCK_SIZE - DMA0SZ)

#define DMA1_DESTINATION_ADDRESS    (0x000152)      //&CRCDIRB CRC input

#define DMA1_BLOCK_SIZE             (BITSTREAM_SEGMENT_DATA_SIZE/2)     //frame size + seq. number
extern volatile payload2_uplink_t i2c_rx_buffer;

void dma0_setup(void)
{
    DMACTL0 &= ~(0x001F);                 // clear DMA0SEL
    DMACTL0 |= DMA0TSEL__UCB0RXIFG0;      // i2c rx
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) DMA0_SOURCE_ADDRESS);
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &i2c_rx_buffer);

    DMA0SZ = sizeof(i2c_rx_buffer);
    DMA0CTL = DMADT_4 | DMASRCINCR_0 | DMADSTINCR_3 | DMADSTBYTE;        // Rpt single, inc dst
    DMA0CTL |= DMAEN;

    //dma0_start();
}

void dma1_setup(uint32_t dma1_source_address)
{
    DMACTL0 &= ~(0x1F00);               // clear DMA1SEL
    DMACTL0 |= DMA1TSEL__DMAREQ;        // crc ready
    __data16_write_addr((unsigned short) &DMA1SA,(unsigned long) dma1_source_address);
    __data16_write_addr((unsigned short) &DMA1DA,(unsigned long) DMA1_DESTINATION_ADDRESS);

    DMA1SZ = DMA1_BLOCK_SIZE;
    DMA1CTL = DMADT_2 | DMASRCINCR_3 | DMADSTINCR_0;        // burst-block, inc src
    DMA1CTL |= DMAEN;
}

void dma0_start(void)
{
    DMA0CTL |= DMAREQ;
}

uint8_t dma0_ready(void)
{
    return ((DMA0CTL & DMAIFG) != 0);
}

void dma1_start(void)
{
    DMA1CTL |= DMAREQ;
}

uint8_t dma1_ready(void)
{
    return ((DMA1CTL & DMAIFG) != 0);
}
