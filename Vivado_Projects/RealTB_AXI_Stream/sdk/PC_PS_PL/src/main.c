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
#include "platform_zynq.h"
#include "echo.h"
#include "AXIS_Peripheral.h"

#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID

#define RX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR
#define TX_INTR_ID		XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR

#define INTC_DEVICE_ID          XPAR_SCUGIC_SINGLE_DEVICE_ID

#define INTC		XScuGic
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



int data[CHANNEL][WINDOW][SAMPLE];
char stream[MAX_STREAM_SIZE];

/* missing declaration in lwIP */
void lwip_init();
extern volatile int dhcp_timoutcntr;
err_t dhcp_start(struct netif *netif);

extern volatile int count_ttcps_timer;

extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;
static struct netif server_netif;
struct netif *echo_netif;

extern volatile bool run_flag;
extern bool stream_flag;

//JOHN
extern XAxiDma AxiDmaInstance;
extern char axidma_error, axidma_rx_done;
static int XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes);
static void XAxiDma_S2MM_DMASR(void);
static void XAxiDma_S2MM_DMACR(void);
void decToHexa(int n) ;
#define PACKETS 10
static int PacketsToSend = 0;
int Status;
int *PtrData;

int main()
{
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;

	int *regbank = XPAR_AXIS_TEST_COMPONENT_0_S00_AXI_BASEADDR;
	regbank[CONTROL_REG] = 0;			// Stop
	regbank[NBR_OF_PACKETS_REG] = 0;	// Nbr of packets = 0
	regbank[CONTENT_PACKET_1] = 0;		// Content of 1st packet = 0

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =
	{ 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	echo_netif = &server_netif;

	xil_printf("\n\r\n\r------START------\r\n");

	init_platform();

    ipaddr.addr = 0;
	gw.addr = 0;
	netmask.addr = 0;

	print_app_header();

	lwip_init();

	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(echo_netif, &ipaddr, &netmask,
						&gw, mac_ethernet_address,
						PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return -1;
	}

	netif_set_default(echo_netif);

	/* now enable interrupts */
	platform_enable_interrupts();

	/* specify that the network if is up */
	netif_set_up(echo_netif);

	/* Create a new DHCP client for this interface.
	 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
	 * the predefined regular intervals after starting the client.
	 */
	dhcp_start(echo_netif);
	dhcp_timoutcntr = 50;
	xil_printf("Time out : 5...");

	while(((echo_netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0)) {
		xemacif_input(echo_netif);
	}
	if (dhcp_timoutcntr <= 0) {		// No DHCP, so the IP as to be fixed manually
		if ((echo_netif->ip_addr.addr) == 0) {
			xil_printf("DHCP Timeout\r\n");
			xil_printf("Configuring default IP of 192.168.1.10\r\n");
			IP4_ADDR(&(echo_netif->ip_addr),  192, 168,   1, 10);
			IP4_ADDR(&(echo_netif->netmask), 255, 255, 255,  0);
			IP4_ADDR(&(echo_netif->gw),      192, 168,   1,  1);
		}
	}

	ipaddr.addr = echo_netif->ip_addr.addr;
	gw.addr = echo_netif->gw.addr;
	netmask.addr = echo_netif->netmask.addr;

	print_ip_settings(&ipaddr, &netmask, &gw);

	IP4_ADDR(&pc_ipaddr,			 192, 168,   1, 11 );

	/* start the application (web server, rxtest, txtest, etc..) */
	start_application(pc_ipaddr);

	//err_t ret_transfert;
	XTime tStart, tEnd;

	for(int k= 1; k < 5 ; k++){
		PacketsToSend = PACKETS*k;
		regbank[NBR_OF_PACKETS_REG] = PacketsToSend;
		regbank[CONTENT_PACKET_1] = 100*k;
		if(k == 0) regbank[MODE_REG] = RAMP_MODE;
		else if(k == 1) regbank[MODE_REG] = SAW_MODE;
		else if(k == 2) regbank[MODE_REG] = BIT_MODE;
		else regbank[MODE_REG] = RAMP_MODE;


		if(regbank[NBR_OF_PACKETS_REG] != PacketsToSend){
			xil_printf("Error launding regbank[NBR_OF_PACKETS_REG]\r\n");
			break;
		}
		if(regbank[CONTENT_PACKET_1] != 100*k){
			xil_printf("Error launding regbank[CONTENT_PACKET_1]\r\n");
			break;
		}

		xil_printf("Packet N%d :\tNbr:%d\tContent:%d\tMode:%d\r\n",k,regbank[NBR_OF_PACKETS_REG],regbank[CONTENT_PACKET_1],regbank[MODE_REG]);

		PtrData = malloc(sizeof(int)*PacketsToSend);
		if(PtrData == NULL){
			xil_printf("Malloc Failed\r\n");
			return XST_FAILURE;
		}
		else{
			for(int i=0; i < PacketsToSend;i++){
				PtrData[i] = i;
			}
		}

		Xil_DCacheFlushRange((UINTPTR)PtrData, PacketsToSend*sizeof(int));

		Status = XAxiDma_SimpleTransfer_Hej(&AxiDmaInstance,(UINTPTR)PtrData, (PacketsToSend*sizeof(int)));
		if (Status == XST_FAILURE) {
			xil_printf("Failure\r\n");
			return XST_FAILURE;
		}
		else if(Status == XST_INVALID_PARAM){
			xil_printf("Invalid\r\n");
				return XST_INVALID_PARAM;
		}

		//*** ResetTimer *************************/
		int TimeOutCnt = 5;

		//*** Start Transfer *********************/
		regbank[CONTROL_REG] = 1;		//Enable Axi Peripheral to start sending data

		while (!axidma_rx_done && !axidma_error && TimeOutCnt) {
				/* NOP */
				sleep(1);
				TimeOutCnt--;
				xil_printf("...\r\n");
		}
		if(!TimeOutCnt){
			xil_printf(">> Time Out Reached 5sec\r\n");
		}
		else if(axidma_rx_done){
			xil_printf(">> RxDone Interrupt Caught\r\n");
			xil_printf("\tVerifying...");

			Xil_DCacheInvalidateRange((UINTPTR)PtrData, PacketsToSend*sizeof(int));


			int cst = regbank[CONTENT_PACKET_1];
			switch(regbank[MODE_REG] ){
				case RAMP_MODE:
					for(int a=0; a < PacketsToSend; a++){
						if(PtrData[a] != (a+cst)){
							xil_printf("FAILED\r\n");
							axidma_error = 1;
							break;
						}
					}
					break;

				case SAW_MODE:
					for(int a=0; a < PacketsToSend; a++){
						if(PtrData[a] != (a%5)){
							xil_printf("FAILED\r\n");
							axidma_error = 1;
							break;
						}
					}
					break;

				case BIT_MODE:
					for(int a=0; a < PacketsToSend; a++){
						if(PtrData[a] != (a%2)){
							xil_printf("FAILED\r\n");
							axidma_error = 1;
							break;
						}
					}
					break;

			}

			//else{
				xil_printf("Packet :\tNbr:%d\tContent:%d\tMode:%d\r\n",regbank[NBR_OF_PACKETS_REG],regbank[CONTENT_PACKET_1],regbank[MODE_REG]);
				switch(regbank[MODE_REG] ){
					case RAMP_MODE:
						for(int a=0; a < PacketsToSend; a++){
							decToHexa(&PtrData[a]);
							xil_printf("\tPtrData[%d]\t= %d\t (= %d)\r\n",a,PtrData[a],a+cst);
						}
						break;

					case SAW_MODE:
						for(int a=0; a < PacketsToSend; a++){
							decToHexa(&PtrData[a]);
							xil_printf("\tPtrData[%d]\t= %d\t (= %d)\r\n",a,PtrData[a],a%5);
						}
						break;

					case BIT_MODE:
						for(int a=0; a < PacketsToSend; a++){
							decToHexa(&PtrData[a]);
							xil_printf("\tPtrData[%d]\t= %d\t (= %d)\r\n",a,PtrData[a],a%2);
						}
						break;
				}
			//}
		}
		else if(axidma_error){
			xil_printf(">> Error\r\n");
			break;
		}

		if(axidma_error){
			xil_printf("General Error while Reading back values\r\n");
			//break;
		}

		//regbank[NBR_OF_PACKETS_REG] = 0;
		regbank[CONTROL_REG] = 0;		//Enable Axi Peripheral to start sending data
		free(PtrData);

		axidma_error = 0;
		axidma_rx_done = 0;
		xil_printf("\r\n");
		//sleep(1);
	}
	xil_printf("test john soft done \r\n");
	while (run_flag){
		if(stream_flag){
			XTime_GetTime(&tStart);
			while(stream_flag);
			XTime_GetTime(&tEnd);
			printf("Output took %llu clock cycles.\n", 2*(tEnd - tStart));
			printf("Output took %.2f us.\n",1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000));
		}
	}



	/* never reached */
	cleanup_platform();
	xil_printf("-------END-------\r\n");

	return 0;
}


/****************************************************************************
*
* This is the DMA Simple Packet Transfert Function
*
******************************************************************************/
static int XAxiDma_SimpleTransfer_Hej(XAxiDma *InstancePtr, UINTPTR BuffAddr, int LengthOfBytes){

	int n;
	//xil_printf("Addr:\t%d %d\r\n",LOWER_32_BITS(BuffAddr),UPPER_32_BITS(BuffAddr));

	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_OFFSET, LOWER_32_BITS(BuffAddr));
	//XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_MSB_OFFSET, UPPER_32_BITS(BuffAddr));
	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_MSB_OFFSET, 0);

