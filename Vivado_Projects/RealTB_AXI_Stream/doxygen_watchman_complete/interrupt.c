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

#include "interrupt.h"

#ifndef USE_SOFTETH_ON_ZYNQ
static int ResetRxCntr = 0;
#endif

static XScuTimer TimerScuInstance;
static XTtcPs TimerTtcPsInstance;
static XScuGic TestCompInstance;
static XScuWdt WdtScuInstance;
static XScuGic Intc;

static TmrCntrSetup SettingsTable = {10, 0, 0, 0};
char dummy_data[MAX_STREAM_SIZE];
uint16_t length_dummy_data;
static XTime tStart_dma, tEnd_dma;
static XTime tStart_wdt, tEnd_wdt;

/****************************************************************************/
/**
* @brief	Callback for the timer scu
*
* @param	TimerInstance: pointer on the timer's instance
*
* @return	None
*
* @note		This callback is called every 250ms
*
****************************************************************************/
void timer_scu_callback(XScuTimer * TimerInstance)
{
	count_scu_timer++;
	XScuWdt_RestartWdt(&WdtScuInstance);	// Reload the counter for the wdt

#ifndef USE_SOFTETH_ON_ZYNQ
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
	ResetRxCntr++;
	if (ResetRxCntr >= RESET_RX_CNTR_LIMIT) {
		xemacpsif_resetrx_on_no_rxdata(echo_netif);
		ResetRxCntr = 0;
	}
	//xemacpsif_resetrx_on_no_rxdata(echo_netif); // Now the function is called every 250ms
#endif

	// Need to call this function every 250ms
	xemacif_input(echo_netif);

	// Clear timer's interrupt
	XScuTimer_ClearInterruptStatus(TimerInstance);
}

/****************************************************************************/
/**
* @brief	Callback for the timer tcps
*
* @param	TimerInstance: pointer on the timer's instance
*
* @return	None
*
* @note		This callback is called every 1sec
*
****************************************************************************/
void timer_ttcps_callback(XTtcPs * TimerInstance)
{
	u32 StatusEvent;
	StatusEvent = XTtcPs_GetInterruptStatus(TimerInstance);
	XTtcPs_ClearInterruptStatus(TimerInstance, StatusEvent);
	if ((StatusEvent & XTTCPS_IXR_INTERVAL_MASK) != 0){
		flag_ttcps_timer = true;
	}
}

/****************************************************************************/
/**
* @brief	Callback when the dma finish a transfer
*
* @param	AxiDmaInst: pointer on the axidma's instance
*
* @return	None
*
* @note		-
*
****************************************************************************/
void axidma_rx_callback(XAxiDma* AxiDmaInst){
	uint32_t IrqStatus;

	/* Read pending interrupts */
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DEVICE_TO_DMA);

	/* Acknowledge pending interrupts */
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

	/* If no interrupt is asserted, we do not do anything */
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {
		axidma_error = 1;
		return;
	}

	/* If completion interrupt is asserted, then set RxDone flag */
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK)) {
		transfer_data(PtrData, NBR_DATA*4);
		XTime_GetTime(&tEnd_dma);
		axidma_rx_done = 1;
		xil_printf("TIMING DMA TRANFSERT:\r\n");
		printf("     Output took %llu clock cycles.\n", 2*(tEnd_dma - tStart_dma));
		printf("     Output took %.2f us.\n",1.0 * (tEnd_dma - tStart_dma) / (COUNTS_PER_SECOND/1000000));
		//printf("     Output took %.2f ms.\n",1.0 * (tEnd_dma - tStart_dma) / (COUNTS_PER_SECOND/1000));
		//printf("Bandwidth:%.2f Mbit/s\r\n",PacketsToSend*32/(1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000)) );
	}
}

/****************************************************************************/
/**
* @brief	Callback when the dma start a transfer
*
* @param	callbackInst: pointer on the testcomponent's instance
*
* @return	None
*
* @note		This callback is used onlay the mesure the duration of a tansfer
*
****************************************************************************/
void testcomponent_callback(void *callbackInst){
	XTime_GetTime(&tStart_dma);
}

