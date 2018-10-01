/*
 * hal.h
 *
 *  Created on: 30 de abr de 2018
 *      Author: elder
 */

#ifndef HAL_H_
#define HAL_H_

#include <msp430.h>
#include "misc/misc.h"

#define DIR_INPUT   0
#define DIR_OUTPUT  1

#define SETUP_DIR(dir, PDIR, BIT)((dir == DIR_OUTPUT) ?  BIT_SET(PDIR, BIT) :  BIT_CLEAR(PDIR, BIT) )

#define LOW  0
#define HIGH 1


#define SETUP_OUT(out, POUT, BIT)((out == HIGH) ? BIT_SET(POUT, BIT) : BIT_CLEAR(POUT, BIT) )

#define READ_INPUT(PIN, BIT)    ((PIN & BIT) != 0)

#define SELECT_PERIPHERAL_0   0
#define SELECT_PERIPHERAL_1   1
#define SELECT_PERIPHERAL_2   2
#define SELECT_PERIPHERAL_3   3

#define SETUP_SEL(sel, PSEL0, PSEL1, BIT)     \
({                                \
    if(sel & 0x01)                \
        BIT_SET(PSEL0, BIT);      \
    else                          \
        BIT_CLEAR(PSEL0, BIT);    \
    if(sel & 0x02)                \
        BIT_SET(PSEL1, BIT);      \
    else                          \
        BIT_CLEAR(PSEL1, BIT);    \
})


//(ULP 4.1) Detected uninitialized Port D in this project. Recommend initializing all unused ports to eliminate wasted current consumption on unused pins.</a>  null: (ULP 4.1) Detected uninitialized Port D in this project.
// Recommend initializing all unused ports to eliminate wasted current consumption on unused pins.

#define gpio_reset() {                 \
                        P1DIR = 0x00;  \
                        P2DIR = 0x00;  \
                        P3DIR = 0x00;  \
                        P4DIR = 0x00;  \
                        P5DIR = 0x00;  \
                        P6DIR = 0x00;  \
                        P7DIR = 0x00;  \
                        P8DIR = 0x00;  \
                        P9DIR = 0x00;  \
                        P10DIR = 0x00; \
                        P1OUT = 0x00;  \
                        P2OUT = 0x00;  \
                        P3OUT = 0x00;  \
                        P4OUT = 0x00;  \
                        P5OUT = 0x00;  \
                        P6OUT = 0x00;  \
                        P7OUT = 0x00;  \
                        P8OUT = 0x00;  \
                        P9OUT = 0x00;  \
                        P10OUT = 0x00; \
                    }

// PIN 4
#define SAT_BUS_I2C_SDA_SETUP()     SETUP_SEL(SELECT_PERIPHERAL_1, P1SEL0, P1SEL1, BIT6)

// PIN 5
#define SAT_BUS_I2C_SCL_SETUP()     SETUP_SEL(SELECT_PERIPHERAL_1, P1SEL0, P1SEL1, BIT7)

// PIN 7
#define BRAVE_MODE_0_SETUP(dir)     SETUP_DIR(dir, P6DIR, BIT0)
#define BRAVE_MODE_0_OUTPUT(out)    SETUP_OUT(out, P6OUT, BIT0)

// PIN 8
#define BRAVE_MODE_1_SETUP(dir)     SETUP_DIR(dir, P6DIR, BIT1)
#define BRAVE_MODE_1_OUTPUT(out)    SETUP_OUT(out, P6OUT, BIT1)

// PIN 11
#define UTMC_RESET_N_SETUP()        SETUP_DIR(DIR_OUTPUT, P6DIR, BIT4)
#define UTMC_RESET_N_OUTPUT(out)    SETUP_OUT(out, P6OUT, BIT4)

// PIN 12
#define UTMC_ERROR_SETUP()          SETUP_DIR(DIR_INPUT, P6DIR, BIT5)
#define READ_UTMC_ERROR()           READ_INPUT(P6IN, BIT5)

// PIN 17
#define SPARE_GPIO_1_SETUP(dir)     SETUP_DIR(dir, P2DIR, BIT7)

// PIN 18
#define SPARE_GPIO_2_SETUP(dir)     SETUP_DIR(dir, P10DIR, BIT2)

// PIN 19
#define SPARE_GPIO_3_SETUP(dir)     SETUP_DIR(dir, P5DIR, BIT0)

// PIN 20
#define SPARE_GPIO_4_SETUP(dir)     SETUP_DIR(dir, P5DIR, BIT1)

// PIN 39
#define BRAVE_HARD_RESET_N_SETUP()   SETUP_DIR(DIR_OUTPUT, P3DIR, BIT3)
#define BRAVE_HARD_RESET_N_OUTPUT(out)   SETUP_OUT(out, P3OUT, BIT3)

