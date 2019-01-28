/*
 * transfer_function.c
 *
 *  Created on: 16 janv. 2019
 *      Author: antho
 */

#include "transfer_function.h"

/* Extern global variables */
extern int* regptr;
extern volatile bool flag_axidma_rx_done;
extern uint16_t pedestal[512][16][32];
extern uint16_t lookup_table[2048];

int init_transfer_function(void){
	int timeout;
	int window, channel, sample, voltage;
	double data_tmp[11];
	GMtype_Data DataX, DataY;
	GMtype_Polynomial Polynomial;
	int range_min = 3;
	int range_max = 10;
	double y_voltage[2048];

	data_list* tmp_ptr  = (data_list *)malloc(sizeof(data_list));
	if(!tmp_ptr){
		printf("malloc for tmp_ptr failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	tmp_ptr->next = NULL;
	tmp_ptr->previous = NULL;

	for(voltage=0; voltage< 11; voltage ++){
		data_tmp[voltage] = 0;
		if(DAC_LTC2657_SetChannelVoltage(DAC_VPED, voltage*0.25) != XST_SUCCESS){
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
				for(sample=0; sample<32; sample++){
					for(channel=0; channel<16; channel++){
						printf("%d\t", (uint)tmp_ptr->data.data_struct.data[channel][sample]);
					}
					printf("\r\n");
				}
				printf("Timeout on window %d: pedestal initialization failed!\r\n", window);
				return XST_FAILURE;
			}
			else flag_axidma_rx_done = false;

			if(tmp_ptr->data.data_struct.wdo_id != window){
				printf("window id is wrong! voltage = %d | window = %d | wdo_id = %d\r\n", voltage, window, (uint)tmp_ptr->data.data_struct.wdo_id);
				printf("wdo_time: %d\r\n", (uint)tmp_ptr->data.data_struct.wdo_time);
				printf("dig_time: %d\r\n", (uint)tmp_ptr->data.data_struct.dig_time);
				printf("info: 0x%X\r\n", (uint)tmp_ptr->data.data_struct.info);
				return XST_FAILURE;
			}
			else{
				for(channel=0; channel<16; channel++){
					for(sample=0; sample<32; sample++){
						data_tmp[voltage] += (double)(tmp_ptr->data.data_struct.data[channel][sample] + VPED_DIGITAL - pedestal[window][channel][sample]);
					}
				}

			}
			ControlRegisterWrite(PSBUSY_MASK,DISABLE);
		}
		data_tmp[voltage] = data_tmp[voltage]/(512*16*32);
	}
	free(tmp_ptr);

	printf("data_tmp:\r\n[");
	for(int i=0; i<10; i++) printf("%.17g, ", data_tmp[i]);
	printf("%.17g]\r\n", data_tmp[10]);
	for(int i=range_min; i<range_max; i++) DataX.element[i-range_min] = data_tmp[i];
	DataX.size = range_max-range_min;
	DataY.size = range_max-range_min;
	for(int i=range_min; i<range_max; i++) DataY.element[i-range_min] = i*0.25;
	printf("DataX:\r\n[");
	for(int i=0; i<(DataX.size-1); i++) printf("%.17g, ", DataX.element[i]);
	printf("%.17g]\r\n", DataX.element[DataX.size-1]);
	printf("DataY:\r\n[");
	for(int i=0; i<(DataY.size-1); i++) printf("%.17g, ", DataY.element[i]);
	printf("%.17g]\r\n", DataY.element[DataY.size-1]);
	Polynomial.degree = 3;
	for(int i=0; i<30; i++) Polynomial.coef[i] = 0;
	GM_PolyFit(DataX, DataY, &Polynomial);
	printf("coefficient:\r\n[");
	for(int i=0; i<(Polynomial.degree); i++) printf("%.17g, ",Polynomial.coef[i]);
	printf("%.17g]\r\n",Polynomial.coef[3]);
	printf("lookup table:\r\n[");
	for(int i=0; i<2048; i++){
		y_voltage[i] = GM_SolvePolynomial(Polynomial, (double)i);
		lookup_table[i] = (uint16_t)(y_voltage[i]*2048/2.5);
	}
	for(int i=0; i<2047; i++) printf("%d, ", lookup_table[i]);
	printf("%d]\r\n", lookup_table[2047]);

	if(DAC_LTC2657_SetChannelVoltage(DAC_VPED, VPED_ANALOG) != XST_SUCCESS){
		xil_printf("DAC: setting vped voltage failed!\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
