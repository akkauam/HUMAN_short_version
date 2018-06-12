/*
 * i2c.c
 *
 *  Created on: 12 de jun de 2018
 *      Author: elder
 */

#include "../hal.h"
#include "i2c.h"

uint8_t *p_tx_buffer;
uint16_t tx_size;
uint8_t *p_rx_buffer, *rx_buffer_iterator, *rx_max_address;
uint16_t *rx_size;

void i2c0_prepare_to_send(uint8_t *data, uint16_t length)
{
    p_tx_buffer = data;
    tx_size = length;
}


void i2c0_setup(uint8_t *rx_buffer_address, uint16_t rx_max_size, uint16_t *p_rx_size)
{
    tx_size = 0;

    rx_size = p_rx_size;
    *rx_size = 0;
    p_rx_buffer = rx_buffer_address;
    rx_buffer_iterator = p_rx_buffer;

    rx_max_address = rx_buffer_address + rx_max_size;

    SAT_BUS_I2C_SDA_SETUP();
    SAT_BUS_I2C_SCL_SETUP();

    UCB0CTLW0 = UCSWRST;                      // Software reset enabled

    UCB0CTLW0 |= UCMODE_3 | UCSYNC;           // I2C mode, sync mode
    UCB0I2COA0 = 0x42 | UCOAEN;               // own address is 0x42 + enable

    UCB0CTLW0 &= ~UCSWRST;                    // clear reset register
    UCB0IE |= UCTXIE0 | UCRXIE0 | UCSTTIE |UCSTPIE;              // transmit, receive, start and stop interrupt enable
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts
    case USCI_I2C_UCALIFG:   break;         // Vector 2: ALIFG
    case USCI_I2C_UCNACKIFG: break;         // Vector 4: NACKIFG
    case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:                 // Vector 8: STPIFG
      UCB0IFG &= ~UCSTPIFG;                 // Clear stop condition int flag
      break;
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
        if(rx_buffer_iterator > rx_max_address)
        {
            rx_buffer_iterator = p_rx_buffer;
        }
        *rx_buffer_iterator = UCB0RXBUF;
        rx_buffer_iterator++;
        (*rx_size)++;
        break;
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        if(tx_size > 0)
        {
            UCB0TXBUF = *p_tx_buffer;
            p_tx_buffer++;
            tx_size--;
        }
        else
        {
            UCB0TXBUF = 0xFF;
        }

      break;
    case USCI_I2C_UCBCNTIFG: break;         // Vector 26: BCNTIFG
    case USCI_I2C_UCCLTOIFG: break;         // Vector 28: clock low timeout
    case USCI_I2C_UCBIT9IFG: break;         // Vector 30: 9th bit
    default: break;
  }
}
