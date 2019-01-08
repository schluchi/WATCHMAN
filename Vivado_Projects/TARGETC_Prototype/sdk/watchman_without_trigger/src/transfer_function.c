/*
 * transfer_function.c
 *
 *  Created on: 7 janv. 2019
 *      Author: antho
 */

#include "transfer_function.h"

/* Extern global variables */
extern int* regptr;
extern volatile bool flag_axidma_rx_done;
extern uint16_t pedestal[512][16][32];
extern char* frame_buf;

int send_data_transfer_fct(void){
	int window = 0;
	int timeout;
	int k,i,j,index;

	data_list* tmp_ptr  = (data_list *)malloc(sizeof(data_list));
	if(!tmp_ptr){
		printf("malloc for tmp_ptr failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	tmp_ptr->next = NULL;
	tmp_ptr->previous = NULL;

	for(k=0; k< 11; k ++){
		if(DAC_LTC2657_SetChannelVoltage(DAC_VPED, k*0.25) != XST_SUCCESS){
			xil_printf("DAC: setting vped voltage failed!\r\n");
			return XST_FAILURE;
		}

		for(window=0; window<512; window++){
			XAxiDma_SimpleTransfer_Hej((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

			regptr[TC_FSTWINDOW_REG] = window;
			regptr[TC_NBRWINDOW_REG] = 1;
			ControlRegisterWrite(SMODE_MASK ,ENABLE);
			ControlRegisterWrite(SS_TPG_MASK ,ENABLE);
			ControlRegisterWrite(WINDOW_MASK,ENABLE);
			usleep(50);
			ControlRegisterWrite(WINDOW_MASK,DISABLE); // PL side starts on falling edge

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
				for(j=1; j<32; j++){
					for(i=0; i<16; i++){
						printf("%d\t", (uint)tmp_ptr->data.data_struct.data[i][j]);
					}
					printf("\r\n");
				}
				printf("Timeout on window %d: pedestal initialization failed!\r\n", window);
				return XST_FAILURE;
			}
			else flag_axidma_rx_done = false;

			if(tmp_ptr->data.data_struct.wdo_id != window){
				printf("window id is wrong! window = %d | wdo_id = %d\r\n", window, (uint)tmp_ptr->data.data_struct.wdo_id);
				return XST_FAILURE;
			}
			else{
				index = 0;
				frame_buf[index++] = 0x55;
				frame_buf[index++] = 0xAA;
				frame_buf[index++] = k;
				frame_buf[index++] = (char)window;
				frame_buf[index++] = (char)(window >> 8);
				for(i=0; i<16; i++){
					for(j=0; j<32; j++){
						tmp_ptr->data.data_struct.data[i][j] = tmp_ptr->data.data_struct.data[i][j] + VPED - pedestal[window][i][j];
						frame_buf[index++] = (char)tmp_ptr->data.data_struct.data[i][j];
						frame_buf[index++] = (char)(tmp_ptr->data.data_struct.data[i][j] >> 8);
					}
				}
				frame_buf[index++] = 0x33;
				frame_buf[index++] = 0xCC;
				transfer_data(frame_buf, index);
			}
		}
		printf("transfer function -> vped = %lf\r\n", k*0.25);
	}
	free(tmp_ptr);

	if(DAC_LTC2657_SetChannelVoltage(DAC_VPED, 2.00) != XST_SUCCESS){
		xil_printf("DAC: setting vped voltage failed!\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

