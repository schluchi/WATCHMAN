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

//#include "xil_types.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "interrupt.h"
#include "xtime_l.h"
#include "data_analysis.h"

#define FEATURES_ID			0
#define FULL_WAVEFORM_ID	1

/*** Function prototype *********************************************/
void XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes);
void dma_first_adress(void);
void dma_received_data(int group);

#endif /* SRC_AXIS_PERIPHERAL_H_ */