/****************************************************************************/
/**
* @brief	Callback for the timer wdt
*
* @param	WdtInstance: pointer on the timer's instance
*
* @return	None
*
* @note		This callback is called only if the wdt is used has a timer
* 			(not the case here), used only the configure the wdt period of 1sec
*
****************************************************************************/
void wdt_scu_callback(XScuWdt *WdtInstance){
	XTime_GetTime(&tEnd_wdt);
	printf("Output took %llu clock cycles.\r\n", 2*(tEnd_wdt - tStart_wdt));
	printf("Output took %.2f us.\r\n",1.0 * (tEnd_wdt - tStart_wdt) / (COUNTS_PER_SECOND/1000000));
	XTime_GetTime(&tStart_wdt);

	// Clear Watchdog's interrupt
	XScuWdt_WriteReg(WdtInstance->Config.BaseAddr,
						 XSCUWDT_ISR_OFFSET,
						 XSCUWDT_ISR_EVENT_FLAG_MASK);
	XScuWdt_RestartWdt(WdtInstance);	// Reload the counter for the wdt
}

/****************************************************************************/
/**
* @brief	Setup the timer scu interrupt
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
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
	TimerLoadValue = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 20;

	XScuTimer_LoadTimer(&TimerScuInstance, TimerLoadValue);
	return Status;
}

/****************************************************************************/
/**
* @brief	Setup the timer tcps interrupt
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
int setup_ttcps_timer_int(void){
	int Status = XST_SUCCESS;
	XTtcPs_Config *ConfigPtr;
	XTtcPs *Timer;
	TmrCntrSetup *TimerSetup;


	ConfigPtr = XTtcPs_LookupConfig(TTC_TICK_DEVICE_ID);
	if (ConfigPtr == NULL){
		xil_printf("In %s: TtcPs timer Look up config failed...\r\n",
		__func__);
		return XST_FAILURE;
	}

	Status = XTtcPs_CfgInitialize(&TimerTtcPsInstance, ConfigPtr,
			ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: TtcPs timer Cfg initialization failed...\r\n",
		__func__);
		return Status;
	}

	Status = XTtcPs_SelfTest(&TimerTtcPsInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: TtcPs timer Self test failed...\r\n",
		__func__);
		return Status;

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

	return Status;
}

/****************************************************************************/
/**
* @brief	Setup the axidma interrupt
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
int setup_axidma_int(void)
{
	int Status = XST_SUCCESS;
	XAxiDma_Config *ConfigPtr;

	ConfigPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
	if (ConfigPtr == NULL){
		xil_printf("In %s: AxiDMA Look up config failed...\r\n",
		__func__);
		return XST_FAILURE;
	}
	Status = XAxiDma_CfgInitialize(&AxiDmaInstance, ConfigPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: AxiDMA Cfg initialization failed...\r\n",
		__func__);
		return Status;
	}

	Status = XAxiDma_Selftest(&AxiDmaInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: AxiDMA Self test failed...\r\n",
		__func__);
		return Status;
	}

	Status = XAxiDma_HasSg(&AxiDmaInstance);
	if (Status != XST_SUCCESS){
		xil_printf("In %s: AxiDMA configured as SG mode...\r\n",
		__func__);
		return Status;
	}

	XAxiDma_Reset(&AxiDmaInstance);
	uint32_t reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_CR_OFFSET);
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_CR_OFFSET, reg | XAXIDMA_CR_RESET_MASK);
	int TimeOutCnt = 5;
	while(TimeOutCnt){
		reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_CR_OFFSET);
		if((reg &  XAXIDMA_CR_RESET_MASK) == NULL) {
			break;
		}
		TimeOutCnt-=1;
		sleep(1);
	}

	if((reg &  XAXIDMA_CR_RESET_MASK) != NULL) {
		xil_printf("In %s: AxiDMA Reset failed...\r\n",
		__func__);
		return XST_FAILURE;
	}

	/* Disable MM2S Sied of AXI DMA */
	reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_TX_OFFSET, XAXIDMA_CR_OFFSET);
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_TX_OFFSET,XAXIDMA_CR_OFFSET,reg & !XAXIDMA_CR_RUNSTOP_MASK);

	axidma_error = 0;
	axidma_rx_done = 0;
	return Status;
}

