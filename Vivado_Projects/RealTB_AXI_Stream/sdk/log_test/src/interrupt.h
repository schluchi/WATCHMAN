/*
 * interrupt.h
 *
 *  Created on: 9 nov. 2018
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
#include "log_file.h"
#include "time_hm.h"

// Parameter Definitions
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_SCUTIMER_DEVICE_ID
#define INTC_BASE_ADDR		XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR

//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
void timer_scu_callback(XScuTimer * TimerInstance);
int setup_scu_timer_int(void);
int setup_interrupts(void);
int init_interrupts();
void cleanup_interrupts();

#endif /* SRC_INTERRUPT_H_ */
