/*
 * boot.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#include "../device/mcu.h"
#include "../src/human.h"
#include "boot.h"

void mcu_boot(void)
{
    setup_hardware();

    if(check_active_bitstream_integrity() == RUNNING_IMAGE_CORRUPTED)
    {
        swap_image_in_use(CHOOSE_IMAGE_A);

        if(check_active_bitstream_integrity() == RUNNING_IMAGE_CORRUPTED)
        {
            swap_image_in_use(CHOOSE_IMAGE_B);
        }
    }

    update_target();

    //checks if there is an error using this configuration

    enable_interrupts();
}

