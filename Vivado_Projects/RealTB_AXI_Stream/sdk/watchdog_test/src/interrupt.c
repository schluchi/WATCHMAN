/*
 * interrupt.c
 *
 *  Created on: 7 nov. 2018
 *      Author: Anthony
 */

#include "interrupt.h"

static XScuTimer TimerScuInstance;
XScuWdt WdtScuInstance;
static XScuGic Intc;
int count_timer = 0;
int count_wdt = 0;
static XTime tStart;
static XTime tEnd;
bool run_flag;

void timer_scu_callback(XScuTimer * TimerInstance)
{
	count_timer++;
    // Clear timer's interrupt
    XScuTimer_ClearInterruptStatus(TimerInstance);
}

void wdt_scu_callback(XScuWdt *WdtInstance){
	count_wdt++;
	xil_printf("count_timer = %d | count_wdt = %d\r\n", count_timer, count_wdt);
	if(count_wdt >= 5) run_flag = false;
	XTime_GetTime(&tEnd);
	printf("Output took %llu clock cycles.\r\n", 2*(tEnd - tStart));
	printf("Output took %.2f us.\r\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000));
	printf("Output took %.2f ms.\r\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000));
	XTime_GetTime(&tStart);
	if(XScuWdt_IsWdtExpired(&WdtScuInstance)) xil_printf("%s: Watch dog has expired\r\n", __func__);
	else xil_printf("%s: Watch dog has not expired\r\n", __func__);
	if(XScuWdt_IsTimerExpired(&WdtScuInstance)) xil_printf("%s: Wdt Timer has expired\r\n", __func__);
	else xil_printf("%s: Wdt Timer has not expired\r\n", __func__);

	// Clear Watchdog's interrupt
	XScuWdt_WriteReg(WdtInstance->Config.BaseAddr,
						 XSCUWDT_ISR_OFFSET,
						 XSCUWDT_ISR_EVENT_FLAG_MASK);
	XScuWdt_RestartWdt(WdtInstance);	// Reload the counter for the wdt
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
	 * WARNING: If period is modified, must adapt counter in function timer_scu_callback()
	 */
	TimerLoadValue = XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2;

	XScuTimer_LoadTimer(&TimerScuInstance, TimerLoadValue);
	return Status;
}

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

	/*
	 * Put the watchdog timer in timer mode.
	 */
	XScuWdt_SetWdMode(&WdtScuInstance); // Wdt mode: no callback, reset with a flag on reboot
	//XScuWdt_SetTimerMode(&WdtScuInstance); // Timer mode: callback then need to restart the timer (= reload)

	if(XScuWdt_IsWdtExpired(&WdtScuInstance)) xil_printf("%s: Watch dog has expired\r\n", __func__);
	else xil_printf("%s: Watch dog has not expired\r\n", __func__);
	if(XScuWdt_IsTimerExpired(&WdtScuInstance)) xil_printf("%s: Wdt Timer has expired\r\n", __func__);
	else xil_printf("%s: Wdt Timer has not expired\r\n", __func__);
	usleep(300000);
	return Status;
}

int setup_interrupts(void)
{
	static XScuGic_Config *IntcConfig;
	int Status = XST_SUCCESS;
	uint32_t Reg;

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
	count_timer = 0;
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
	XScuGic_RegisterHandler(INTC_BASE_ADDR, WDT_IRPT_INTR,
					(Xil_ExceptionHandler)wdt_scu_callback,
					(void *)&WdtScuInstance);
	/*
	 * Enable the interrupt for scu timer.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, WDT_IRPT_INTR);

	/*
	 * Enable non-critical exceptions.
	 */
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	XScuTimer_EnableInterrupt(&TimerScuInstance);
	XScuTimer_Start(&TimerScuInstance);
	/*
	 * enable wdt interrupt
	 */
	Reg = XScuWdt_GetControlReg(&WdtScuInstance);
	XScuWdt_SetControlReg(&WdtScuInstance, Reg | XSCUWDT_CONTROL_IT_ENABLE_MASK);

	if(XScuWdt_IsWdtExpired(&WdtScuInstance)) xil_printf("%s: Watch dog has expired\r\n", __func__);
	else xil_printf("%s: Watch dog has not expired\r\n", __func__);
	if(XScuWdt_IsTimerExpired(&WdtScuInstance)) xil_printf("%s: Wdt Timer has expired\r\n", __func__);
	else xil_printf("%s: Wdt Timer has not expired\r\n", __func__);

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
	XTime_GetTime(&tStart);



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

	Status = setup_scu_wdt_int();
	if(Status != XST_SUCCESS){
		xil_printf("In %s: Scu Wdt failed...\r\n",
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
	uint32_t Reg;
	XScuTimer_DisableInterrupt(&TimerScuInstance);
	XScuTimer_Stop(&TimerScuInstance);
	Reg = XScuWdt_GetControlReg(&WdtScuInstance);
	XScuWdt_SetControlReg(&WdtScuInstance, Reg | ~(XSCUWDT_CONTROL_IT_ENABLE_MASK));
	XScuWdt_Stop(&WdtScuInstance);
	Xil_ICacheDisable();
	Xil_DCacheDisable();
	return;
}
