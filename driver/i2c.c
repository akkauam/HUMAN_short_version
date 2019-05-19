/*
 * i2c.c
 *
 *  Created on: 12 de jun de 2018
 *      Author: elder
 */

#include "../hal.h"
#include "i2c.h"
#include "../misc/project_definitions.h"



#pragma PERSISTENT(i2c_tx_buffer)
volatile payload2_downlink_t i2c_tx_buffer[NUMBER_OF_UTMC_SEGMENTS+NUMBER_OF_STATUS_SEGMENTS] = {{PAYLOAD2_NO_PENDING_DATA, {0xFF}}};

#pragma PERSISTENT(i2c_rx_buffer)
volatile payload2_uplink_t i2c_rx_buffer = {PAYLOAD2_NO_PENDING_DATA, {0xFF}};

volatile uint8_t i2c_flag = 0;
uint8_t n;
volatile uint8_t actual_transfer = 0;
volatile uint8_t ccsds_offset = 0;
volatile uint8_t status_offset = 0;
volatile uint8_t i2c_buffer_index = 0;


void i2c0_setup()
{
    SAT_BUS_I2C_SDA_SETUP();
    SAT_BUS_I2C_SCL_SETUP();

    UCB0CTLW0 = UCSWRST;                      // Software reset enabled

    UCB0CTLW0 |= UCMODE_3 | UCSYNC;           // I2C mode, sync mode
    UCB0I2COA0 = 0x42 | UCOAEN;               // own address is 0x42 + enable

    UCB0CTLW0 &= ~UCSWRST;                    // clear reset register
    UCB0IE |= UCTXIE0 | UCSTTIE | UCSTPIE;              // transmit, receive, start and stop interrupt enable
    i2c_buffer_index = 0;
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
        if(UCTR & UCB0CTLW0)                //if are in transmite mode
        {
            for (n = 0; n < NUMBER_OF_STATUS_SEGMENTS + status_offset &&
                (i2c_tx_buffer[(status_offset + n) % NUMBER_OF_STATUS_SEGMENTS + NUMBER_OF_UTMC_SEGMENTS].type
                != PAYLOAD2_BITSTREAM_STATUS_REPLAY); n++);

            status_offset = (status_offset + n) % NUMBER_OF_STATUS_SEGMENTS;
            actual_transfer = status_offset + NUMBER_OF_UTMC_SEGMENTS;

            if (i2c_tx_buffer[actual_transfer].type != PAYLOAD2_BITSTREAM_STATUS_REPLAY)
            {
                for (n=0; (n < NUMBER_OF_UTMC_SEGMENTS + ccsds_offset) &&
                    (i2c_tx_buffer[(ccsds_offset + n) % NUMBER_OF_UTMC_SEGMENTS].type != PAYLOAD2_CCSDS_TELEMETRY);n++);

                ccsds_offset = (ccsds_offset + n) % NUMBER_OF_UTMC_SEGMENTS;
                actual_transfer = ccsds_offset;
            }
            UCB0TXBUF = i2c_tx_buffer[actual_transfer].type;
        }
        i2c_buffer_index = 0;
        UCB0IFG &= ~UCSTTIFG;               // Clear start condition int flag
        break;

    case USCI_I2C_UCSTPIFG:                 // Vector 8: STPIFG
      i2c_flag |= I2C_FLAG_STOP;
      if(UCTR & ~UCB0CTLW0)
      {
          i2c_flag |= I2C_FLAG_RX;
          DMA0CTL &= ~DMAEN;                //clear dma enable to read e inteprete the telecommand
      }else
      {
          i2c_flag |= I2C_FLAG_TX;
          i2c_tx_buffer[actual_transfer].type = PAYLOAD2_NO_PENDING_DATA;
      }
      UCB0IFG &= ~UCSTPIFG;                 // Clear stop condition int flag
      break;

    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
       break;

    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        UCB0TXBUF = ((uint8_t*) &i2c_tx_buffer[actual_transfer].data) [i2c_buffer_index++];
        break;

    case USCI_I2C_UCBCNTIFG: break;         // Vector 26: BCNTIFG
    case USCI_I2C_UCCLTOIFG: break;         // Vector 28: clock low timeout
    case USCI_I2C_UCBIT9IFG: break;         // Vector 30: 9th bit
    default: break;
  }
}
