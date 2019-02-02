/*
 * AXIS_Peripheral.c
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#include "axis_peripheral.h"

/* Extern global variables */
extern XAxiDma AxiDmaInstance;
extern data_list* first_element;
extern data_list* last_element;
extern char* frame_buf;
extern int* regptr;
extern volatile bool flag_axidma_error;
extern volatile bool flag_axidma_rx_done;
extern volatile bool empty_flag;
extern volatile bool flag_ttcps_timer;
extern volatile bool flag_scu_timer;
extern XScuWdt WdtScuInstance;

/****************************************************************************
*
* This is the DMA Simple Packet Transfert Function
*
******************************************************************************/
void XAxiDma_SimpleTransfer_hm(UINTPTR BuffAddr, int LengthOfBytes)
{
	uint32_t reg;

	// Update the memory with the value in the cache
	Xil_DCacheFlushRange(BuffAddr, LengthOfBytes);
	// Pass the memory address to the DMA
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_OFFSET, LOWER_32_BITS(BuffAddr));
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_MSB_OFFSET, 0);
	// Enable the DMA
	reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_CR_OFFSET);
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_CR_OFFSET,reg | XAXIDMA_CR_RUNSTOP_MASK);
	// Pass the size of the memory location which will start the DMA
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_BUFFLEN_OFFSET, LengthOfBytes);
}

void dma_received_data(int pmt){
	data_list* tmp_first_element;
	data_list* tmp_last_element;
	data_list* tmp_last_element_next;
	data_list* next_ele;
	data_list* prev_ele;
	uint32_t info, mask;
	bool flag = true;
	char nbr_wdo = 1;
	uint16_t data[32*MAX_WINDOW];
	int ch, length, i;
	features_ext features;

	tmp_first_element = first_element;
	while(flag){
		info = tmp_first_element->data.data_struct.info;
		mask = 0x1 << (TRIG_SHIFT+pmt);
		if((info & mask) != 0) flag = false;
		else tmp_first_element = tmp_first_element->next;
	}

	flag = true;
	tmp_last_element = tmp_first_element;
	while(flag){
		info = tmp_last_element->data.data_struct.info;
		mask = 0x1 << (LAST_SHIFT+pmt);
		if((info & mask) != 0) flag = false;
		else{
			nbr_wdo++;
			tmp_last_element = tmp_last_element->next;
		}
	}

	ch = correct_data(data, pmt, nbr_wdo, &info, tmp_first_element);
	mask = 0x1 << (TOO_LONG_SHIFT+pmt);
	length = 32 * nbr_wdo;
	if((info & mask) != 0){
		//send full wave form
		printf("send full waveform/r/n");
		length = length*2;
		frame_buf[0] = 0x55;
		frame_buf[1] = 0xAA;
		frame_buf[2] = (char)(length+15);
		frame_buf[3] = (char)((length+15) >> 8);
		frame_buf[4] = (FULL_WAVEFORM_ID << 7) + (nbr_wdo << 4) + (ch);
		for(i=0; i<8; i++) frame_buf[5+i] = (char)(tmp_first_element->data.data_struct.wdo_time >> i*8);
		for(i=0; i<length; i++){
			frame_buf[13+i] = (char)data[i/2];
			i++;
			frame_buf[13+i] = (char)((int)data[i/2] >> 8);
		}
		frame_buf[length+13] = 0x33;
		frame_buf[length+14] = 0xCC;
		transfer_data(frame_buf, (length+15));
	}
	else{
		//find amplitude and time, and send
		extract_features(data, length, &features);
		printf("amplitude = %d | time = %f\r\n", features.amplitude, features.time.time_fl);
		frame_buf[0] = 0x55;
		frame_buf[1] = 0xAA;
		frame_buf[2] = 21;
		frame_buf[3] = 0;
		frame_buf[4] = (FEATURES_ID << 7) + (nbr_wdo << 4) + (ch);
		for(i=0; i<8; i++) frame_buf[5+i] = (char)(tmp_first_element->data.data_struct.wdo_time >> (i*8));
		frame_buf[13] = (char)features.amplitude;
		frame_buf[14] = (char)(features.amplitude >> 8);
		for(i=0; i<4; i++) frame_buf[15+i] = (char)(features.time.time_t >> (i*8));
		frame_buf[19] = 0x33;
		frame_buf[20] = 0xCC;
		transfer_data(frame_buf, 21);
	}

	tmp_last_element_next = tmp_last_element->next;
	do{
		mask = ~((0x1 << (TRIG_SHIFT+pmt)) + (0x1 << (LAST_SHIFT+pmt)) + (0x1 << (TOO_LONG_SHIFT+pmt)));
		info = tmp_first_element->data.data_struct.info & mask;
		if(!(info & (MASK_INFO << TRIG_SHIFT))){
			// if tmp_first_element == first_element
			if (tmp_first_element->previous == NULL){
				//Removing first element
				next_ele = tmp_first_element->next;
				next_ele->previous = NULL;
				//Remove
				free(tmp_first_element);
				//Update first_element and ptr_first_element
				first_element = next_ele;
				tmp_first_element = next_ele;
			}
			else{
				//Removing middle element
				next_ele = tmp_first_element->next;
				prev_ele = tmp_first_element->previous;
				//Update
				next_ele->previous = prev_ele;
				prev_ele->next = next_ele;
				//Remove
				free(tmp_first_element);
				//Update
				tmp_first_element = next_ele;
			}
		}
		else{
			tmp_first_element->data.data_struct.info = info;
			next_ele = tmp_first_element->next;
			tmp_first_element = next_ele;
		}
	}while(tmp_first_element != tmp_last_element_next);
	if(first_element == last_element) empty_flag = true;
	else empty_flag = false;
}

