/*
 * mcu.c
 *
 *  Created on: 30 de mai de 2018
 *      Author: elder
 */


#include <msp430.h>
#include "../hal.h"
#include "../driver/clock.h"
#include "../driver/dma.h"
#include "../driver/uart.h"
#include "../driver/i2c.h"
#include "../driver/spi.h"
#include "../driver/watchdog_timer.h"
#include "../device/brave.h"
#include "../device/flash_memory.h"
#include "../device/mux.h"
#include "../device/utmc.h"
#include "mcu.h"

void setup_gpio(void)
{
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    gpio_reset();

//    brave_setup();

//    utmc_setup();

//    mux_setup();

//    memory_setup();
}

void setup_communication_interfaces(void)
{
//    uca0_setup(USCI_SPI_MODE);
//    ucb0_setup(USCI_I2C_MODE);
//    ucb1_setup(USCI_SPI_MODE);
//    uart0_setup(115200);
    i2c0_setup();
    //spi_setup();
}

void setup_hardware(void)
{
    watchdog_timer_setup();

    setup_gpio();

    clock_setup();

    setup_communication_interfaces();

    dma0_setup();

    watchdog_timer_reset_counter();
}

void enable_interrupts(void)
{

}
