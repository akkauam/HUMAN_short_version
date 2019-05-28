/*
 * uart.c
 *
 *  Created on: 9 de jun de 2018
 *      Author: elder
 */

#include <msp430.h>
#include "uart.h"
#include "../misc/project_definitions.h"
#include "../variables.h"


uint8_t *p_rx_buffer;
uint8_t *buffer_iterator;

#pragma PERSISTENT(uart_buffer_rx_index)
volatile uint16_t uart_buffer_rx_index = 0;
#pragma PERSISTENT(uart_buffer_rx_offset)
volatile uint16_t uart_buffer_rx_offset = 0;

#pragma PERSISTENT(uart_buffer_tx_index)
volatile uint16_t uart_buffer_tx_index = 0;


#pragma PERSISTENT(uart_buffer_rx)
volatile uart_buffer_rx_t uart_buffer_rx = {0xFF};
//volatile uint8_t uart_buffer_rx[1119] = {0xFF};
#pragma PERSISTENT(uart_buffer_tx)
volatile payload2_uplink_t uart_buffer_tx = {0xFF};
volatile uint8_t uart_flag = 0;

void uart0_setup(uint32_t baudrate)
{
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK

    switch(baudrate)
    {
    case 9600:
        UCA0BR0 = 104;                             // 9600 baud at 16 MHz
        UCA0BR1 = 0x00;
        UCA0MCTLW |= UCOS16 | UCBRF_2 | 0xD600;
        break;
    case 115200:
        UCA0BR0 = 8;                             // 115200 baud at 16 MHz
        UCA0BR1 = 0x00;
        UCA0MCTLW |= UCOS16 | UCBRF_10 | 0xF700;
        break;
    default: //9600
        UCA0BR0 = 104;
        UCA0BR1 = 0x00;
        UCA0MCTLW |= UCOS16 | UCBRF_2 | 0xD600;
        break;
    }

    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
//    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    __bis_SR_register(GIE);
}

void uart0_write(uint8_t *data, uint8_t length)
{
    while (length-- > 0)
    {
        while (UCA0STATW & UCBUSY);
        UCA0TXBUF = *data;
        data++;
    }
}

void uart0_flush(void)
{
    volatile uint8_t x;
    while (UCA0IFG & UCRXIFG)
    {
        x = UCA0RXBUF;
    }
}

//void uart0_read(uint8_t *data, uint8_t length)
//{
//    uart0_flush();
//    while (length--)
//    {
//        while (!(UCA0IFG & UCRXIFG));
//        *data =  UCA0RXBUF;
//        data++;
//    }
//}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      uart_buffer_rx.byte[uart_buffer_rx_index++] = UCA0RXBUF;
      uart_buffer_rx_index %= sizeof(uart_buffer_rx);
      break;
    case USCI_UART_UCTXIFG:
        if(uart_buffer_tx_index < sizeof(i2c_rx_buffer.data.ccsds_telecommand))
            UCA0RXBUF = uart_buffer_tx.data.ccsds_telecommand[uart_buffer_tx_index++];
        break;
    case USCI_UART_UCSTTIFG:
        break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
