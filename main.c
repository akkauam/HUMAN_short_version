/**
 * main.c
 */

#include "src/boot.h"
#include "src/human.h"


int main(void)
{
    mcu_boot();

    housekeeping();

    //It will never reach here

    return 0;
}




