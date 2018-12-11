/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xtime_l.h"
#include "data_analysis.h"
#include "global.h"

extern double period[512][16][32];
extern int16_t pedestal[512][16][32];
extern data_list* first_element;
extern data_list* last_element;

int main()
{
	XTime tStart, tEnd, tInt;
	int tmp1[] = {0,0,0,0,0,-1,-1,-1,-1,2,2,2,3,3,3,1,1,1,1,-2,-5,-6,-9,-20,-30,-36,-53,-62,-80,-102,-120,-164,-201,-356,-400,-434,-399,-354,-270,-202,-155,-118,-99,-78,-63,-45,-31,-13,-3,-1,0,0,0,2,3,1,4,2,1,0,1,1,0,0};
	int tmp2[] = {0,0,0,0,0,-1,-1,-1,-1,2,2,2,3,3,3,1,1,1,1,-2,-5,-6,-9,-20,-30,-36,-53,-62,-80,-102,-120,-164,-201,-356,-400,-434,-399,-354,-270,-202,-155,-118,-99,-78,-63,-45,-31,-13,-3,-1,0,0,0,2,3,1,4,2,1,0,1,1,0,0};
	int tmp3[] = {0,0,0,0,0,-1,-1,-1,-1,2,2,2,3,3,3,1,1,1,1,-2,-5,-6,-9,-20,-30,-36,-53,-62,-80,-102,-120,-164,-201,-356,-400,-434,-399,-354,-270,-202,-155,-118,-99,-78,-63,-45,-31,-13,-3,-1,0,0,0,2,3,1,4,2,1,0,1,1,0,0};
	int tmp4[] = {0,0,0,0,0,-1,-1,-1,-1,2,2,2,3,3,3,1,1,1,1,-2,-5,-6,-9,-20,-30,-36,-53,-62,-80,-102,-120,-164,-201,-356,-400,-434,-399,-354,-270,-202,-155,-118,-99,-78,-63,-45,-31,-13,-3,-1,0,0,0,2,3,1,4,2,1,0,1,1,0,0};
	uint16_t data[128];
	features_ext features;
	int i,j,k,length;
	data_list* ptr;
	uint32_t info = 0;

	printf("\r\n\r\n--------START-------\n\r");
	/* Initialize the global variables */
	if(init_global_var() == 0) printf("Global variables initialization pass!\r\n");
	else{
		printf("-------END-------\r\n");
		return -1;
	}

	last_element->data.data_struct.wdo_id = 0;
	last_element->data.data_struct.info = 0;

	for(i=0;i<32;i++){
		last_element->data.data_struct.data[0][i] = tmp1[i] + 1024;
		last_element->data.data_struct.data[1][i] = tmp2[i] + 1024;
		last_element->data.data_struct.data[2][i] = tmp3[i] + 1024;
		last_element->data.data_struct.data[3][i] = tmp4[i] + 1024;
	}
	ptr = malloc(sizeof(data_list));
	if(!ptr){
		printf("malloc(%d) for first_element failed in function, %s!\r\n", sizeof(data_list), __func__);
		return 1;
	}
	ptr->next = NULL;
	last_element->next = ptr;
	last_element = ptr;
	last_element->data.data_struct.wdo_id = 1;
	last_element->data.data_struct.info = 0;
	for(i=0;i<32;i++){
		last_element->data.data_struct.data[0][i] = tmp1[i+32] + 1024;
		last_element->data.data_struct.data[1][i] = tmp2[i+32] + 1024;
		last_element->data.data_struct.data[2][i] = tmp3[i+32] + 1024;
		last_element->data.data_struct.data[3][i] = tmp4[i+32] + 1024;
	}
	ptr = malloc(sizeof(data_list));
	if(!ptr){
		printf("malloc(%d) for first_element failed in function, %s!\r\n", sizeof(data_list), __func__);
		return 1;
	}
	ptr->next = NULL;
	last_element->next = ptr;
	last_element = ptr;
	last_element->data.data_struct.wdo_id = 2;
	last_element->data.data_struct.info = 0;
	for(i=0;i<32;i++){
		last_element->data.data_struct.data[0][i] = 1024;
		last_element->data.data_struct.data[1][i] = 1024;
		last_element->data.data_struct.data[2][i] = 1024;
		last_element->data.data_struct.data[3][i] = 1024;
	}
	ptr = malloc(sizeof(data_list));
	if(!ptr){
		printf("malloc(%d) for first_element failed in function, %s!\r\n", sizeof(data_list), __func__);
		return 1;
	}
	ptr->next = NULL;
	last_element->next = ptr;
	last_element = ptr;
	last_element->data.data_struct.wdo_id = 3;
	last_element->data.data_struct.info = 0;
	for(i=0;i<32;i++){
		last_element->data.data_struct.data[0][i] = 1024;
		last_element->data.data_struct.data[1][i] = 1024;
		last_element->data.data_struct.data[2][i] = 1024;
		last_element->data.data_struct.data[3][i] = 1024;
	}
	init_platform();

	for(i=0; i<512; i++){
		for(j=0; j<16; j++){
			for(k=0; k<32; k++) pedestal[i][j][k] = 1024;
		}
	}
    length = sizeof(data)/4;
    XTime_GetTime(&tStart);
	int ch = correct_data(data, 0, 4, &info);
	XTime_GetTime(&tEnd);
	printf("--------correct_data--------\r\n");
	printf("total     Output took %llu clock cycles.\n", 2*(tEnd - tStart));
	printf("total     Output took %.2f us.\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000));
	printf("ch = %d | info = 0x%08X\r\n", ch, info);

    XTime_GetTime(&tStart);
    extract_features(data, length, &features, &tInt);
    XTime_GetTime(&tEnd);
    printf("-------extract_fetures-------\r\n");
    printf("inter     Output took %llu clock cycles.\n", 2*(tInt - tStart));
	printf("inter     Output took %.2f us.\n",1.0 * (tInt - tStart) / (COUNTS_PER_SECOND/1000000));
	printf("total     Output took %llu clock cycles.\n", 2*(tEnd - tStart));
	printf("total     Output took %.2f us.\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000));
    printf("amplitude=%d | time=%f\n\r", features.amplitude, features.time);

    cleanup_platform();
    cleanup_global_var();
    printf("-------END----------\n\r");
    return 0;
}
