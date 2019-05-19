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
#include "../device/flash_memory.h"
#include "../device/brave.h"
#include "../device/utmc.h"
#include "human.h"
#include "../variables.h"
#include <string.h>

#pragma PERSISTENT(FRAM_utmc_tm_to_obdh)
volatile uint8_t  FRAM_utmc_tm_to_obdh[TELEMETRY_FROM_UTMC_SIZE] = {0xFF};

#pragma PERSISTENT(FRAM_segment)
bitstream_segment_t FRAM_segment = {0xFF};

#pragma PERSISTENT(FRAM_sector_data)
volatile uint8_t FRAM_sector_data[SECTOR_SIZE] = {0xFF};

void update_target(void)
{
    brave_power_on();

    utmc_power_on();
}

uint8_t is_memory_ok(void)
{
    volatile uint8_t id[3];
    uint8_t memory_ok = 0;

    memory_id((uint8_t *)id);
    if((id[0] == MEM_ID_BYTE_0) &&
       (id[1] == MEM_ID_BYTE_1) &&
       (id[2] == MEM_ID_BYTE_2) )
    {
        memory_ok = 1;
    }

    return memory_ok;
}

void write_segment_in_memory(bitstream_segment_t *bitstream_segment, uint32_t bitstream_address)
{
    uint16_t i, position_in_sector;
    uint32_t sector_to_modify;

    sector_to_modify   = (bitstream_segment->sequence_number * BITSTREAM_SEGMENT_DATA_SIZE) / SECTOR_SIZE;
    position_in_sector = (bitstream_segment->sequence_number * BITSTREAM_SEGMENT_DATA_SIZE) % SECTOR_SIZE;

    memory_read(bitstream_address + sector_to_modify, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);

    for(i = 0; (i < BITSTREAM_SEGMENT_DATA_SIZE) && ((i + position_in_sector) < SECTOR_SIZE); i++)
    {
        FRAM_sector_data[i + position_in_sector] = bitstream_segment->data[i];
    }

    memory_sector_erase(bitstream_address + sector_to_modify);
    memory_page_program(bitstream_address + sector_to_modify, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);

    if(i < BITSTREAM_SEGMENT_DATA_SIZE)
    {
        memory_read(bitstream_address + sector_to_modify + 1, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);

        for(; i < BITSTREAM_SEGMENT_DATA_SIZE; i++)
        {
            FRAM_sector_data[i + position_in_sector - SECTOR_SIZE] = bitstream_segment->data[i];
        }

        memory_sector_erase(bitstream_address + sector_to_modify + 1);
        memory_page_program(bitstream_address + sector_to_modify + 1, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);
    }

#ifdef DIAGNOSTIC_MODE
    memory_read(image_in_use + sector_to_modify, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);
    for(i = 0; i < BITSTREAM_SEGMENT_DATA_SIZE; i++)
    {
        if(FRAM_sector_data[i + position_in_sector] != new_segment[i])
        {
            throw_error(FLASH_MEMORY_CORRUPTED_SECTOR);
        }
    }
#endif
    // stores crc
    sector_to_modify   = (bitstream_segment->sequence_number * 2) / SECTOR_SIZE;
    position_in_sector = (bitstream_segment->sequence_number * 2) % SECTOR_SIZE;

    memory_read(bitstream_address + IMAGE_MAX_SIZE + sector_to_modify, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);

    FRAM_sector_data[position_in_sector]     = (bitstream_segment->sequence_number     ) & 0xFF;
    FRAM_sector_data[position_in_sector + 1] = (bitstream_segment->sequence_number >> 8) & 0xFF;

    memory_sector_erase(bitstream_address + IMAGE_MAX_SIZE + sector_to_modify);
    memory_page_program(bitstream_address + IMAGE_MAX_SIZE + sector_to_modify, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);
}

void swap_image_in_use(uint8_t version)
{
    uint32_t current_sector_address;
    uint32_t image_to_swap;

    if(version == CHOOSE_IMAGE_A)
    {
        image_to_swap = IMAGE_A_COPY_1_ADDRESS;
    }
    else
    {
        image_to_swap = IMAGE_B_COPY_1_ADDRESS;
    }

    for(current_sector_address = 0; current_sector_address < IMAGE_MAX_SIZE; current_sector_address += SECTOR_SIZE)
    {
        memory_read(image_to_swap + current_sector_address, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);

        memory_sector_erase(RUNNING_IMAGE_ADDRESS + current_sector_address);
        memory_page_program(RUNNING_IMAGE_ADDRESS + current_sector_address, (uint8_t *) FRAM_sector_data, SECTOR_SIZE);
    }
}

