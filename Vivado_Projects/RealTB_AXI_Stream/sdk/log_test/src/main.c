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
#include "ff.h"
#include "xil_cache.h"
//Lab7 addition end

// Parameter Definitions
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_SCUTIMER_DEVICE_ID
#define INTC_BASE_ADDR		XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR
/* IMPORTANT NOTE: The AXI timer frequency is set to 50Mhz
 * the timer is set up to be counting UP, these two facts affect the value selected for
 * the load register to generate 1 Hz interrupts
 */

static XScuTimer TimerScuInstance;
static XScuGic Intc;

//Global variables
unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int hours = 0;
char buffer [9];
unsigned int options;
bool mount_sd = false;

//Lab7 addition start
static FATFS FS_instance; // File System instance
static FIL file1;		// File instance
FRESULT result;			// FRESULT variable
static char FileName[32] = "RECORDS.txt"; // name of the log
static char *Log_File; // pointer to the log
char *Path = "0:/";  //  string pointer to the logical drive number
int counter_logger=0; // Counter for the push button
unsigned int BytesWr; // Bytes written
int len=0;			// length of the string
int accum=0;		//  variable holding the EOF
u8 Buffer_logger[64] __attribute__ ((aligned(32))); // Buffer should be word aligned (multiple of 4)
u32 Buffer_size = 64;
//Lab7 addition end
//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
void timer_scu_callback(XScuTimer * TimerInstance);
int setup_scu_timer_int(void);
int setup_interrupts(void);
int init_interrupts();
void cleanup_interrupts();


void cleanup_interrupts()
{
	XScuTimer_DisableInterrupt(&TimerScuInstance);
	XScuTimer_Stop(&TimerScuInstance);
	Xil_ICacheDisable();
	Xil_DCacheDisable();
	return;
}

int init_interrupts()
{
	int Status;
	Status = setup_scu_timer_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Scu timer failed...\r\n",
		__func__);
		return Status;
	}

	Status = setup_interrupts();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Setup interrupts failed...\r\n",
		__func__);
		return Status;
	}


	return XST_SUCCESS;
}

int setup_interrupts(void)
{
	static XScuGic_Config *IntcConfig;
	int Status = XST_SUCCESS;

	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
	if (IntcConfig == NULL){
		xil_printf("In %s: XScuGic Look up config failed...\r\n",
		__func__);
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&Intc, IntcConfig,IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: XScuGic Cfg initialization failed...\r\n",
		__func__);
		return Status;
	}

	Xil_ExceptionInit();  // don't do anythings, prevent problem with other arm

	XScuGic_DeviceInitialize(INTC_DEVICE_ID);

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler)XScuGic_DeviceInterruptHandler,
			(void *)INTC_DEVICE_ID);
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, TIMER_IRPT_INTR,
					(Xil_ExceptionHandler)timer_scu_callback,
					(void *)&TimerScuInstance);
	/*
	 * Enable the interrupt for scu timer.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TIMER_IRPT_INTR);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	XScuTimer_EnableInterrupt(&TimerScuInstance);
	XScuTimer_Start(&TimerScuInstance);

	return Status;
}

int setup_scu_timer_int(void)
{
	int Status = XST_SUCCESS;
	XScuTimer_Config *ConfigPtr;
	int TimerLoadValue = 0;

	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	if (ConfigPtr == NULL){
		xil_printf("In %s: Scutimer timer Look up config failed...\r\n",
		__func__);
		return XST_FAILURE;
	}
	Status = XScuTimer_CfgInitialize(&TimerScuInstance, ConfigPtr,
			ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scutimer Cfg initialization failed...\r\n",
		__func__);
		return Status;
	}

	Status = XScuTimer_SelfTest(&TimerScuInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scutimer Self test failed...\r\n",
		__func__);
		return Status;

	}

	XScuTimer_EnableAutoReload(&TimerScuInstance);
	/*
	 * Set for 250 ms timeout. (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ/2 -> 1s)
	 *
	 * WARNING: If period is modified, must adapt counter in function timer_scu_callback()
	 */
	TimerLoadValue = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2;

	XScuTimer_LoadTimer(&TimerScuInstance, TimerLoadValue);
	return Status;
}

