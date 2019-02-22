/**
 * main.c
 */

#include <msp430.h>
#include "src/boot.h"
#include "src/human.h"

int main(void)
{
    //todo: verificar clocks
    mcu_boot();

    housekeeping();

    //It will never reach here

    return 0;
}




