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
#include "data_analysis.h"

#define SIZE_DATA_ARRAY 518*4 // int32_t is 4 bytes

#define TRIG_SHIFT		0
#define LAST_SHIFT		4
#define	TOO_LONG_SHIFT	8
#define MASK_INFO		0xF

typedef struct data_axi_st{
	uint64_t wdo_time;
	uint64_t dig_time;
	uint32_t info;
	uint32_t wdo_id;
	uint32_t data[16][32];
}data_axi;

typedef union data_axi_union{
	struct data_axi_st data_struct;
	uint32_t data_array[SIZE_DATA_ARRAY];
}data_axi_un;

typedef struct data_list_st data_list;
struct data_list_st{
	data_axi_un data;
	data_list* next;
};

/*** Function prototype *********************************************/
void XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes);
void dma_first_adress(void);
int dma_received_data(void);

#endif /* SRC_AXIS_PERIPHERAL_H_ */
