/******************************************************************************
*
* Copyright (C) 2009 - 2017 Xilinx, Inc.  All rights reserved.
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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "xparameters.h"

#include "netif/xadapter.h"
#include "data_test.h"

#include "platform_config.h"
#if defined (__arm__) || defined(__aarch64__)
#include "xil_printf.h"
#endif

#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "xil_cache.h"
/////////////////////////////////////////////////////////////////////////////

#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"
#include "xtime_l.h"
#include "interrupt.h"
#include "udp_peripheral.h"
#include "axis_peripheral.h"
#include "utility.h"
#include "log_file.h"

#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID
#define RX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR
#define TX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR
#define INTC_DEVICE_ID  XPAR_SCUGIC_SINGLE_DEVICE_ID
#define INTC			XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler


/* Timeout loop counter for reset */
#define RESET_TIMEOUT_COUNTER	10000

/* Array length and the number of bytes to transfer */
#define ARRAY_LENGTH		50
#define BYTES_TO_TRANSFER	4*ARRAY_LENGTH
///////////////////////////////////////////////////////////////////////////////////
#if LWIP_IPV6==1
#include "lwip/ip.h"
#else
#if LWIP_DHCP==1
#include "lwip/dhcp.h"
#endif
#endif

/* missing declaration in lwIP */
void lwip_init();
err_t dhcp_start(struct netif *netif);

#if LWIP_DHCP==1
extern volatile int dhcp_timoutcntr;
#endif
static struct netif server_netif;
struct netif *echo_netif;

extern volatile int count_ttcps_timer;
extern volatile bool run_flag;
extern bool stream_flag;
extern bool flag_ttcps_timer;

int main()
{
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =
	{ 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	echo_netif = &server_netif;

	xil_printf("\n\r\n\r------START------\r\n");

	/* Initial the interrupt timer, axidma, ... */
	if(init_interrupts() == XST_SUCCESS) xil_printf("Interrupts initialization pass!\r\n");
	else{
		xil_printf("-------END-------\r\n");
		return -1;
	}

	/* Initilize the LWip */
	lwip_init();

	/* Add network interface to the netif_list, and set it as default */
	ipaddr.addr = 0;
	gw.addr = 0;
	netmask.addr = 0;
	if (!xemac_add(echo_netif, &ipaddr, &netmask,
						&gw, mac_ethernet_address,
						PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		xil_printf("-------END-------\r\n");
		return -1;
	}

	/* now enable interrupts */
	enable_interrupts();

	/* specify that the network if is up */
	netif_set_default(echo_netif);
	netif_set_up(echo_netif);

	/* If DHCP available, create a new DHCP client for this interface.
	 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
	 * the predefined regular intervals after starting the client.
	 */
	dhcp_start(echo_netif);
	dhcp_timoutcntr = 20; // Time out of 5sec (look at "TimerLoadValue" for the scu timer)
	xil_printf("DHCP Time out : 5...");

	while(((echo_netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0)) {
		xemacif_input(echo_netif);
	}
	if (dhcp_timoutcntr <= 0) {		// No DHCP, so the IP as to be fixed manually
		if ((echo_netif->ip_addr.addr) == 0) {
			xil_printf("DHCP not available, ip settings manually fixed:\r\n");
			IP4_ADDR(&(echo_netif->ip_addr),  192, 168,   1, 10);
			IP4_ADDR(&(echo_netif->netmask), 255, 255, 255,  0);
			IP4_ADDR(&(echo_netif->gw),      192, 168,   1,  1);
		}
	}

	ipaddr.addr = echo_netif->ip_addr.addr;
	gw.addr = echo_netif->gw.addr;
	netmask.addr = echo_netif->netmask.addr;
	print_ip_settings(&ipaddr, &netmask, &gw);

	/* Set the PC address */
	IP4_ADDR(&pc_ipaddr, 192, 16, 1, 11);
	print_ip("\r\nPC IP: ", &pc_ipaddr);

	/* Set the UDP connections and callback for data and commands */
	setup_udp_settings(pc_ipaddr);

	// Mount SD Card and create log file
	FRESULT result = mount_sd_card();
	if (result != 0) return -1;
	result = create_logfile();
	if (result != 0) return -1;


	XTime tStart, tEnd;
	while (run_flag){
		if(stream_flag){
			XTime_GetTime(&tStart);
			while(stream_flag){
				if(flag_ttcps_timer){
					flag_ttcps_timer = false;
					if(dma_transfert(10) == XST_SUCCESS) xil_printf("=> DMA PASSED\r\n");
					else xil_printf("=> DMA FAILED!\r\n");
					result = write_logfile("aloha!!!", 8);
				}
			}
			XTime_GetTime(&tEnd);
			printf("Output took %llu clock cycles.\n", 2*(tEnd - tStart));
			printf("Output took %.2f us.\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000));
		}
	}


	cleanup_interrupts();
	cleanup_udp();
	xil_printf("-------END-------\r\n");

	return 0;
}
