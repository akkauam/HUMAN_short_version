/*
 * spi.c
 *
 *  Created on: 30 de abr de 2018
 *      Author: elder
 */
#include "spi.h"

void spi_setup(void)
{
//    BIT_SET(P5SEL0, (BIT4 | BIT5 | BIT6 | BIT7));
//    BIT_CLEAR(P5SEL1, (BIT4 | BIT5 | BIT6 | BIT7));
    BIT_CLEAR(P1SEL0, BIT5 );
    BIT_SET(P1SEL1, BIT5 );
    BIT_SET(P4SEL0, (BIT2 | BIT3));
    BIT_CLEAR(P4SEL1, (BIT2 | BIT3));


    UCA0CTLW0 = UCSWRST;                      // **Put state machine in reset**
    UCA0CTLW0 |= UCMST | UCSYNC | UCCKPH | UCMSB; // 3-pin, 8-bit SPI master
                                              // Clock polarity high, MSB
    UCA0CTLW0 |= UCSSEL__SMCLK;               // SM 1MHz
    UCA0BR0 = 0x02;                           // /2
    UCA0BR1 = 0;                              //
    UCA0MCTLW = 0;                            // No modulation
    UCA0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
}

void spi_write_byte(uint8_t byte)
{
    while(!(UCA0IFG & UCTXIFG)); //wait_for_buffer_empty
    UCA0TXBUF = byte;
    while(!(UCA0IFG & UCRXIFG)); //wait_for_data_sent
}

uint8_t spi_read_byte(void)
{
    while(!(UCA0IFG & UCTXIFG)); //wait_for_buffer_empty
    UCA0TXBUF = 0;
    while(!(UCA0IFG & UCRXIFG)); //wait_for_data_sent
    return UCA0RXBUF;
}


