/*
 * pedestal.c
 *
 *  Created on: 18 d�c. 2018
 *      Author: Anthony
 */

#include "pedestal.h"

/* Extern global variables */
extern int* regptr;
extern uint16_t pedestal[512][16][32];
extern volatile bool flag_axidma_rx_done;

int init_pedestals(void){
	uint64_t sqr_val[2][16][32];
	double rms[2][16][32];
	uint32_t data[2][16][32];
	int window, window_index;
	int timeout;
	int i,j,count,pair;

	data_list* tmp_ptr  = (data_list *)malloc(sizeof(data_list));
	if(!tmp_ptr){
		printf("malloc for tmp_ptr failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	tmp_ptr->next = NULL;
	tmp_ptr->previous = NULL;

	for(window_index=0; window_index<512; window_index+=2){
		for(pair=0; pair<2; pair++){
			for(i=0; i<16; i++){
				for(j=0; j<32; j++){
					data[pair][i][j] = 0;
					sqr_val[pair][i][j] = 0;
				}
			}
		}
		for(count=0; count<10; count++){
			window = window_index;
			for(pair=0; pair<2; pair++){
				window += pair;
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
					printf("\r\nwindow = %d | count = %d\r\n", window, count);
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
					for(i=0; i<16; i++){
						for(j=0; j<32; j++) {
							data[pair][i][j] += tmp_ptr->data.data_struct.data[i][j];
							sqr_val[pair][i][j] += tmp_ptr->data.data_struct.data[i][j]*tmp_ptr->data.data_struct.data[i][j];
						}
					}
				}
			}
		}

		window = window_index;
		for(pair=0; pair<2; pair++){
			window += pair;
			for(i=0; i<16; i++){
				for(j=0; j<32; j++){
					pedestal[window][i][j]= data[pair][i][j]/10;
					sqr_val[pair][i][j] = sqr_val[pair][i][j]/10;
					rms[pair][i][j] = sqrt(sqr_val[pair][i][j] - (pedestal[window][i][j]*pedestal[window][i][j]));
				}
			}
			if(window == 0){
				printf("RMS values\r\n");
				for(j=0; j<32; j++){
					for(i=0; i<16; i++) printf("%lf\t", rms[pair][i][j]);
					printf("\r\n");
				}
			}
		}
	}
	free(tmp_ptr);

	return XST_SUCCESS;
}
