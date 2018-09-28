/*
 * update_manager.h
 *
 *  Created on: 3 de set de 2018
 *      Author: elder
 */
#ifndef SRC_UPDATE_MANAGER_H_
#define SRC_UPDATE_MANAGER_H_

#include "stdint.h"

void update_target(void);
void handle_new_bitstream_segment(uint8_t *new_segment, uint16_t sequence_number);
void swap_image_in_use(uint8_t version);

#endif /* SRC_UPDATE_MANAGER_H_ */
