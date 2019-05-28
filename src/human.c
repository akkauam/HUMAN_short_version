/*
 * human.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */

#include "../misc/project_definitions.h"
#include "../hal.h"
#include "../driver/watchdog_timer.h"
#include "../driver/dma.h"
#include "../driver/crc.h"
#include "../driver/i2c.h"
#include "../driver/uart.h"
#include "../device/brave.h"
#include "../device/utmc.h"
#include "human.h"
#include "../variables.h"
#include <string.h>

#pragma PERSISTENT(FRAM_utmc_tm_to_obdh)
volatile uint8_t  FRAM_utmc_tm_to_obdh[TELEMETRY_FROM_UTMC_SIZE] = {0xFF};

//
//#pragma PERSISTENT(FRAM_sector_data)
//volatile uint8_t FRAM_sector_data[SECTOR_SIZE] = {0xFF};

void update_target(void)
{
    brave_power_on();

    utmc_power_on();
}


void housekeeping(void)
{
    payload2_uplink_t actual_command;


    while(1)
    {

        if(i2c_flag & I2C_FLAG_RX)
        {
            i2c_flag &= ~I2C_FLAG_RX;
            actual_command = i2c_rx_buffer;
            DMA0CTL |= DMAEN;

            switch (actual_command.type)
            {
            case PAYLOAD2_CCSDS_TELECOMMAND:
                memcpy( (void*) uart_buffer_tx.data.ccsds_telecommand , (const void*) actual_command.data.ccsds_telecommand, sizeof(actual_command.data.ccsds_telecommand));
                uart_buffer_tx_index = 0;
                UCA0RXBUF = uart_buffer_tx.data.ccsds_telecommand[uart_buffer_tx_index++];
                break;
            case PAYLOAD2_NO_PENDING_DATA:
            default:
                break;
            }
        }

        utmc_telemetry_verify();

    }


}

void utmc_telemetry_verify() // polling uart buffer verifying if have one telemetry received
{
    while((((uart_buffer_rx_offset + 4) % sizeof(uart_buffer_rx.byte)) != uart_buffer_rx_index)  &&
            uart_buffer_rx.byte[(uart_buffer_rx_offset + 0) % sizeof(uart_buffer_rx)] == 0x1A &&
            uart_buffer_rx.byte[(uart_buffer_rx_offset + 1) % sizeof(uart_buffer_rx)] == 0xCF &&
            uart_buffer_rx.byte[(uart_buffer_rx_offset + 2) % sizeof(uart_buffer_rx)] == 0xFC &&
            uart_buffer_rx.byte[(uart_buffer_rx_offset + 3) % sizeof(uart_buffer_rx)] == 0x1D)
    {
        uart_buffer_rx_offset = (++uart_buffer_rx_offset) % sizeof(uart_buffer_rx);
    }
    // uart_buffer_rx is a circular buffer
    // pollig until 3 possition before the last byte recive, but the uart_buffer_rx_index aways point to the next free position
    // 0x1A ins the first byte of UTMC Telemetry package

    // 0x1ACFFC1D is the first 4 bytes of UTMC Telemetry package
    if( uart_buffer_rx.byte[(uart_buffer_rx_offset + 0) % sizeof(uart_buffer_rx)] == 0x1A &&
        uart_buffer_rx.byte[(uart_buffer_rx_offset + 1) % sizeof(uart_buffer_rx)] == 0xCF &&
        uart_buffer_rx.byte[(uart_buffer_rx_offset + 2) % sizeof(uart_buffer_rx)] == 0xFC &&
        uart_buffer_rx.byte[(uart_buffer_rx_offset + 3) % sizeof(uart_buffer_rx)] == 0x1D &&
        ((uart_buffer_rx_index + 1) % sizeof(uart_buffer_rx)) == uart_buffer_rx_offset)
    {
        split_utmc_telemetry();
    }
}


void split_utmc_telemetry()
{
    uint16_t sequence_number;
    uint16_t aux = 0;

    for(sequence_number = 0; sequence_number < NUMBER_OF_UTMC_SEGMENTS; sequence_number++)
        i2c_tx_buffer[sequence_number].type = PAYLOAD2_NO_PENDING_DATA;

    for(sequence_number = 0; (sequence_number < NUMBER_OF_UTMC_SEGMENTS) ; sequence_number++)
    {

        aux = uart_buffer_rx_offset + sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment) * sequence_number;

        if(aux + sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment) <= sizeof(uart_buffer_rx))
        {
            i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment_number = sequence_number;

            memcpy( (void*) i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment,
                    (const void*) (&uart_buffer_rx + aux),
                    sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment));

        }else if(aux < sizeof(uart_buffer_rx))
        {
            i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment_number = sequence_number;

            memcpy( (void*) i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment,
                    (const void*) (&uart_buffer_rx + aux),
                    sizeof(uart_buffer_rx) - aux);


            memcpy( (void*) i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment,
                    (const void*) &uart_buffer_rx,
                    sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment) - (sizeof(uart_buffer_rx) - aux));

        }else
        {
            i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment_number = sequence_number;

            memcpy( (void*) i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment,
                    (const void*) (&uart_buffer_rx + aux - sizeof(uart_buffer_rx)),
                    sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment));

        }

        i2c_tx_buffer[sequence_number].type = PAYLOAD2_CCSDS_TELEMETRY;
    }
}



