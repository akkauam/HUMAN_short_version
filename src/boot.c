/*
 * boot.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#include "../device/mcu.h"
#include "../src/human.h"
#include "boot.h"
#include "hal.h"

void mcu_boot(void)
{
    setup_hardware();

    update_target();

//    READ_BRAVE_CONFIG_ERROR();//checks if there is an error using this configuration

    enable_interrupts();
}