void timer_scu_callback(XScuTimer * TimerInstance)
{
	static bool first_time = true;
	// Increment the global seconds variable
	seconds++;

	// Increment the global minutes variable
	if (seconds>=60)
		{
		minutes++;
		seconds = 0;
		}
	// Increment the global hours variable
	if (minutes>=60)

		{
		hours++;
		minutes = 0;
		}
	// Restricting boundaries
    if (hours >= 24) {
      hours = 0;
    }

	/*sprintf : Composes a string with the same text that would be printed if format was used on printf
	 it is used here to organise the time in HH:MM:SS format*/
    sprintf (buffer, "%d:%d:%d Event: Timer Triggered %d",hours,minutes,seconds);

    if(mount_sd){
		sprintf(Buffer_logger, "%d:%d:%d Event: Timer Triggered %d\r\n",hours,minutes,seconds,counter_logger);

		// Open log for writing
		Log_File = (char *)FileName;
		if(first_time){
			result = f_open(&file1, Log_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
			first_time = false;
		}
		else result = f_open(&file1, Log_File,FA_WRITE);
		if (result!=0) {
			xil_printf("%d) f open failed = %d\r\n", counter_logger, result);
		}
		else{
			// Point to the end of log
			result = f_lseek(&file1,accum);
			if (result!=0) {
				xil_printf("%d) f_lseek failed = %d\r\n", counter_logger, result);
			}
			else{
				// Write to log
				result = f_write(&file1, (const void*)Buffer_logger, Buffer_size,&BytesWr);
				if (result!=0) {
					xil_printf("%d) f_write failed = %d\r\n", counter_logger, result);
				}
				else{
					// Increment file EOF pointer
					len = strlen(Buffer_logger);
					accum=accum+len;

					 //Close file.
					result = f_close(&file1);
					if (result!=0) {
						xil_printf("%d) f_close failed = %d\r\n", counter_logger, result);
					}
					else xil_printf("%d) write in file passed\r\n", counter_logger);
				}
			}
		}

		// Increment counter
		counter_logger++;
    }

    // Clear timer's interrupt
    XScuTimer_ClearInterruptStatus(TimerInstance);


}

//----------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------
int main (void)
{
	xil_printf("Lab7-Zynq Soc Training- SD Card Logging\r\n");
	xil_printf("EmbeddedCentric.com\r\n");
	bool init_flag = true;
	//----------------------------------------------------
	// SETUP THE TIMER
	//----------------------------------------------------
	if(init_interrupts() == XST_SUCCESS) xil_printf("Interrupts initialization pass!\r\n");
	else{
		xil_printf("Interrupts initialization failed!\r\n");
		init_flag = false;
	}

	// Lab7 addition start
	// Mount SD Card and initialize device
	result = f_mount(&FS_instance,Path, 1);
	if (result != 0) {
		xil_printf("SD card mount failed %d\r\n", result);
		init_flag = false;
	}
	else xil_printf("SD card mount passed!\r\n");
	mount_sd = true;
//	// Creating new file with read/write permissions
//	Log_File = (char *)FileName;
//	result = f_open(&file1, Log_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
//	if (result!= 0) {
//		xil_printf("f open failed!\r\n");
//		return XST_FAILURE;
//	}
//	else xil_printf("f open passed!\r\n");
	// Lab7 addition end
	while((counter_logger < 10) && init_flag){
	// Infinite loop - Do nothing
		sleep(1);
	}

	xil_printf("end of while loop\r\n");
	cleanup_interrupts();
	// Never reached on normal execution
	xil_printf("------END------\r\n");
return (0);
}



