/*
 * mcu.c
 *
 *  Created on: 30 de mai de 2018
 *      Author: elder
 */


#include <msp430.h>
#include "../hal.h"
#include "mcu.h"
#include "../driver/clock.h"
#include "../driver/uart.h"

void setup_gpio(void)
{
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

//    mux_demux_setup()
//    flash_setup();
    BRAVE_UART_RX_SETUP();
    BRAVE_UART_TX_SETUP();
}

void setup_communication_interfaces(void)
{
//    uca0_setup(USCI_SPI_MODE);
    uart0_setup(115200);
//    ucb0_setup(USCI_I2C_MODE);
//    ucb1_setup(USCI_SPI_MODE);
}

void setup_hardware(void)
{
//    watchdog_timer_setup();

    setup_gpio();

    clock_setup();

    setup_communication_interfaces();
}
