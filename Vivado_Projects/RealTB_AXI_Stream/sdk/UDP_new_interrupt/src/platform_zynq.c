/******************************************************************************
*
* Copyright (C) 2010 - 2014 Xilinx, Inc.  All rights reserved.
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
* platform_zynq.c
*
* Zynq platform specific functions.
*
* 02/29/2012: UART initialization is removed. Timer initializations are
* removed. All unnecessary include files and hash defines are removed.
* 03/01/2013: Timer initialization is added back. Support for SI #692601 is
* added in the timer callback. The SI #692601 refers to the following issue.
*
* The EmacPs has a HW bug on the Rx path for heavy Rx traffic.
* Under heavy Rx traffic because of the HW bug there are times when the Rx path
* becomes unresponsive. The workaround for it is to check for the Rx path for
* traffic (by reading the stats registers regularly). If the stats register
* does not increment for sometime (proving no Rx traffic), the function resets
* the Rx data path.
*
* </pre>
 */

#include "platform_zynq.h"

static XScuTimer TimerScuInstance;
static XTtcPs TimerTtcPsInstance;
static XScuGic Intc;
static TmrCntrSetup SettingsTable = {10, 0, 0, 0};
volatile int count_ttcps_timer = 0;
volatile int count_scu_timer = 0;
volatile int nbre_of_bytes = 0;

#ifndef USE_SOFTETH_ON_ZYNQ
static int ResetRxCntr = 0;
extern struct netif *echo_netif;
#endif

volatile int TcpFastTmrFlag = 0;
volatile int TcpSlowTmrFlag = 0;

char dummy_data[MAX_STREAM_SIZE];
uint16_t length_dummy_data;

#if LWIP_DHCP==1
volatile int dhcp_timoutcntr = 24;
#endif

extern bool stream_flag;

void timer_scu_callback(XScuTimer * TimerInstance)
{
	/* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified
	 * by lwIP. It is not important that the timing is absoluetly accurate.
	 */
	static int odd = 1;

	TcpFastTmrFlag = 1;

	if(dhcp_timoutcntr > 0){
		dhcp_timoutcntr--;
		if(dhcp_timoutcntr%4 == 0){
			if(dhcp_timoutcntr == 0) xil_printf("%d...\r\n", dhcp_timoutcntr);
			else xil_printf("%d...", dhcp_timoutcntr/4);
		}
	}
	count_scu_timer++;

	/*
	 * Function to call to do perdiocally (every 250ms or 500ms, normaly)
	 */
	odd = !odd;

#if LWIP_DHCP==1
	static int dhcp_timer = 0;
    if (odd){
    	dhcp_timer++;
		dhcp_fine_tmr(); // Must call this function every 500ms
		if (dhcp_timer >= 120) {
			dhcp_coarse_tmr(); // Must call this function every 60sec
			dhcp_timer = 0;
		}
	}
#endif

    /* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified
	 * by lwIP. It is not important that the timing is absoluetly accurate.
	 */
	tcp_fasttmr(); // Must call this function every 250ms
	if (odd){
		tcp_slowtmr(); // Must call this function every 500ms
		TcpSlowTmrFlag = 1;
	}

    /* For providing an SW alternative for the SI #692601. Under heavy
	 * Rx traffic if at some point the Rx path becomes unresponsive, the
	 * following API call will ensures a SW reset of the Rx path. The
	 * API xemacpsif_resetrx_on_no_rxdata is called every 100 milliseconds.
	 * This ensures that if the above HW bug is hit, in the worst case,
	 * the Rx path cannot become unresponsive for more than 100
	 * milliseconds.
	 *
	 * PROBLEM : this function should be called every 100ms, but in fact with a counter
	 * of 400, it is called every 100s (original soft)
	 */
#ifndef USE_SOFTETH_ON_ZYNQ
    ResetRxCntr++;
	if (ResetRxCntr >= RESET_RX_CNTR_LIMIT) {
		xemacpsif_resetrx_on_no_rxdata(echo_netif);
		ResetRxCntr = 0;
	}
#endif
	// Need to call this function every 250ms
	xemacif_input(echo_netif);

	// Clear timer's interrupt
	XScuTimer_ClearInterruptStatus(TimerInstance);
}

void timer_ttcps_callback(XTtcPs * TimerInstance)
{
	u32 StatusEvent;
	uint16_t length;
	static char test = 1;
	StatusEvent = XTtcPs_GetInterruptStatus(TimerInstance);
	XTtcPs_ClearInterruptStatus(TimerInstance, StatusEvent);
	if (((StatusEvent & XTTCPS_IXR_INTERVAL_MASK) != 0) && (stream_flag)){
		count_ttcps_timer++;
		length = sizeof(dummy_data);
		length_dummy_data = made_frame(dummy_data, length);
		nbre_of_bytes += length_dummy_data;
		transfer_data(dummy_data, length_dummy_data);
		test = 0;
	}
	else{
		if(test == 0){
			xil_printf("number of frame sent = %d | total of bytes = %d\r\n", count_ttcps_timer, nbre_of_bytes);
			test = 1;
		}
	}
}

