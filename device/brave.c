/*
 * brave.c
 *
 *  Created on: 11 de jun de 2018
 *      Author: elder
 */
#include "../hal.h"
#include "../driver/uart.h"
#include "brave.h"

#pragma PERSISTENT(buffer)
uint8_t buffer[3600] = {0};

uint8_t brave_status(void)
{
    uint8_t status = BRAVE_CONFIGURATION_UNKNOWN;

    if(READ_BRAVE_CONFIG_ERROR() == HIGH)
    {
        status = BRAVE_CONFIGURATION_ERROR;
    }
    else
    {
        if (READ_BRAVE_CONFIG_READY() == HIGH)
        {
            status = BRAVE_CONFIGURATION_OK;
        }
    }

    return status;
}

void brave_setup(void)
{
    uart0_set_rx_pointer(buffer);

    BRAVE_UART_RX_SETUP();
    BRAVE_UART_TX_SETUP();

    BRAVE_HARD_RESET_N_OUTPUT(LOW);         //start in reset state
    BRAVE_HARD_RESET_N_SETUP();

    BRAVE_CONFIG_ERROR_SETUP();

    BRAVE_CONFIG_READY_SETUP();

    BRAVE_MODE_0_SETUP(DIR_INPUT);          //let the pull-up/down resistors choose the initial mode
    BRAVE_MODE_1_SETUP(DIR_INPUT);
}

void brave_send(uint8_t *data, uint16_t length)
{
    uart0_write(data, length);
}

void brave_receive(uint8_t *data, uint16_t length)
{
    uart0_read(data, length);
}
