/*
 * AXIS_Peripheral.c
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#include "axis_peripheral.h"

/* Extern lobal variables */
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
	/*******************************************/
	// set pulse
	/*******************************************/
	XAxiDma_SimpleTransfer_Hej(&AxiDmaInstance,(UINTPTR)first_element->data.data_array, SIZE_DATA_ARRAY);
	/*******************************************/
	// reset pulse
	/*******************************************/
}

void dma_received_data(int group){
	data_list* tmp_first_element;
	data_list* tmp_last_element;
	data_list* tmp_ptr;
	uint32_t info, mask;
	bool flag = true;
	char nbr_wdo = 1;
	uint16_t data[32*MAX_WINDOW];
	int ch, length, i;
	features_ext features;

	tmp_first_element = first_element;
	while(flag){
		info = tmp_first_element->data.data_struct.info;
		mask = 0x1 << (TRIG_SHIFT+group);
		if((info && mask) != 0) flag = false;
		else tmp_first_element = tmp_first_element->next;
	}

	flag = true;
	tmp_last_element = tmp_first_element;
	while(flag){
		info = tmp_last_element->data.data_struct.info;
		mask = 0x1 << (LAST_SHIFT+group);
		if((info && mask) != 0) flag = false;
		else{
			nbr_wdo++;
			tmp_last_element = tmp_last_element->next;
		}
	}

	ch = correct_data(data, group, nbr_wdo, &info, tmp_first_element);
	mask = 0x1 << (TOO_LONG_SHIFT+group);
	length = 32 * nbr_wdo;
	if((info && mask) != 0){
		//send full wave form
		length = length*2;
		char* frame = (char *)malloc(length+15);
		frame[0] = 0x55;
		frame[1] = 0xAA;
		frame[2] = (char)(length+14);
		frame[3] = (char)((length+14) >> 8);
		frame[4] = (FULL_WAVEFORM_ID << 7) + (nbr_wdo << 4) + (ch);
		for(i=0; i<8; i++) frame[5+i] = (char)(tmp_first_element->data.data_struct.wdo_time >> i*8);
		for(i=0; i<length; i++){
			frame[13+i] = (char)data[i];
			i++;
			frame[13+i] = (char)((int)data[i] >> 8);
		}
		frame[length+13] = 0x33;
		frame[length+14] = 0xCC;
		transfer_data(frame, (length+15));
		free(frame);
	}
	else{
		//find amplitude and time, and send
		extract_features(data, length, &features);
		char* frame = (char *)malloc(21);
		frame[0] = 0x55;
		frame[1] = 0xAA;
		frame[2] = 21;
		frame[3] = 0;
		frame[4] = (FEATURES_ID << 7) + (nbr_wdo << 4) + (ch);
		for(i=0; i<8; i++) frame[5+i] = (char)(tmp_first_element->data.data_struct.wdo_time >> (i*8));
		frame[13] = (char)features.amplitude;
		frame[14] = (char)(features.amplitude >> 8);
		for(i=0; i<4; i++) frame[15+i] = (char)(features.time.time_t >> (i*8));
		frame[19] = 0x33;
		frame[20] = 0xCC;
		transfer_data(frame, 21);
		free(frame);
	}

	tmp_ptr = tmp_first_element->previous;
	do{
		info = tmp_first_element->data.data_struct.info & ((~(0x1 << (TRIG_SHIFT+group))) | (~(0x1 << (LAST_SHIFT+group))) | (~(0x1 << (TOO_LONG_SHIFT+group))));
		if(!(info && (MASK_INFO << TRIG_SHIFT))){
			tmp_ptr->next = tmp_first_element->next;
			free(tmp_first_element);
			tmp_first_element = tmp_ptr->next;
			tmp_first_element->previous = tmp_ptr;
		}
		else{
			tmp_ptr = tmp_first_element;
			tmp_first_element = tmp_ptr->next;
		}
	}while(tmp_first_element != tmp_last_element->next);
}
