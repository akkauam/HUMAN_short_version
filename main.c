/**
 * main.c
 */

#include "src/boot.h"
#include "src/housekeeper.h"
#include "src/update_manager.h"


int main(void)
{
    mcu_boot();

    housekeeping();

    //It will never reach here

    return 0;
}