// PIN 40
#define BRAVE_CONFIG_ERROR_SETUP()  SETUP_DIR(DIR_INPUT, P3DIR, BIT4)
#define READ_BRAVE_CONFIG_ERROR()   READ_INPUT(P3IN, BIT4)

// PIN 41
#define BRAVE_CONFIG_READY_SETUP()  SETUP_DIR(DIR_INPUT, P3DIR, BIT5)
#define READ_BRAVE_CONFIG_READY()   READ_INPUT(P3IN, BIT5)

// PIN 45
#define LED_SETUP()                 SETUP_DIR(DIR_OUTPUT, P8DIR, BIT1)
#define LED_OUTPUT(out)             SETUP_OUT(out, P8OUT, BIT1)

// PIN 48

// PIN 49

// PIN 50
#define BRAVE_UART_RX_SETUP()       SETUP_SEL(SELECT_PERIPHERAL_1, P2SEL0, P2SEL1, BIT1)

// PIN 51
#define BRAVE_UART_TX_SETUP()       SETUP_SEL(SELECT_PERIPHERAL_1, P2SEL0, P2SEL1, BIT0)

// PIN 63
#define RADIO_RESET_N_SETUP(dir)    SETUP_DIR(dir, P1DIR, BIT3)
#define RADIO_RESET_N_OUTPUT(out)   SETUP_OUT(out, P1OUT, BIT3)

// PIN 64
#define RADIO_GPIO2_SETUP(dir)      SETUP_DIR(dir, P1DIR, BIT2)
#define RADIO_GPIO2_OUTPUT(out)     SETUP_OUT(out, P1OUT, BIT2)

// PIN 65
#define RADIO_GPIO1_SETUP(dir)      SETUP_DIR(dir, P1DIR, BIT1)
#define RADIO_GPIO1_OUTPUT(out)     SETUP_OUT(out, P1OUT, BIT1)

// PIN 66
#define RADIO_GPIO0_SETUP(dir)      SETUP_DIR(dir, P1DIR, BIT0)
#define RADIO_GPIO0_OUTPUT(out)     SETUP_OUT(out, P1OUT, BIT0)

// PIN 73
#define MUX_SELECT_SETUP()          SETUP_DIR(DIR_OUTPUT, P9DIR, BIT6)
#define MUX_SELECT_OUTPUT(out)      SETUP_OUT(out, P9OUT, BIT6)

// PIN 74
#define MUX_OUTPUT_ENABLE_N_SETUP()      SETUP_DIR(DIR_OUTPUT, P9DIR, BIT7)
#define MUX_OUTPUT_ENABLE_N_OUTPUT(out)  SETUP_OUT(out, P9OUT, BIT7)

// PIN 81
#define HFXOUT_SETUP()              SETUP_SEL(SELECT_PERIPHERAL_1, PJSEL0, PJSEL1, BIT7)

// PIN 82
#define HFXIN_SETUP()               SETUP_SEL(SELECT_PERIPHERAL_1, PJSEL0, PJSEL1, BIT6)

// PIN 84
#define LFXIN_SETUP()               SETUP_SEL(SELECT_PERIPHERAL_1, PJSEL0, PJSEL1, BIT4)

// PIN 85
#define LFXOUT_SETUP()              SETUP_SEL(SELECT_PERIPHERAL_1, PJSEL0, PJSEL1, BIT5)

// PIN 87
#define FLASH_SPI_MOSI_SETUP()      SETUP_SEL(SELECT_PERIPHERAL_1, P5SEL0, P5SEL1, BIT4)

// PIN 88
#define FLASH_SPI_MISO_SETUP()      SETUP_SEL(SELECT_PERIPHERAL_1, P5SEL0, P5SEL1, BIT5)

// PIN 89
#define FLASH_SPI_CLOCK_SETUP()     SETUP_SEL(SELECT_PERIPHERAL_1, P5SEL0, P5SEL1, BIT6)

// PIN 90
#define FLASH_SELECT_N_SETUP()      SETUP_DIR(DIR_OUTPUT, P5DIR, BIT7)
#define FLASH_SELECT_N_OUTPUT(out)  SETUP_OUT(out, P5OUT, BIT7)

// PIN 91
#define RADIO_SELECT_N_SETUP()      SETUP_SEL(SELECT_PERIPHERAL_2, P4SEL0, P4SEL1, BIT4)

// PIN 92
#define RADIO_SPI_CLOCK_SETUP()     SETUP_SEL(SELECT_PERIPHERAL_2, P4SEL0, P4SEL1, BIT5)

// PIN 93
#define RADIO_SPI_MOSI_SETUP()      SETUP_SEL(SELECT_PERIPHERAL_2, P4SEL0, P4SEL1, BIT6)

// PIN 94
#define RADIO_SPI_MISO_SETUP()      SETUP_SEL(SELECT_PERIPHERAL_2, P4SEL0, P4SEL1, BIT7)


#endif /* HAL_H_ */
