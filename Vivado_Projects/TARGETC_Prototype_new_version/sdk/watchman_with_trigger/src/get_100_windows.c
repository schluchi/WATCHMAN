/*
 * get_1000_windows.c
 *
 *  Created on: 21 janv. 2019
 *      Author: antho
 */


#include "get_100_windows.h"

/* Extern global variables */
extern int* regptr;
extern volatile bool flag_axidma_rx_done;
extern uint16_t pedestal[512][16][32];
extern char* frame_buf;
extern uint16_t lookup_table[2048];


int get_100_windows_fct(void){
	int window = 0;
	int timeout;
	int k,i,j,index;
	uint16_t data_tmp;

	data_list* tmp_ptr  = (data_list *)malloc(sizeof(data_list));
	if(!tmp_ptr){
		printf("malloc for tmp_ptr failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	tmp_ptr->next = NULL;
	tmp_ptr->previous = NULL;

	window = 0;

	XAxiDma_SimpleTransfer_Hej((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

	regptr[TC_FSTWINDOW_REG] = window;
	regptr[TC_NBRWINDOW_REG] = 100;
	ControlRegisterWrite(SMODE_MASK ,ENABLE);
	ControlRegisterWrite(SS_TPG_MASK ,ENABLE);
	ControlRegisterWrite(WINDOW_MASK,ENABLE);
	usleep(50);
	ControlRegisterWrite(WINDOW_MASK,DISABLE); // PL side starts on falling edge

	for(k =0; k<100; k++){
		if(k != 0) XAxiDma_SimpleTransfer_Hej((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

		timeout = 200000; // 10sec
		while(timeout && !flag_axidma_rx_done){
			usleep(50);
			timeout--;
		}

		Xil_DCacheInvalidateRange((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

		if(timeout <= 0){
			printf("\r\nwindow = %d\r\n", window);
			printf("wdo_time: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_time);
			printf("dig_time: %d\r\n", (uint)tmp_ptr->data.data_struct.dig_time);
			printf("info: 0x%X\r\n", (uint)tmp_ptr->data.data_struct.info);
			printf("wdo_id: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_id);
			for(j=0; j<32; j++){
				for(i=0; i<16; i++){
					printf("%d\t", (uint)tmp_ptr->data.data_struct.data[i][j]);
				}
				printf("\r\n");
			}
			printf("Timeout on window %d: get 1000 windows failed!\r\n", window);
			return XST_FAILURE;
		}
		else flag_axidma_rx_done = false;
		printf("wdo_time: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_time);
		printf("dig_time: %d\r\n", (uint)tmp_ptr->data.data_struct.dig_time);
		printf("wdo_id: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_id);

		if(tmp_ptr->data.data_struct.wdo_id != window){
			printf("window id is wrong! window = %d | wdo_id = %d\r\n", window, (uint)tmp_ptr->data.data_struct.wdo_id);
			return XST_FAILURE;
		}
		else{
			index = 0;
			frame_buf[index++] = 0x55;
			frame_buf[index++] = 0xAA;
			frame_buf[index++] = (char)k;
			frame_buf[index++] = (char)(k >> 8);
			for(i=0; i<16; i++){
				for(j=0; j<32; j++){
					data_tmp = (uint16_t)(tmp_ptr->data.data_struct.data[i][j] + VPED_DIGITAL - pedestal[window][i][j]);
					if(data_tmp > 2047) data_tmp = 2047;
					frame_buf[index++] = (char)lookup_table[data_tmp];
					frame_buf[index++] = (char)(lookup_table[data_tmp] >> 8);
				}
			}
			frame_buf[index++] = 0x33;
			frame_buf[index++] = 0xCC;
			transfer_data(frame_buf, index);
		}
		ControlRegisterWrite(PSBUSY_MASK,DISABLE);
	}

	free(tmp_ptr);

	return XST_SUCCESS;
}

