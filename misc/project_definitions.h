/*
 * project_definitions.h
 *
 *  Created on: 28 de ago de 2018
 *      Author: elder
 */
#ifndef MISC_PROJECT_DEFINITIONS_H_
#define MISC_PROJECT_DEFINITIONS_H_

#include <stdint.h>

#define CCSDS_FRAME_SIZE                1119
#define CCSDS_SUBFRAME_SIZE             192     //  = 1119 / 6, rounded up some bytes
#define CCSDS_SUBFRAMES                 6

#define I2C_RX_BUF_SIZE                 BITSTREAM_SEGMENT_SIZE + 1 // + COMMAND

#define TELECOMMAND_TO_UTMC_SIZE        82
#define BITSTREAM_SEGMENT_DATA_SIZE     80
#define BITSTREAM_SEGMENT_SIZE          84
#define DATA_REQUEST_SIZE               1

#define TELEMETRY_FROM_UTMC_SEGMENT_SIZE (CCSDS_SUBFRAME_SIZE + 1)
#define TELEMETRY_FROM_UTMC_SIZE         (TELEMETRY_FROM_UTMC_SEGMENT_SIZE * NUMBER_OF_UTMC_SEGMENTS)
#define NUMBER_OF_UTMC_SEGMENTS          6
#define NUMBER_OF_STATUS_SEGMENTS        1

typedef struct {
    uint8_t type;
    union{
        struct{
            uint8_t segment[192];
            uint8_t segment_number;
        }ccsds_telemetry;
        struct {
            uint8_t status_segment[218];
            uint16_t segment_number;
        }bitstream_status_replay;
    }data;
} payload2_downlink_t;

typedef struct {
    uint8_t type;
    union{
        uint8_t bitstream_Upload[84];
        uint8_t ccsds_telecommand[82];
        uint8_t cmd;
    }data;
} payload2_uplink_t;

typedef union{
    uint8_t byte[1119];
    uint32_t compare;
} uart_buffer_t;

#define PAYLOAD2_CCSDS_TELECOMMAND          'T'    /**< command to request telecommand to playloadx*/
#define PAYLOAD2_BITSTREAM_UPLOAD           'U'    /**< command to request playloadx bitstream upload*/
#define PAYLOAD2_BITSTREAM_SWAP             'V'    /**< command to request a swap version of payloadx bitstream*/
#define PAYLOAD2_BITSTREAM_STATUS_REQUEST   'S'    /**< command to request the status of bitstream frames*/
#define PAYLOAD2_BITSTREAM_STATUS_REPLAY    'S'
#define PAYLOAD2_CCSDS_TELEMETRY            'T'
#define PAYLOAD2_NO_PENDING_DATA            'N'


//todo: trocar pelo ID da memória de voo { 
#define MEM_ID_BYTE_0                   0x9D
#define MEM_ID_BYTE_1                   0x60
#define MEM_ID_BYTE_2                   0x18
//todo: trocar pelo ID da memória de voo }

//#define IMAGE_A         0x0001
//#define IMAGE_B         0x0010
//#define IMAGE_CRC_OK    0x0100
//#define IMAGE_CRC_NOK   0x1000

typedef struct {
    uint8_t data[BITSTREAM_SEGMENT_DATA_SIZE];
//    uint8_t image_status;
    uint16_t sequence_number;
    uint16_t crc;
} bitstream_segment_t;




//memory position  in bytes
#define IMAGE_MAX_SIZE              (1572864)   //1,5 MB
#define METADATA_MAX_SIZE           (65536)
#define SPARE_SIZE                  (458752)
#define COPY_SIZE                   (IMAGE_MAX_SIZE + METADATA_MAX_SIZE + SPARE_SIZE)

#define RUNNING_IMAGE_ADDRESS       (0)
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
