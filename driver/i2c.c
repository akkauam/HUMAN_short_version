/*
 * i2c.c
 *
 *  Created on: 12 de jun de 2018
 *      Author: elder
 */

#include "../hal.h"
#include "i2c.h"

uint8_t address_counter;
uint32_t address;
uint8_t *buffer;
uint8_t *flag;


void i2c0_setup(uint8_t *buffer_address, uint8_t *new_data_flag)
{
    buffer = buffer_address;
    flag = new_data_flag;

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
    case USCI_I2C_UCSTTIFG:                 // Vector 6: STTIFG
        break;
    case USCI_I2C_UCSTPIFG:                 // Vector 8: STPIFG
      UCB0IFG &= ~UCSTPIFG;                 // Clear stop condition int flag
      address_counter = 0;
      address = 0;
      *flag |= I2C_FLAG_STOP;
      break;
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
        if(address_counter < 4)
        {
            address |= ((uint32_t)UCB0RXBUF)<<address_counter;
            address_counter++;
            if(address >= 10000) //invalid address
            {
                address = 0;
            }
        }
        else
        {
            buffer[address] = UCB0RXBUF;
            address++;
            *flag |= I2C_FLAG_RX;
        }
        break;
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        if(address_counter >= 4)
        {
            UCB0TXBUF = buffer[address];
            address++;
            *flag |= I2C_FLAG_TX;
        }
        break;
    case USCI_I2C_UCBCNTIFG: break;         // Vector 26: BCNTIFG
    case USCI_I2C_UCCLTOIFG: break;         // Vector 28: clock low timeout
    case USCI_I2C_UCBIT9IFG: break;         // Vector 30: 9th bit
    default: break;
  }
}
