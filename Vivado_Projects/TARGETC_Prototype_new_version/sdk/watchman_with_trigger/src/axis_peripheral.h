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
#include "xstatus.h"
#include "data_analysis.h"
#include "TARGETC_RegisterMap.h"
#include "file_hm.h"
#include "xscuwdt.h"

#define FEATURES_ID			0
#define FULL_WAVEFORM_ID	1

/*** Function prototype *********************************************/
void XAxiDma_SimpleTransfer_hm(UINTPTR BuffAddr, int LengthOfBytes);
void dma_received_data(int pmt);
int test_TPG(void);

#endif /* SRC_AXIS_PERIPHERAL_H_ */
