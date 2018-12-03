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
#include "features_extraction.h"
#include "xtime_l.h"


int main()
{
	XTime tStart, tEnd, tInt;
	int tmp[] = {0,0,0,0,0,-1,-1,-1,-1,2,2,2,3,3,3,1,1,1,1,-2,-5,-6,-9,-20,-30,-36,-53,-62,-80,-102,-120,-164,-201,-356,-400,-434,-399,-354,-270,-202,-155,-118,-99,-78,-63,-45,-31,-13,-3,-1,0,0,0,2,3,1,4,2,1,0,1,1,0,0};
	int data[128];
	features_ext features;
	int i,length;
	for(i=0;i<64;i++) data[i]=tmp[i];
	for(i=64;i<128;i++) data[i]=0;
	init_platform();

    printf("--------START-------\n\r");
    length = sizeof(data)/4;
    for(i=0; i<length; i++) data[i] += 1024;
    //for(i=0; i<length; i++) printf("%d) %d\n\r",i,data[i]);
    XTime_GetTime(&tStart);
    extract_features(1024, data, length, &features, &tInt);
    XTime_GetTime(&tEnd);
    printf("inter     Output took %llu clock cycles.\n", 2*(tInt - tStart));
	printf("inter     Output took %.2f us.\n",1.0 * (tInt - tStart) / (COUNTS_PER_SECOND/1000000));
	printf("     Output took %llu clock cycles.\n", 2*(tEnd - tStart));
		printf("     Output took %.2f us.\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000));
    printf("amplitude=%d | time=%f\n\r", features.amplitude, features.time);

    cleanup_platform();
    printf("-------END----------\n\r");
    return 0;
}
