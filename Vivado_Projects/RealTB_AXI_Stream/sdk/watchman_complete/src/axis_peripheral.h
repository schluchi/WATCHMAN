/*
 * axis_peripheral.h
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#ifndef SRC_AXIS_PERIPHERAL_H_
#define SRC_AXIS_PERIPHERAL_H_

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "xil_types.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "interrupt.h"
#include "xtime_l.h"

/*** Definition of Registers ****************************************/
#define	CONTROL_REG			0
#define	NBR_OF_PACKETS_REG	1
#define	CONTENT_PACKET_1	2
#define	MODE_REG			3

// Modes for MODE REGISTER
#define	RAMP_MODE			0
#define	SAW_MODE			1
#define	BIT_MODE			2

#define NBR_DATA	1000

typedef struct data_axi_st{
	int64_t wdo_time;
	int64_t dig_time;
	int32_t trigger;
	int32_t	wdo_id;
	int32_t data[16][16];
}data_axi;

typedef union data_axi_union{
	struct data_axi_st data_struct;
	int32_t data_array[262];
}data_axi_un;

typedef struct data_list_st data_list;
struct data_list_st{
	data_axi_un data;
	data_list* next;
};

/*** Function prototype *********************************************/
void XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes);
int dma_transfert(int start);

#endif /* SRC_AXIS_PERIPHERAL_H_ */
