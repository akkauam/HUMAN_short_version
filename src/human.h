/*
 * human.h
 *
 *  Created on: 4 de set de 2018
 *      Author: elder
 */
#ifndef SRC_HUMAN_H_
#define SRC_HUMAN_H_

#include <stdint.h>

#define RUNNING_IMAGE_OK            1
#define RUNNING_IMAGE_CORRUPTED     0

#define CHOOSE_IMAGE_A              1
#define CHOOSE_IMAGE_B              2

void update_target(void);
void housekeeping(void);
void check_bitstreams_integrity(uint32_t bitstream_address[3]);
uint8_t check_active_bitstream_integrity(void);
void swap_image_in_use(uint8_t version);

#endif /* SRC_HUMAN_H_ */


//The FRAM supports bit error correction and uncorrectable bit error detection. The UBDIFG FRAM
//uncorrectable bit error flag is set if an uncorrectable bit error has been detected in the FRAM error
//detection logic. The CBDIFG FRAM correctable bit error flag is set if a correctable bit error has been
//detected and corrected. UBDRSTEN enables a power up clear (PUC) reset. If an uncorrectable bit error is
//detected, UBDIE enables a NMI event if an uncorrectable bit error is detected. CBDIE enables an NMI
//event if a marginal correctable bit error is detected and corrected.
