/*
 * data_test.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Jose Idlabs
 */

#ifndef SRC_DATA_TEST_H_
#define SRC_DATA_TEST_H_

#include <stdio.h>
#include <string.h>
#include "lwip/err.h"
#include "lwip/udp.h"
#include "xil_printf.h"
#define CHANNEL 16
#define WINDOW 4
#define SAMPLE 32
#define FRAME WINDOW*SAMPLE
/*
 * 12 bits/samples -> 16 bits = 2bytes
 * 32 sampl/wind * 4 wind/ch * 2 bytes/sample = 256
 * 256 + 1 (ch. n°) + 1 (frame's type) = 258
 * 258 * 16 (ch.) + 4 (0x55AA <-> start | 0x33CC <-> end) + 2 (frame's length) = 4134
 * Real max value for UDP is 65507
 */
//#define MAX_STREAM_SIZE CHANNEL*(FRAME*2+2)+6

uint16_t made_frame(char stream[], uint16_t length);

#endif /* SRC_DATA_TEST_H_ */
