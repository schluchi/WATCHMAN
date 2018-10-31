/*
 * AXIS_Peripheral.h
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#ifndef SRC_AXIS_PERIPHERAL_H_
#define SRC_AXIS_PERIPHERAL_H_


#include "xil_types.h"
#include "xaxidma.h"
#include "xparameters.h"

/*** Definition of Registers ****************************************/
#define	CONTROL_REG			0
#define	NBR_OF_PACKETS_REG	1
#define	CONTENT_PACKET_1	2
#define	MODE_REG			3

// Modes for MODE REGISTER
#define	RAMP_MODE			0
#define	SAW_MODE			1
#define	BIT_MODE			2

/*** Function prototype *********************************************/
int XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes);

#endif /* SRC_AXIS_PERIPHERAL_H_ */
