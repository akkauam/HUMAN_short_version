/*
 * project_definitions.h
 *
 *  Created on: 28 de ago de 2018
 *      Author: elder
 */
#ifndef MISC_PROJECT_DEFINITIONS_H_
#define MISC_PROJECT_DEFINITIONS_H_

#include <stdint.h>

#define CCSDS_FRAME_SIZE            1119
#define CCSDS_SUBFRAME_SIZE         192     //  = 1119 / 6, rounded up some bytes
#define CCSDS_SUBFRAMES             6

#define HUMAN_DATA_SIZE             79


typedef struct {
    uint8_t data[HUMAN_DATA_SIZE];
    uint8_t image_version;
    uint8_t sequence_number[2];
    uint8_t crc[2];
} bitstream_segment_t;


//memory position


#endif /* MISC_PROJECT_DEFINITIONS_H_ */