void platform_setup_scu_timer(void)
{
	int Status = XST_SUCCESS;
	XScuTimer_Config *ConfigPtr;
	int TimerLoadValue = 0;

	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	if (ConfigPtr == NULL){
		xil_printf("In %s: Scutimer timer Look up config failed...\r\n",
		__func__);
		return;
	}
	Status = XScuTimer_CfgInitialize(&TimerScuInstance, ConfigPtr,
			ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scutimer Cfg initialization failed...\r\n",
		__func__);
		return;
	}

	Status = XScuTimer_SelfTest(&TimerScuInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scutimer Self test failed...\r\n",
		__func__);
		return;

	}

	XScuTimer_EnableAutoReload(&TimerScuInstance);
	/*
	 * Set for 100 ms timeout. (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ/2 -> 1s)
	 */
	TimerLoadValue = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 16;//8;

	XScuTimer_LoadTimer(&TimerScuInstance, TimerLoadValue);
	return;
}

void platform_setup_ttcps_timer(void){
	int Status = XST_SUCCESS;
	XTtcPs_Config *ConfigPtr;
	XTtcPs *Timer;
	TmrCntrSetup *TimerSetup;


	ConfigPtr = XTtcPs_LookupConfig(TTC_TICK_DEVICE_ID);
	if (ConfigPtr == NULL){
		xil_printf("In %s: TtcPs timer Look up config failed...\r\n",
		__func__);
		return;
	}

	Status = XTtcPs_CfgInitialize(&TimerTtcPsInstance, ConfigPtr,
			ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: TtcPs timer Cfg initialization failed...\r\n",
		__func__);
		return;
	}

	Status = XTtcPs_SelfTest(&TimerTtcPsInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: TtcPs timer Self test failed...\r\n",
		__func__);
		return;

	}

	Timer = &TimerTtcPsInstance;
	TimerSetup = &SettingsTable;
	/*
	 * Set up appropriate options for Ticker: interval mode without
	 * waveform output.
	 */
	TimerSetup->Options |= (XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_WAVE_DISABLE);
	TimerSetup->OutputHz = TTCPS_TIMER_FREQ_HZ;
	XTtcPs_SetOptions(Timer, TimerSetup->Options);

	/*
	 * Timer frequency is preset in the TimerSetup structure,
	 * however, the value is not reflected in its other fields, such as
	 * IntervalValue and PrescalerValue. The following call will map the
	 * frequency to the interval and prescaler values.
	 */
	XTtcPs_CalcIntervalFromFreq(Timer, TimerSetup->OutputHz,
		&(TimerSetup->Interval), &(TimerSetup->Prescaler));

	/*
	 * Set the interval and prescale
	 */
	XTtcPs_SetInterval(Timer, TimerSetup->Interval);
	XTtcPs_SetPrescaler(Timer, TimerSetup->Prescaler);
}

void platform_setup_interrupts(void)
{
	static XScuGic_Config *IntcConfig;
	int Status = XST_SUCCESS;

	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
	if (IntcConfig == NULL){
		xil_printf("In %s: XScuGic Look up config failed...\r\n",
		__func__);
		return;
	}

	Status = XScuGic_CfgInitialize(&Intc, IntcConfig,IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: XScuGic Cfg initialization failed...\r\n",
		__func__);
		return;
	}

	// set the priority to 0xA0 (highest 0xF8, lowest 0x00) and a trigger for a rising edge 0x3.
	XScuGic_SetPriorityTriggerType(&Intc, TIMER_IRPT_INTR, 0xA1, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, TTC_TICK_INTR_ID, 0xA2, 0x3);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			(void *)&Intc);

	Status = XScuGic_Connect(&Intc,	TIMER_IRPT_INTR,
							(Xil_InterruptHandler)timer_scu_callback,
							&TimerScuInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: TIMER_IRPT_INTR failed...\r\n",
				__func__);
		return ;
	}

	Status = XScuGic_Connect(&Intc,	TTC_TICK_INTR_ID,
							(Xil_InterruptHandler)timer_ttcps_callback,
							&TimerTtcPsInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: TTC_TICK_INTR_ID failed...\r\n",
				__func__);
		return ;
	}
	return;
}

void platform_enable_interrupts()
{
	/*
	 * Enable non-critical exceptions.
	 */
	XScuGic_Enable(&Intc, TIMER_IRPT_INTR);
	XScuGic_Enable(&Intc, TTC_TICK_INTR_ID);

	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	XScuTimer_EnableInterrupt(&TimerScuInstance);
	XScuTimer_Start(&TimerScuInstance);
	XTtcPs_EnableInterrupts(&TimerTtcPsInstance, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Start(&TimerTtcPsInstance);
	return;
}

void init_platform()
{
	platform_setup_scu_timer();
	platform_setup_ttcps_timer();
	platform_setup_interrupts();
	uint16_t length = sizeof(dummy_data);
	length_dummy_data = made_frame(dummy_data, length);

	return;
}

void cleanup_platform()
{
	XTtcPs_DisableInterrupts(&TimerTtcPsInstance, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Stop(&TimerTtcPsInstance);
	XScuTimer_DisableInterrupt(&TimerScuInstance);
	XScuTimer_Stop(&TimerScuInstance);
	Xil_ICacheDisable();
	Xil_DCacheDisable();
	return;
}