/*	xil_printf("Reg Low:\t");
	n = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_OFFSET);
	decToHexa(n);
	xil_printf("\r\n");

	xil_printf("Reg High:\t");
	n = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_DESTADDR_MSB_OFFSET);
	decToHexa(n);
	xil_printf("\r\n");
*/

	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_CR_OFFSET,XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_CR_OFFSET)| XAXIDMA_CR_RUNSTOP_MASK);

	XAxiDma_WriteReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET, XAXIDMA_BUFFLEN_OFFSET, LengthOfBytes);

}

/****************************************************************************
*
* Read The bits of S2MM_DMACR Register
*
******************************************************************************/
static void XAxiDma_S2MM_DMACR(void){
	int Reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_CR_OFFSET);

	/* Register Set S2MM_DMACR
		0		RS
		1		n.a
		2		Reset
		3		KeyHole
		4 		Cyclic BD Enable
		5-11	n.a
		12		IOC_IrqEn
		13		Dly_IrqEn
		14		Err_IrqEn
		15		n.a
		16-23	IRQThreshold
		24-31	IRQDelay
	*/

	xil_printf(">> XAxiDma_S2MM_DMACR\r\n");
	xil_printf("\t%d\tRS\r\n",Reg & 0x00000001);
	xil_printf("\t%d\tReset\r\n",(Reg & 0x00000004)>>2);
	xil_printf("\t%d\tKeyHole\r\n",(Reg & 0x00000008)>>3);
	xil_printf("\t%d\tCyclic BD Enable\r\n",(Reg & 0x00000010)>>4);
	xil_printf("\t%d\tIOC_IrqEn\r\n",(Reg & 0x00001000)>>12);
	xil_printf("\t%d\tDly_IrqEn\r\n",(Reg & 0x00002000)>>13);
	xil_printf("\t%d\tErr_IrqEn\r\n",(Reg & 0x00004000)>>14);
	xil_printf("\t%d\tIRQThershold\r\n",(Reg & 0x00FF0000)>>16);
	xil_printf("\t%d\tIRQDelay\r\n",(Reg & 0xFF000000)>>24);



}
/****************************************************************************
*
* Read The bits of S2MM_DMASR Register
*
******************************************************************************/
static void XAxiDma_S2MM_DMASR(void){
	int Reg = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR+XAXIDMA_RX_OFFSET,XAXIDMA_SR_OFFSET);

	/* Register Set S2MM_DMASR
		0		Halted
		1		Idle
		2		n.a.
		3		SGincld
		4 		DMAIntErr
		5		DMASlvErr
		6		DMADecErr
		7		n.a.
		8		SGIntErr
		9		SGSlvErr
		10		SGDecErr
		11		n.a.
		12		IOC_Irq
		13		Dly_Irq
		14		Err_Irq
		15		n.a.
		16-23	IRQThresholdSts
		24-31	IRQDelaySts
	*/

	xil_printf(">> XAxiDma_S2MM_DMASR\r\n");
	xil_printf("\t%d\tHalted\r\n",Reg & 0x00000001);
	xil_printf("\t%d\tIdle\r\n",(Reg & 0x00000002)>>1);
	xil_printf("\t%d\tSGincld\r\n",(Reg & 0x00000008)>>3);
	xil_printf("\t%d\tDMAIntErr\r\n",(Reg & 0x00000010)>>4);
	xil_printf("\t%d\tDMASlvErr\r\n",(Reg & 0x00000020)>>5);
	xil_printf("\t%d\tDMADecErr\r\n",(Reg & 0x00000040)>>6);

	xil_printf("\t%d\tSGIntErr\r\n",(Reg & 0x00000100)>>8);
	xil_printf("\t%d\tSGSlvErr\r\n",(Reg & 0x00000200)>>9);
	xil_printf("\t%d\tSGDecErr\r\n",(Reg & 0x00000400)>>10);

	xil_printf("\t%d\tIOC_Irq\r\n",(Reg & 0x00001000)>>12);
	xil_printf("\t%d\tDly_Irq\r\n",(Reg & 0x00002000)>>13);
	xil_printf("\t%d\tErr_Irq\r\n",(Reg & 0x00004000)>>14);

	xil_printf("\t%d\tIRQThresholdSts\r\n",(Reg & 0x00FF0000)>>16);
	xil_printf("\t%d\tIRQDelaySts\r\n",(Reg & 0xFF000000)>>24);

}

void decToHexa(int n)
{
    // char array to store hexadecimal number
    char hexaDeciNum[100];

    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 16;

        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n/16;
    }

    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--)
        xil_printf("%c",hexaDeciNum[j]);


}
