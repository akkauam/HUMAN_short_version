/*
 * utmc.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#include "../hal.h"
#include "utmc.h"

void utmc_setup(void)
{
    UTMC_ERROR_SETUP();
    UTMC_RESET_N_OUTPUT(LOW);
    UTMC_RESET_N_SETUP();
}

uint8_t utmc_status(void)
{
    uint8_t status = UTMC_STATE_OK;

    if(READ_UTMC_ERROR() == HIGH)
    {
        status = UTMC_STATE_ERROR;
    }

    return status;
}