//
//void dma_received_data(int pmt){
//	data_list* tmp_first_element;
//	data_list* tmp_last_element;
//	data_list* tmp_ptr;
//	uint32_t info, mask;
//	bool flag = true;
//	char nbr_wdo = 1;
//	uint16_t data[32*MAX_WINDOW];
//	int ch, length, i;
//	features_ext features;
//
//	tmp_first_element = first_element;
//	while(flag){
//		info = tmp_first_element->data.data_struct.info;
//		mask = 0x1 << (TRIG_SHIFT+pmt);
//		if((info & mask) != 0) flag = false;
//		else tmp_first_element = tmp_first_element->next;
//	}
//
//	flag = true;
//	tmp_last_element = tmp_first_element;
//	while(flag){
//		info = tmp_last_element->data.data_struct.info;
//		mask = 0x1 << (LAST_SHIFT+pmt);
//		if((info & mask) != 0) flag = false;
//		else{
//			nbr_wdo++;
//			tmp_last_element = tmp_last_element->next;
//		}
//	}
//
//	ch = correct_data(data, pmt, nbr_wdo, &info, tmp_first_element);
//	mask = 0x1 << (TOO_LONG_SHIFT+pmt);
//	length = 32 * nbr_wdo;
//	if((info & mask) != 0){
//		//send full wave form
//		printf("send full waveform/r/n");
//		length = length*2;
//		frame_buf[0] = 0x55;
//		frame_buf[1] = 0xAA;
//		frame_buf[2] = (char)(length+15);
//		frame_buf[3] = (char)((length+15) >> 8);
//		frame_buf[4] = (FULL_WAVEFORM_ID << 7) + (nbr_wdo << 4) + (ch);
//		for(i=0; i<8; i++) frame_buf[5+i] = (char)(tmp_first_element->data.data_struct.wdo_time >> i*8);
//		for(i=0; i<length; i++){
//			frame_buf[13+i] = (char)data[i/2];
//			i++;
//			frame_buf[13+i] = (char)((int)data[i/2] >> 8);
//		}
//		frame_buf[length+13] = 0x33;
//		frame_buf[length+14] = 0xCC;
//		transfer_data(frame_buf, (length+15));
//	}
//	else{
//		//find amplitude and time, and send
//		extract_features(data, length, &features);
//		printf("amplitude = %d | time = %f\r\n", features.amplitude, features.time.time_fl);
//		frame_buf[0] = 0x55;
//		frame_buf[1] = 0xAA;
//		frame_buf[2] = 21;
//		frame_buf[3] = 0;
//		frame_buf[4] = (FEATURES_ID << 7) + (nbr_wdo << 4) + (ch);
//		for(i=0; i<8; i++) frame_buf[5+i] = (char)(tmp_first_element->data.data_struct.wdo_time >> (i*8));
//		frame_buf[13] = (char)features.amplitude;
//		frame_buf[14] = (char)(features.amplitude >> 8);
//		for(i=0; i<4; i++) frame_buf[15+i] = (char)(features.time.time_t >> (i*8));
//		frame_buf[19] = 0x33;
//		frame_buf[20] = 0xCC;
//		transfer_data(frame_buf, 21);
//	}
//
//	if(tmp_first_element == first_element){
//		tmp_ptr = first_element;
//		flag = true;
//	}
//	else{
//		tmp_ptr = tmp_first_element->previous;
//		flag = false;
//	}
//	do{
//		mask = ~((0x1 << (TRIG_SHIFT+pmt)) + (0x1 << (LAST_SHIFT+pmt)) + (0x1 << (TOO_LONG_SHIFT+pmt)));
//		info = tmp_first_element->data.data_struct.info & mask;
//		if(!(info & (MASK_INFO << TRIG_SHIFT))){
//			tmp_ptr->next = tmp_first_element->next;
//			free(tmp_first_element);
//			tmp_first_element = tmp_ptr->next;
//			tmp_first_element->previous = tmp_ptr;
//		}
//		else{
//			tmp_first_element->data.data_struct.info = info;
//			tmp_ptr = tmp_first_element;
//			tmp_first_element = tmp_ptr->next;
//		}
//	}while(tmp_first_element != tmp_last_element->next);
//	if(flag){
//		first_element = tmp_ptr;
//		first_element->previous = 0;
//	}
//}

