/*
 * interrupt.h
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#ifndef SRC_INTERRUPT_H_
#define SRC_INTERRUPT_H_

#ifdef __arm__

#include "xparameters.h"
#include "xparameters_ps.h"	/* defines XPAR values */
#include "xil_cache.h"
#include "xscugic.h"
#include "lwip/tcp.h"
#include "xil_printf.h"
#include "platform_config.h"
#include "netif/xadapter.h"
#ifdef PLATFORM_ZYNQ
#include "xparameters.h"
#include "xscutimer.h"
#include "xttcps.h"
#include "xaxidma.h"
#include "xscuwdt.h"
#include "udp_peripheral.h"
#include "axis_peripheral.h"
#include "xtime_l.h"
#include "file_hm.h"

#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_SCUTIMER_DEVICE_ID
#define INTC_BASE_ADDR		XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_0_DIST_BASEADDR
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR

#define TTC_TICK_DEVICE_ID	XPAR_XTTCPS_0_DEVICE_ID
#define TTC_TICK_INTR_ID	XPAR_XTTCPS_0_INTR
#define TTCPS_TIMER_FREQ_HZ	1

#define WDT_DEVICE_ID		XPAR_SCUWDT_0_DEVICE_ID
#define WDT_IRPT_INTR		XPAR_SCUWDT_INTR
#define WDT_LOAD_VALUE		0x27FFFFFE				// = 2sec | 0x13FFFFFF = 1sec

#define RESET_RX_CNTR_LIMIT	400

void assert_callback(const char8 *File, s32 Line);
void timer_scu_callback(XScuTimer * TimerInstance);
void timer_ttcps_callback(XTtcPs * TimerInstance);
void axidma_rx_callback(XAxiDma* AxiDmaInstance);
void wdt_scu_callback(XScuWdt *WdtInstance);
int setup_scu_timer_int(void);
int setup_ttcps_timer_int(void);
int setup_axidma_int(void);
int interrupts_initialization(void);
void enable_interrupts();
int devices_initialization();
void cleanup_interrupts();
#endif
#endif

typedef struct TmrCntrSetup_st{
	u32 OutputHz;	/* Output frequency */
	XInterval Interval;	/* Interval value */
	u8 Prescaler;	/* Prescaler value */
	u32 Options;	/* Option settings */
} TmrCntrSetup;

#endif /* SRC_INTERRUPT_H_ */