void read_segment_from_memory(bitstream_segment_t *bitstream_segment, uint32_t bitstream_address)
{
    memory_read(bitstream_address + (bitstream_segment->sequence_number * BITSTREAM_SEGMENT_DATA_SIZE), bitstream_segment->data, BITSTREAM_SEGMENT_DATA_SIZE);
    memory_read(bitstream_address + IMAGE_MAX_SIZE + (bitstream_segment->sequence_number * 2), (uint8_t *) &(bitstream_segment->crc), 2);
}

uint8_t check_segment_integrity(uint16_t crc)
{
    dma1_setup((uint32_t) FRAM_segment.data);
    crc_init();
    dma1_start();

    while(dma1_ready() == 0);

    return crc_compare(crc);
}

void check_bitstreams_integrity(uint32_t bitstream_address[3])
{
    uint8_t integrity1, integrity2, integrity3;

    for(FRAM_segment.sequence_number = 0;
        FRAM_segment.sequence_number <= (IMAGE_MAX_SIZE/BITSTREAM_SEGMENT_DATA_SIZE);
        FRAM_segment.sequence_number++)
    {
        read_segment_from_memory(&FRAM_segment, bitstream_address[0]);
        integrity1 = check_segment_integrity(FRAM_segment.crc);
        read_segment_from_memory(&FRAM_segment, bitstream_address[1]);
        integrity2 = check_segment_integrity(FRAM_segment.crc);
        read_segment_from_memory(&FRAM_segment, bitstream_address[2]);
        integrity3 = check_segment_integrity(FRAM_segment.crc);

        uint8_t integrity = (integrity1 == CRC_VALID)<<2 | (integrity2 == CRC_VALID)<<1 | (integrity3 == CRC_VALID);

        switch(integrity)
        {
        case 0b000: //none is valid
            break;
        case 0b001:
        case 0b010:
        case 0b011:
        case 0b100:
        case 0b101:
        case 0b110:
            if(integrity1 == CRC_INVALID)
            {
                if(integrity2 == CRC_INVALID)
                {
                    //correct 1 and 2 with 3
                    read_segment_from_memory(&FRAM_segment, bitstream_address[2]);
                    write_segment_in_memory(&FRAM_segment, bitstream_address[0]);
                    write_segment_in_memory(&FRAM_segment, bitstream_address[1]);
                }
                else
                {
                    //correct 1 with 2
                    read_segment_from_memory(&FRAM_segment, bitstream_address[1]);
                    write_segment_in_memory(&FRAM_segment, bitstream_address[0]);
                    if(integrity3 == CRC_INVALID)
                    {
                        //correct 3 with 2
                        write_segment_in_memory(&FRAM_segment, bitstream_address[2]);
                    }
                }
            }
            else
            {
                read_segment_from_memory(&FRAM_segment, bitstream_address[0]);
                if(integrity2 == CRC_INVALID)
                {
                    //correct 2 with 1
                    write_segment_in_memory(&FRAM_segment, bitstream_address[1]);
                }
                if(integrity3 == CRC_INVALID)
                {
                    //correct 3 with 1
                    write_segment_in_memory(&FRAM_segment, bitstream_address[2]);
                }
            }

            break;
        case 0b111: //everything is fine
            break;
        default: //something is very wrong
            break;
        }

        watchdog_timer_reset_counter();
    }
    //tem que ter um lugar para contar a quantidade de erros encontrados e quantidade corrigida
}

uint8_t check_active_bitstream_integrity(void)
{
    uint8_t integrity = CRC_INVALID;
    uint8_t status = RUNNING_IMAGE_OK;

    for(FRAM_segment.sequence_number = 0;
        FRAM_segment.sequence_number <= (IMAGE_MAX_SIZE/BITSTREAM_SEGMENT_DATA_SIZE);
        FRAM_segment.sequence_number++)
    {
        read_segment_from_memory(&FRAM_segment, RUNNING_IMAGE_ADDRESS);
        integrity = check_segment_integrity(FRAM_segment.crc);

        if(integrity != CRC_VALID)
        {
            status = RUNNING_IMAGE_CORRUPTED;
        }

        watchdog_timer_reset_counter();
    }

    return status;
}

