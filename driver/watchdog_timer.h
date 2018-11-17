/*
 * watchdog_timer.h
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#ifndef DRIVER_WATCHDOG_TIMER_H_
#define DRIVER_WATCHDOG_TIMER_H_

void watchdog_timer_setup(void);
void watchdog_timer_reset_counter(void);
void watchdog_timer_release_counter(void);

#endif /* DRIVER_WATCHDOG_TIMER_H_ */