/****************************************************************************/
/**
* @brief	Setup the testcomponent interrupt
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
int setup_test_component_int(void){
	int *regbank;
	int Status = XST_SUCCESS;

	regbank = XPAR_AXIS_TEST_COMPONENT_0_S00_AXI_BASEADDR;
	regbank[CONTROL_REG] = 0;			// Stop
	regbank[NBR_OF_PACKETS_REG] = 0;	// Nbr of packets = 0
	regbank[CONTENT_PACKET_1] = 0;		// Content of 1st packet = 0

	return Status;
}

/****************************************************************************/
/**
* @brief	Setup the wdt interrupt
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
int setup_scu_wdt_int(void){
	int Status = XST_SUCCESS;
	XScuWdt_Config *ConfigPtr;

	ConfigPtr = XScuWdt_LookupConfig(WDT_DEVICE_ID);
	if (ConfigPtr == NULL){
		xil_printf("In %s: Scu Wdt Look up config failed...\r\n",
		__func__);
		return XST_FAILURE;
	}
	Status = XScuWdt_CfgInitialize(&WdtScuInstance, ConfigPtr,
			ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scu Wdt Cfg initialization failed...\r\n",
		__func__);
		return Status;
	}

	Status = XScuWdt_SelfTest(&WdtScuInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: Scu Wdt Self test failed...\r\n",
		__func__);
		return Status;

	}

	/* Choose the mode for the WDT interrupt */
	XScuWdt_SetWdMode(&WdtScuInstance); // Wdt mode: no callback, reset with a flag on reboot

	if(XScuWdt_IsWdtExpired(&WdtScuInstance)){
		xil_printf("%s: Watch dog has expired\r\n", __func__);
		write_wdtfile();//if(write_wdtfile() != FR_OK)
	}
	else{
		xil_printf("%s: Watch dog has not expired\r\n", __func__);
		create_wdtfile();
	}
	create_timefile();
	flag_timefile = true;

	return Status;
}

