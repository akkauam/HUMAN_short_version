/*
 * i2c.h
 *
 *  Created on: 12 de jun de 2018
 *      Author: elder
 */
#ifndef DRIVER_I2C_H_
#define DRIVER_I2C_H_

#include <stdint.h>

#define I2C_FLAG_RX         0x01
#define I2C_FLAG_TX         0x02
#define I2C_FLAG_STOP       0x04

#define PAYLOAD2_CCSDS_TELECOMMAND          'T'    /**< command to request telecommand to playloadx*/
#define PAYLOAD2_BITSTREAM_UPLOAD           'U'    /**< command to request playloadx bitstream upload*/
#define PAYLOAD2_BITSTREAM_SWAP             'V'    /**< command to request a swap version of payloadx bitstream*/
#define PAYLOAD2_BITSTREAM_STATUS_REQUEST   'S'    /**< command to request the status of bitstream frames*/
#define PAYLOAD2_BITSTREAM_STATUS_REPLAY    'S'
#define PAYLOAD2_CCSDS_TELEMETRY            'T'
#define PAYLOAD2_NO_PENDING_DATA            'N'

void i2c0_setup();

#endif /* DRIVER_I2C_H_ */
