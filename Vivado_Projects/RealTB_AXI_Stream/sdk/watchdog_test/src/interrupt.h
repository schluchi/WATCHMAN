/*
 * interrupt.h
 *
 *  Created on: 7 nov. 2018
 *      Author: Anthony
 */

#ifndef SRC_INTERRUPT_H_
#define SRC_INTERRUPT_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "xil_printf.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xil_cache.h"
#include "xtime_l.h"
#include "xscugic.h"
#include "xscutimer.h"
#include "xscuwdt.h"


// Parameter Definitions
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define INTC_BASE_ADDR		XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR
#define TIMER_DEVICE_ID		XPAR_SCUTIMER_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR
#define WDT_DEVICE_ID		XPAR_SCUWDT_0_DEVICE_ID
#define WDT_IRPT_INTR		XPAR_SCUWDT_INTR

#define WDT_LOAD_VALUE		0x13FFFFFF // = 1sec

//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
void timer_scu_callback(XScuTimer * TimerInstance);
void wdt_scu_callback(XScuWdt *WdtInstance);
int setup_scu_timer_int(void);
int setup_scu_wdt_int(void);
int setup_interrupts(void);
int init_interrupts();
void cleanup_interrupts();


#endif /* SRC_INTERRUPT_H_ */