void bitstream_segment_upload(uint8_t *received_data)
{
    uint8_t i;
    uint8_t integrity = CRC_INVALID;
    uint8_t *pointer = (uint8_t *) &FRAM_segment;

    for(i = 0; i < BITSTREAM_SEGMENT_SIZE; i++)
    {
        pointer[i] = received_data[i];
    }

    integrity = check_segment_integrity(FRAM_segment.crc);

    if(integrity == CRC_VALID)
    {
        write_segment_in_memory(&FRAM_segment, IMAGE_A_COPY_1_ADDRESS); //todo:trocar esse endereco
    }
}

void housekeeping(void)
{
    uint32_t bitstream_address[3];
    payload2_uplink_t actual_command;


    while(1)
    {
        bitstream_address[0] = IMAGE_A_COPY_1_ADDRESS;
        bitstream_address[1] = IMAGE_A_COPY_2_ADDRESS;
        bitstream_address[2] = IMAGE_A_COPY_3_ADDRESS;

        check_bitstreams_integrity(bitstream_address);

        bitstream_address[0] = IMAGE_B_COPY_1_ADDRESS;
        bitstream_address[1] = IMAGE_B_COPY_2_ADDRESS;
        bitstream_address[2] = IMAGE_B_COPY_3_ADDRESS;


        check_bitstreams_integrity(bitstream_address);

        if(i2c_flag & I2C_FLAG_RX)
        {
            i2c_flag &= ~I2C_FLAG_RX;
            actual_command = i2c_rx_buffer;
            DMA0CTL |= DMAEN;

            switch (actual_command.type)
            {
            case PAYLOAD2_CCSDS_TELECOMMAND:
                break;
            case PAYLOAD2_BITSTREAM_UPLOAD:
                break;
            case PAYLOAD2_BITSTREAM_SWAP:
                break;
            case PAYLOAD2_BITSTREAM_STATUS_REQUEST:
                break;
            case PAYLOAD2_NO_PENDING_DATA:
            default:
                break;
            }
        }

//        if()
//        {
//
//        }


//
//        change state
//        if(i2c_flag & I2C_FLAG_STOP)
//        {
//            if(DMA0_received_data_length() == TELECOMMAND_TO_UTMC_SIZE)
//            {
//
//            }//todo: zerar contador do dma
//            if(DMA0_received_data_length() == BITSTREAM_SEGMENT_SIZE)
//            {
//
//            }
//            if(DMA0_received_data_length() == DATA_REQUEST_SIZE)
//            {
//
//            }
//            switch(command)
//            {
//            case COMMAND_BITSTREAM_UPLOAD:
//                bitstream_segment_upload();
//                break;
//            case COMMAND_BITSTREAM_STATUS_REQUEST:
//
//                break;
//            case COMMAND_BITSTREAM_SWAP_VERSION:
//                break;
//            case COMMAND_TELECOMMAND_TO_UTMC:
//                break;
//            case COMMAND_READ_DATA:
//                break;
//            }
//        }
//
//        if(uart_flag & UART_FLAG_RX)
//        {
//            split_utmc_telemetry();
//        }
    }
}

void split_utmc_telemetry()
{
    uint16_t sequence_number;

    for(sequence_number = 0; sequence_number < NUMBER_OF_UTMC_SEGMENTS; sequence_number++)
        i2c_tx_buffer[sequence_number].type = PAYLOAD2_NO_PENDING_DATA;

    for(sequence_number = 0; sequence_number < NUMBER_OF_UTMC_SEGMENTS; sequence_number++)
    {
//        FRAM_utmc_tm_to_obdh[sequence_number * TELEMETRY_FROM_UTMC_SIZE] = sequence_number;
//        memcpy(FRAM_utmc_tm_to_obdh + sequence_number * TELEMETRY_FROM_UTMC_SIZE + 1,
//                                 uart_buffer + sequence_number * CCSDS_SUBFRAME_SIZE,
//                                                                CCSDS_SUBFRAME_SIZE);

        i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment_number = sequence_number;
        memcpy( (void*) i2c_tx_buffer[sequence_number].data.ccsds_telemetry.segment,
                (const void*) (uart_buffer + sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment) * sequence_number),
                sizeof(i2c_tx_buffer->data.ccsds_telemetry.segment));


        i2c_tx_buffer[1].type = PAYLOAD2_CCSDS_TELEMETRY;
    }
}



