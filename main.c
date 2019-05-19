/**
 * main.c
 */

#include <msp430.h>
#include "src/boot.h"
#include "src/human.h"
#include "hal.h"

int main(void)
{

    mcu_boot();

    housekeeping();

    //It will never reach here

    return 0;
}