int test_TPG(void){
	int timeout,i,j;
	data_list* tmp_ptr  = (data_list *)malloc(sizeof(data_list));
	if(!tmp_ptr){
		printf("malloc for tmp_ptr failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	tmp_ptr->next = NULL;
	tmp_ptr->previous = NULL;

	XAxiDma_SimpleTransfer_hm((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

	regptr[TC_FSTWINDOW_REG] = 10;
	regptr[TC_NBRWINDOW_REG] = 1;
	WriteRegister(TC_TPG_REG,	0x50A);	// TPG value

	ControlRegisterWrite(SMODE_MASK ,ENABLE);
	ControlRegisterWrite(SS_TPG_MASK ,DISABLE); // disable for TestPattern
	ControlRegisterWrite(WINDOW_MASK,ENABLE);
	usleep(50);
	ControlRegisterWrite(WINDOW_MASK,DISABLE); // PL side starts on falling edge

	timeout = 200000;
	do{
		if(flag_ttcps_timer){
			update_timefile();
			flag_ttcps_timer = false;
		}

		if(flag_scu_timer){
			XScuWdt_RestartWdt(&WdtScuInstance);	// Reload the counter for the wdt
			flag_scu_timer = false;
		}
		usleep(50);
		timeout--;
		if(flag_axidma_error){
			printf("Error with DMA interrupt: TPG !\r\n");
			return XST_FAILURE;
		}
	}while(timeout && !flag_axidma_rx_done);

	if(timeout <= 0){
		printf("Timeout: TPG failed!\r\n");
		return XST_FAILURE;
	}
	else flag_axidma_rx_done = false;

	Xil_DCacheInvalidateRange((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);


	if(tmp_ptr->data.data_struct.wdo_id == 10){
		for(j=0; j<32; j++){
			for(i=0; i<16; i++){
				if(tmp_ptr->data.data_struct.data[i][j] != 0x50A){
					printf("wdo_time: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_time);
					printf("PL_spare: %d\r\n", (uint)tmp_ptr->data.data_struct.PL_spare);
					printf("info: 0x%X\r\n", (uint)tmp_ptr->data.data_struct.info);
					printf("wdo_id: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_id);
					for(j=0; j<32; j++){
						for(i=0; i<16; i++){
							printf("%d\t", (uint)tmp_ptr->data.data_struct.data[i][j]);
						}
						printf("\r\n");
					}
					printf("TPG failed: data wrong!\r\n");
					return XST_FAILURE;
				}
			}
		}
	}
	else{
		printf("wdo_time: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_time);
		printf("PL_spare: %d\r\n", (uint)tmp_ptr->data.data_struct.PL_spare);
		printf("info: 0x%X\r\n", (uint)tmp_ptr->data.data_struct.info);
		printf("wdo_id: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_id);
		for(j=0; j<32; j++){
			for(i=0; i<16; i++){
				printf("%d\t", (uint)tmp_ptr->data.data_struct.data[i][j]);
			}
			printf("\r\n");
		}
		printf("TPG failed: window id wrong!\r\n");
		return XST_FAILURE;
	}
	free(tmp_ptr);

	ControlRegisterWrite(PSBUSY_MASK,DISABLE);

	return XST_SUCCESS;
}
