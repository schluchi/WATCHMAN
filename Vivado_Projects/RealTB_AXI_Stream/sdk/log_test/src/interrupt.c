/*
 * interrupt.c
 *
 *  Created on: 9 nov. 2018
 *      Author: Anthony
 */

#include "interrupt.h"
#include <time.h>

//Global variables
static XScuTimer TimerScuInstance;
static XScuGic Intc;
unsigned int seconds = 0;
unsigned int seconds_tmp = 0;
unsigned int minutes = 0;
unsigned int hours = 0;
unsigned int days = 0;
unsigned int months = 0;
unsigned int years = 0;
char buffer [9];
unsigned int options;
bool mount_sd = false;


FRESULT result;			// FRESULT variable
int count_timer=0; // Counter for the push button


void timer_scu_callback(XScuTimer * TimerInstance)
{
	static bool first_time = true;


	/*sprintf : Composes a string with the same text that would be printed if format was used on printf
	 it is used here to organise the time in HH:MM:SS format*/

    if(mount_sd){
		if(first_time){
			result = create_logfile();
			first_time = false;
		}
		result = write_logfile("testt", 5);
    }
    else xil_printf("SD card not mounted\r\n");
    // Increment counter
	count_timer++;

    // Clear timer's interrupt
    XScuTimer_ClearInterruptStatus(TimerInstance);


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

void cleanup_interrupts()
{
	XScuTimer_DisableInterrupt(&TimerScuInstance);
	XScuTimer_Stop(&TimerScuInstance);
	Xil_ICacheDisable();
	Xil_DCacheDisable();
	return;
}