/****************************************************************************/
/**
* @brief	Attach all the interrupt to the system and set the priority
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
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
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, TTC_TICK_INTR_ID,
					(Xil_ExceptionHandler)timer_ttcps_callback,
					(void *)&TimerTtcPsInstance);
	/*
	 * Enable the interrupt for ttcps timer.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TTC_TICK_INTR_ID);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,
					(Xil_ExceptionHandler)axidma_rx_callback,
					(void *)&AxiDmaInstance);
	/*
	 * Enable the interrupt for axidma.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, XPAR_FABRIC_AXIS_TEST_COMPONENT_0_S00_AXI_INTR_INTR,
					(Xil_ExceptionHandler)testcomponent_callback,
					(void *)&TestCompInstance);
	/*
	 * Enable the interrupt for test component.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, XPAR_FABRIC_AXIS_TEST_COMPONENT_0_S00_AXI_INTR_INTR);

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, WDT_IRPT_INTR,
					(Xil_ExceptionHandler)wdt_scu_callback,
					(void *)&WdtScuInstance);
	/*
	 * Enable the interrupt for WDT timer.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, WDT_IRPT_INTR);

	// set the priority to 0xA0 (highest 0xF8, lowest 0x00) and a trigger for a rising edge 0x3.
	XScuGic_SetPriorityTriggerType(&Intc, TIMER_IRPT_INTR, 0xA1, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, TTC_TICK_INTR_ID, 0xA2, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR, 0xA3, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, XPAR_FABRIC_AXIS_TEST_COMPONENT_0_S00_AXI_INTR_INTR, 0xA0, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, WDT_IRPT_INTR, 0xA4, 0x3);

	return Status;
}

/****************************************************************************/
/**
* @brief	Enable the interrupts and start the timers
*
* @param	None
*
* @return	None
*
* @note		-
*
****************************************************************************/
void enable_interrupts()
{
	uint32_t Reg;

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	/* Enable every interrupts */
	XScuTimer_EnableInterrupt(&TimerScuInstance);
	XScuTimer_Start(&TimerScuInstance);

	XTtcPs_EnableInterrupts(&TimerTtcPsInstance, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Start(&TimerTtcPsInstance);

	XAxiDma_IntrEnable(&AxiDmaInstance, XAXIDMA_IRQ_IOC_MASK, XAXIDMA_DEVICE_TO_DMA);

	XScuGic_Enable(&Intc, XPAR_FABRIC_AXIS_TEST_COMPONENT_0_S00_AXI_INTR_INTR);

	Reg = XScuWdt_GetControlReg(&WdtScuInstance);
	XScuWdt_SetControlReg(&WdtScuInstance, Reg | XSCUWDT_CONTROL_IT_ENABLE_MASK);
	/*
	 * In Watchdog mode, we must start to see the event after the reboot
	 * Useless in Time mode
	 */
	XScuWdt_Start(&WdtScuInstance);
	/*
	 * Load the watchdog counter register.
	 * When the counter is loaded, the count-down start
	 */
	XScuWdt_LoadWdt(&WdtScuInstance, WDT_LOAD_VALUE);
	XTime_GetTime(&tStart_wdt);

	return;
}

/****************************************************************************/
/**
* @brief	Initiate and setup all the interrupts
*
* @param	None
*
* @return	XST_SUCCESS or XST_FAILURE (defined in xstatus.h)
*
* @note		-
*
****************************************************************************/
int init_interrupts()
{
	int Status;
	Status = setup_scu_timer_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Scu timer failed...\r\n",
		__func__);
		return Status;
	}
	Status = setup_ttcps_timer_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Ttcps timer failed...\r\n",
		__func__);
		return Status;
	}
	Status = setup_axidma_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: AxiDMA failed...\r\n",
		__func__);
		return Status;
	}
	Status = setup_test_component_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Test Component failed...\r\n",
		__func__);
		return Status;
	}
	Status = setup_scu_wdt_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: WDT timer failed...\r\n",
		__func__);
		return Status;
	}
	Status = setup_interrupts();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Setup interrupts failed...\r\n",
		__func__);
		return Status;
	}

	length_dummy_data = made_frame(dummy_data, MAX_STREAM_SIZE);

	return XST_SUCCESS;
}

/****************************************************************************/
/**
* @brief	Disable all the interrupts and stop the timers
*
* @param	None
*
* @return	None
*
* @note		-
*
****************************************************************************/
void cleanup_interrupts()
{
	uint32_t Reg;
	XScuTimer_DisableInterrupt(&TimerScuInstance);
	XScuTimer_Stop(&TimerScuInstance);
	XTtcPs_DisableInterrupts(&TimerTtcPsInstance, XTTCPS_IXR_INTERVAL_MASK);
	XTtcPs_Stop(&TimerTtcPsInstance);
	XAxiDma_IntrDisable(&AxiDmaInstance, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	XScuGic_Disable(&Intc, XPAR_FABRIC_AXIS_TEST_COMPONENT_0_S00_AXI_INTR_INTR);
	Reg = XScuWdt_GetControlReg(&WdtScuInstance);
	XScuWdt_SetControlReg(&WdtScuInstance, Reg & ~(XSCUWDT_CONTROL_IT_ENABLE_MASK));
	XScuWdt_Stop(&WdtScuInstance);
	XScuWdt_SetTimerMode(&WdtScuInstance); // Timer mode: to stop the watchdog
	Xil_ICacheDisable();
	Xil_DCacheDisable();
	return;
}

