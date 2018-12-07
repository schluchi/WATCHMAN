/*
 * AXIS_Peripheral.c
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#include "axis_peripheral.h"

/* Extern lobal variables */
extern char axidma_error;
extern char axidma_rx_done;
extern XAxiDma AxiDmaInstance;
extern data_list* first_element;
extern data_list* last_element;

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

void dma_first_adress(void){
	// set pulse
	XAxiDma_SimpleTransfer_Hej(&AxiDmaInstance,(UINTPTR)first_element->data.data_array, SIZE_DATA_ARRAY);
	// Reset pulse
}
int dma_received_data(void){
	static char group_nbr_wdo[4] = {0, 0, 0, 0};
	int group, ch;
	data_list* tmp_ptr_1;
	data_list* tmp_ptr_2;
	uint32_t info, mask;
	bool first = false;
	uint16_t* data;
	// set pulse
	Xil_DCacheInvalidateRange((UINTPTR)last_element->data.data_array, SIZE_DATA_ARRAY);

	for(group=0; group<4; group++){
		info = last_element->data.data_struct.info;
		mask = 0x1 << (TRIG_SHIFT+group);
		if((info && mask) != 0) group_nbr_wdo[group]++;
		mask = 0x1 << (LAST_SHIFT+group);
		if((info && mask) != 0){
			mask = 0x1 << (TOO_LONG_SHIFT+group);
			data = (uint16_t *)malloc(2*32*group_nbr_wdo[group]);
			if(!data){
				xil_printf("malloc for data failed in function, %s!\r\n", __func__);
				return XST_FAILURE;
			}
			// pedestal substraction and transfert function
			// choose the gain stage (wich channel in the group of 4, (CH0-3, CH4-7,... | High gain on smallest channel of each group (CH0,CH4,...))
			ch = correct_data(data, group, group_nbr_wdo[group], &info);
			if((info && mask) != 0){
				//send full wave form
			}
			else{
				//find amplitude and time
			}
			tmp_ptr_1 = first_element;
			do{
				tmp_ptr_2 = tmp_ptr_1->next;
				info = tmp_ptr_1->data.data_struct.info & ((~(0x1 << (TRIG_SHIFT+group))) | (~(0x1 << (LAST_SHIFT+group))) | (~(0x1 << (TOO_LONG_SHIFT+group))));
				if(!((info >> TRIG_SHIFT) && MASK_INFO)) free(tmp_ptr_1);
				else{
					tmp_ptr_1->data.data_struct.info = info;
					if(first){
						first_element = tmp_ptr_1;
						first = false;
					}
				}
				tmp_ptr_1 = tmp_ptr_2;
			}while(tmp_ptr_1 != NULL);
			group_nbr_wdo[group] = 0;
			free(data);
		}
	}

	tmp_ptr_1 = last_element;
	last_element = malloc(sizeof(data_list));
	if(!last_element){
		xil_printf("malloc for last_element failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	last_element->next = NULL;
	tmp_ptr_1->next = last_element;
	XAxiDma_SimpleTransfer_Hej(&AxiDmaInstance,(UINTPTR)last_element->data.data_array, SIZE_DATA_ARRAY);
	// reset pulse
	return XST_SUCCESS;
}
