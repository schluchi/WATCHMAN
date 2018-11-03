/*
 * AXIS_Peripheral.c
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#include "axis_peripheral.h"

extern XAxiDma AxiDmaInstance;
extern char axidma_error, axidma_rx_done;
int PtrData[NBR_DATA];

/****************************************************************************
*
* This is the DMA Simple Packet Transfert Function
*
******************************************************************************/
void XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes)
{
	uint32_t reg;

	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_OFFSET, LOWER_32_BITS(BuffAddr));
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_MSB_OFFSET, 0);

	reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_CR_OFFSET);
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_CR_OFFSET,reg | XAXIDMA_CR_RUNSTOP_MASK);

	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_BUFFLEN_OFFSET, LengthOfBytes);
}

int dma_transfert(int start){
	int *regbank = XPAR_AXIS_TEST_COMPONENT_0_S00_AXI_BASEADDR;

	regbank[CONTROL_REG] = 0;
	regbank[NBR_OF_PACKETS_REG] = NBR_DATA;
	regbank[CONTENT_PACKET_1] = start;
	regbank[MODE_REG] = RAMP_MODE;
	xil_printf("Packet :\tNbr:%d\tContent:%d\tMode:%d\r\n",regbank[NBR_OF_PACKETS_REG],regbank[CONTENT_PACKET_1],regbank[MODE_REG]);

	for(int i=0; i < NBR_DATA;i++) PtrData[i] = 0;
	Xil_DCacheFlushRange((UINTPTR)PtrData, NBR_DATA*sizeof(int));
	XAxiDma_SimpleTransfer_Hej(&AxiDmaInstance,(UINTPTR)PtrData, (NBR_DATA*sizeof(int)));
	//*** ResetTimer *************************/
	int TimeOutCnt = 5;

	//*** Start Transfer *********************/
	regbank[CONTROL_REG] = 1;		//Enable Axi Peripheral to start sending data
	bool odd = true;
	while (!axidma_rx_done && !axidma_error && TimeOutCnt) {
			/* NOP */
			sleep(1);
			TimeOutCnt--;
			if (odd){
				//xil_printf("DMA start transfert:\r\n...\r\n");
				odd = false;
			}
			//else xil_printf("...\r\n");
	}
	if(TimeOutCnt == 0){
		//xil_printf(">> Time Out Reached 5sec\r\n");
		return XST_FAILURE;
	}
	else{
		if(axidma_rx_done){
			//xil_printf(">> RxDone Interrupt Caught\r\n");

			Xil_DCacheInvalidateRange((UINTPTR)PtrData, NBR_DATA*sizeof(int));

			int cst = regbank[CONTENT_PACKET_1];
			for(int a=0; a < NBR_DATA; a++){
				if(PtrData[a] != (a+cst)){
					//xil_printf("\tdata are wrong...\r\n");
					return XST_FAILURE;
				}
			}
			//xil_printf("\tdata are right...\r\n");
		}
		else{
			//xil_printf(">> Error\r\n");
			return XST_FAILURE;
		}
	}
	regbank[CONTROL_REG] = 0;		//Enable Axi Peripheral to start sending data

	axidma_error = 0;
	axidma_rx_done = 0;
	return XST_SUCCESS;
}
