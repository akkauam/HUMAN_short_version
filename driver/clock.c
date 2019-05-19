/*
 * clock.c
 *
 *  Created on: 11 de jun de 2018
 *      Author: elder
 */

#include "../hal.h"
#include "clock.h"

#define     XT_MAX_STARTUP_TIME     10000

void setup_dco(void)
{
    CSCTL1 = DCOFSEL_4 | DCORSEL;            // Set DCO to 16MHz
}

void setup_xt1_xt2(void)
{
    LFXIN_SETUP();
    LFXOUT_SETUP();

    HFXIN_SETUP();
    HFXOUT_SETUP();

    CSCTL4 |= LFXTDRIVE_3 | HFXTDRIVE_3;
    CSCTL4 &= ~(LFXTOFF | HFXTOFF);
}

void setup_clk_sources(void)
{
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    //todo: trocar. (um não usa cristal - launchpad){ 
    //CSCTL2 = SELA__LFXTCLK | SELS__HFXTCLK | SELM__HFXTCLK; 
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    //todo: trocar } 
}

uint8_t test_fault_flags(void)
{
    volatile uint16_t attempts = 0;
    uint8_t result = 0xFF;

    do
    {
        __delay_cycles(4000);
        CSCTL5 &= ~(LFXTOFFG | HFXTOFFG);       // Clear XT1 and XT2 fault flag
        SFRIFG1 &= ~OFIFG;
        attempts++;
    } while((SFRIFG1 & OFIFG)  && (attempts < XT_MAX_STARTUP_TIME) );
    /**< try for 2,5 seconds. The datasheet says 1s of typical Startup time. */

    if( attempts < XT_MAX_STARTUP_TIME)
    {
        result = SETUP_CLOCK_SUCESS;
    }
    else
    {
        result = SETUP_CLOCK_FAIL;
    }
    return result;
}

void unlock_registers(void)
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    CSCTL0_H = CSKEY >> 8;
}

void lock_registers(void)
{
    CSCTL0_H = 0;
}

uint8_t clock_setup(void)
{
    uint8_t test_flag;

    unlock_registers();

    setup_dco();
    //todo: descomentar
    setup_xt1_xt2();

    setup_clk_sources();

    test_flag = test_fault_flags();

    lock_registers();

    return test_flag;
}
