/*Copyright (c) 2015, EmbeddedCentric.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

//////////////////////////////////////////////////////////////////////////////////
// Company: EmbeddedCentric.com
// Author:  Ali Aljaani
// Create Date:    08/21/2015
// Description: Zynq SoC Training-lab7
//////////////////////////////////////////////////////////////////////////////////

// Include Files
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xscutimer.h"
#include "xil_printf.h"
//Lab7 addition start
#include "xsdps.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include "interrupt.h"
#include "time_hm.h"


extern bool mount_sd;
extern int count_timer;

char *Path = "0:/";  //  string pointer to the logical drive number
static FATFS FS_instance; // File System instance
FRESULT result;			// FRESULT variable

//----------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------
int main (void)
{
	time_cplt offset_time = {
		.year = 2000,
		.month = 1,
		.day = 1,
		.hour = 0,
		.minute = 0,
		.second = 0,
		.milisecond = 0
		};
	bool init_flag = true;
	xil_printf("\r\n\r\nLog test\r\n");
	xil_printf("-----START-----\r\n");
	//----------------------------------------------------
	// SETUP THE TIMER
	//----------------------------------------------------
	if(init_interrupts() == XST_SUCCESS) xil_printf("Interrupts initialization pass!\r\n");
	else{
		xil_printf("Interrupts initialization failed!\r\n");
		init_flag = false;
	}

	// Mount SD Card and initialize device
	result = f_mount(&FS_instance,Path, 1);
	if (result != 0) {
		xil_printf("SD card mount failed %d\r\n", result);
		init_flag = false;
	}
	else xil_printf("SD card mount passed!\r\n");
	mount_sd = true;

	if(init_flag){
		while(1){
			sleep(10);
			offset_time.year = 2045;
			offset_time.month = 9;
			offset_time.day = 27;
			offset_time.hour = 13;
			offset_time.minute = 38;
			offset_time.second = 2;
			settime_hm(&offset_time);
		}
	}

	xil_printf("end of while loop\r\n");
	cleanup_interrupts();
	// Never reached on normal execution
	xil_printf("------END------\r\n");
return (0);
}



