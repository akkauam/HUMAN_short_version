/*
 * watchdog_timer.c
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#include "msp430.h"
#include "watchdog_timer.h"

void watchdog_timer_setup(void)
{
    WDTCTL = WDTPW | WDTCNTCL | WDTSSEL__ACLK | WDTIS_3;    // reset counter, clock based on SMCLK (32,768 kHz),
                                                             // 16 seconds until overflow
}

void watchdog_timer_reset_counter(void)
{
    WDTCTL = (WDTCTL & 0x00FF) + WDTPW + WDTCNTCL;
}

void watchdog_timer_release_counter(void)
{
    WDTCTL = ((WDTCTL & 0x00FF) + WDTPW) & ~WDTHOLD;
}
