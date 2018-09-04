/*
 * boot.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#include "../device/mcu.h"
#include "../src/update_manager.h"

void verify_image_integrity(void)
{

}

void enable_interrupts(void)
{

}

void mcu_boot(void)
{
    setup_hardware();

    verify_image_integrity();

    update_target();

    enable_interrupts();
}

