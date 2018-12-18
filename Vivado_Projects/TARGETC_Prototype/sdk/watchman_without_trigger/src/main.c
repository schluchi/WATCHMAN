/*
 * main.c
 *
 *  Created on: 16 nov. 2018
 *      Author: Anthony
 */

#include <stdint.h>
#include "lwip/init.h"
#include "netif/xadapter.h"
#include "platform_config.h"
#include "xparameters.h"
#include "udp_peripheral.h"
#include "axis_peripheral.h"
#include "file_hm.h"
#include "global.h"
#include "iic_DAC_LTC2657.h"

/* Extern global variables */
extern struct netif *echo_netif;
extern volatile bool run_flag;
extern volatile bool stream_flag;
extern volatile bool flag_ttcps_timer;
extern volatile bool flag_scu_timer;
extern XScuWdt WdtScuInstance;
extern volatile bool flag_assertion;
extern volatile bool flag_while_loop;
extern volatile bool flag_axidma_error;
extern volatile bool flag_axidma_rx_done;
extern int flag_axidma_rx[4];
extern int* regptr;

/* Global variables */
static struct netif server_netif;

int main()
{
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;
	int group, timeout, i, j;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	printf("\n\r\n\r------START------\r\n");

	/* Initialize the global variables */
	if(init_global_var() == XST_SUCCESS) xil_printf("Global variables initialization pass!\r\n");
	else{
		xil_printf("-------END-------\r\n");
		return -1;
	}

	/* Initialize the DAC (Vped, Comparator value) */
	if(DAC_LTC2657_initialize() == XST_SUCCESS) xil_printf("DAC initialization pass!\r\n");
	else{
		printf("-------END-------\r\n");
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_VPED,2.00) != XST_SUCCESS){
		xil_printf("DAC: setting vped voltage failed!\r\n");
		printf("-------END-------\r\n");
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_0,1.75) != XST_SUCCESS){
		xil_printf("DAC: setting group 0 voltage failed!\r\n");
		printf("-------END-------\r\n");
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_1,1.75) != XST_SUCCESS){
		xil_printf("DAC: setting group 1 voltage failed!\r\n");
		printf("-------END-------\r\n");
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_2,1.75) != XST_SUCCESS){
		xil_printf("DAC: setting group 2 voltage failed!\r\n");
		printf("-------END-------\r\n");
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_3,1.75) != XST_SUCCESS){
		xil_printf("DAC: setting group 3 voltage failed!\r\n");
		printf("-------END-------\r\n");
		return -1;
	}

	/* Initialize "echo_netif" to avoid warnings with function "xemac_add" */
	echo_netif = &server_netif;

	// Mount SD Card and create log file
	FRESULT result = mount_sd_card();
	if (result != 0) return -1;

	/* Initial the interrupt timer, axidma, ... */
	if(init_interrupts() == XST_SUCCESS) xil_printf("Interrupts initialization pass!\r\n");
	else{
		xil_printf("-------END-------\r\n");
		return -1;
	}

	/* Initilize the LWip */
	lwip_init();

	/* now enable interrupts */
	enable_interrupts();

	/* Add network interface to the netif_list, and set it as default */
	ipaddr.addr = 0;
	gw.addr = 0;
	netmask.addr = 0;
	if (!xemac_add(echo_netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
		printf("Error adding N/W interface\n\r");
		printf("-------END-------\r\n");
		return -1;
	}

	/* specify that the network if is up */
	netif_set_default(echo_netif);
	netif_set_up(echo_netif);

	/* initliaze IP addresses to be used */
	IP4_ADDR(&(echo_netif->ip_addr),  192, 168,   1, 10);
	IP4_ADDR(&(echo_netif->netmask), 255, 255, 255,  0);
	IP4_ADDR(&(echo_netif->gw),      192, 168,   1,  1);
	ipaddr.addr = echo_netif->ip_addr.addr;
	gw.addr = echo_netif->gw.addr;
	netmask.addr = echo_netif->netmask.addr;
	print_ip_settings(&ipaddr, &netmask, &gw);

	/* Set the PC address */
	IP4_ADDR(&pc_ipaddr, 192, 168, 1, 11);
	print_ip("\r\nPC IP: ", &pc_ipaddr);

	/* Set the UDP connections and callback for data and commands */
	if(setup_udp_settings(pc_ipaddr) < 0){
		printf("Error setting up the UDP interface\n\r");
		printf("-------END-------\r\n");
		return -1;
	}
	else xil_printf("UDP started @ port %d for data and @ port %d for commands\n\r", PORT_DATA, PORT_CMD);

	// Initialise control register
	ControlRegisterWrite((int)NULL,INIT);
	// software reset PL side
	ControlRegisterWrite(SWRESET_MASK,DISABLE);
	// Reset TargetC's registers
	ControlRegisterWrite(REGCLR_MASK,DISABLE);
	usleep(100000);
	ControlRegisterWrite(SWRESET_MASK,ENABLE);
	// Waiting on PL's clocks to be ready
	while((regptr[TC_STATUS_REG] & LOCKED_MASK) != LOCKED_MASK){
		sleep(1); //sleep 100ms
	}
	printf("PL's clock ready\r\n");
	// Initialise TargetC's registers
	SetTargetCRegisters();

	/* Test pattern */
	data_list* tmp_ptr  = (data_list *)malloc(sizeof(data_list));
	if(!tmp_ptr){
		printf("malloc for tmp_ptr failed in function, %s!\r\n", __func__);
		printf("-------END-------\r\n");
		return -1;
	}
	tmp_ptr->next = NULL;
	tmp_ptr->previous = NULL;
	XAxiDma_SimpleTransfer_Hej((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

	regptr[TC_FSTWINDOW_REG] = 1;
	regptr[TC_NBRWINDOW_REG] = 1;
	WriteRegister(TC_TPG_REG,	0x50A);	// TPG value

	ControlRegisterWrite(SMODE_MASK ,ENABLE);
	ControlRegisterWrite(SS_TPG_MASK ,DISABLE); // disable for TestPattern
	ControlRegisterWrite(WINDOW_MASK,ENABLE);
	usleep(1000);
	ControlRegisterWrite(WINDOW_MASK,DISABLE); // PL side starts on falling edge

	timeout = 10;
	while(timeout && !flag_axidma_rx_done){
		sleep(1);
		timeout--;
	}
	if(timeout <= 0){
		printf("Timeout: TPG failed!\r\n");
		printf("-------END-------\r\n");
		return -1;
	}
	else flag_axidma_rx_done = false;

	Xil_DCacheInvalidateRange((UINTPTR)tmp_ptr->data.data_array, SIZE_DATA_ARRAY_BYT);

	if(tmp_ptr->data.data_struct.wdo_id == 1){
		for(j=0; j<32; j++){
			for(i=0; i<16; i++){
				if(tmp_ptr->data.data_struct.data[i][j] != 0x50A){
					printf("TPG failed: data[%d][%d] = 0x%X\r\n", i, j, (uint)tmp_ptr->data.data_struct.wdo_id);
					printf("-------END-------\r\n");
					return -1;
				}
			}
		}
	}
	else{
		printf("TPG failed: window ID = 0x%X\r\n", (uint)tmp_ptr->data.data_struct.wdo_id);
		printf("-------END-------\r\n");
		return -1;
	}
	free(tmp_ptr);
	ControlRegisterWrite(SS_TPG_MASK ,ENABLE); // enable for real data

	flag_while_loop = true;
	//dma_first_adress();
	while (run_flag){
		if(flag_assertion) break;

		if(flag_ttcps_timer){
			update_timefile();
			flag_ttcps_timer = false;
		}

		if(flag_scu_timer){
			XScuWdt_RestartWdt(&WdtScuInstance);	// Reload the counter for the wdt
			flag_scu_timer = false;
		}

		for(group=0; group<4; group++){
			if(flag_axidma_rx[group] > 0){
				dma_received_data(group);
				flag_axidma_rx[group]--;
			}
		}
	}

	/* Close and clear everything */
	cleanup_interrupts();
	cleanup_udp();
	cleanup_global_var();
	printf("-------END-------\r\n");

	return 0;
}


