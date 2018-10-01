/*
 * mux.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#include "mux.h"
#include "../hal.h"

void mux_setup(void)
{
    MUX_SELECT_OUTPUT(HIGH);
    MUX_SELECT_SETUP();

    MUX_OUTPUT_ENABLE_N_OUTPUT(LOW);
    MUX_OUTPUT_ENABLE_N_SETUP();
}

void mux_select(uint8_t selected_device)
{
    if(selected_device == SELECT_HUMAN)
    {
        MUX_SELECT_OUTPUT(LOW);
    }
    else
    {
        MUX_SELECT_OUTPUT(HIGH);
    }
}
