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

#define BITSTREAM_SEGMENT_SIZE      79


typedef struct {
    uint8_t data[BITSTREAM_SEGMENT_SIZE];
    uint8_t image_version;
    uint8_t sequence_number[2];
    uint8_t crc[2];
} bitstream_segment_t;


//memory position  in kB
#define IMAGE_MAX_SIZE              (1572864)   //1,5 MB
#define METADATA_MAX_SIZE           (65536)
#define SPARE_SIZE                  (458752)
#define COPY_SIZE                   (IMAGE_MAX_SIZE + METADATA_MAX_SIZE + SPARE_SIZE)

#define CURRENT_IMAGE_ADDRESS       (0)
#define CURRENT_METADATA_ADDRESS    (CURRENT_IMAGE_ADDRESS + IMAGE_MAX_SIZE)

#define IMAGE_A_COPY_1_ADDRESS      (COPY_SIZE)
#define METADATA_A_COPY_1_ADDRESS   (IMAGE_A_COPY_1_ADDRESS + IMAGE_MAX_SIZE)

#define IMAGE_A_COPY_2_ADDRESS      (2 * COPY_SIZE)
#define METADATA_A_COPY_2_ADDRESS   (IMAGE_A_COPY_2_ADDRESS + IMAGE_MAX_SIZE)

#define IMAGE_A_COPY_3_ADDRESS      (3 * COPY_SIZE)
#define METADATA_A_COPY_3_ADDRESS   (IMAGE_A_COPY_3_ADDRESS + IMAGE_MAX_SIZE)

#define IMAGE_B_COPY_1_ADDRESS      (4 * COPY_SIZE)
#define METADATA_B_COPY_1_ADDRESS   (IMAGE_B_COPY_1_ADDRESS + IMAGE_MAX_SIZE)

#define IMAGE_B_COPY_2_ADDRESS      (5 * COPY_SIZE)
#define METADATA_B_COPY_2_ADDRESS   (IMAGE_B_COPY_2_ADDRESS + IMAGE_MAX_SIZE)

#define IMAGE_B_COPY_3_ADDRESS      (6 * COPY_SIZE)
#define METADATA_B_COPY_3_ADDRESS   (IMAGE_B_COPY_3_ADDRESS + IMAGE_MAX_SIZE)


#endif /* MISC_PROJECT_DEFINITIONS_H_ */
