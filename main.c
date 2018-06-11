/**
 * main.c
 */

#include "device/mcu.h"

void update_firmware();
void housekeeping();

int main(void)
{
    mcu_boot();

    update_firmware();

    housekeeping();

    //It will never reach here

    return 0;
}


void update_firmware()
{

}

void housekeeping()
{
    while(1)
    {
//        verify_image_integrity();

    }
}
