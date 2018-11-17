/*
 * crc.c
 *
 *  Created on: 3 de jul de 2018
 *      Author: elder
 */

#include <msp430.h>
#include "crc.h"

#define CRC_SEED    (0xFFFF)     //CRC-16/CCITT-FALSE

void crc_init(void)
{
    CRCINIRES = CRC_SEED;
}

void crc_feed(uint16_t input)
{
    CRCDIRB = input;
    __no_operation();
}

uint16_t crc_result(void)
{
    return CRCINIRES;
}

uint8_t crc_compare(uint16_t value_to_compare)
{
    uint8_t status = CRC_INVALID;

    if(value_to_compare == crc_result())
    {
        status = CRC_VALID;
    }

    return status;
}
