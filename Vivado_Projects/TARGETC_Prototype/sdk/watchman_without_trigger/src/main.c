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
extern int flag_axidma_rx[4];

/* Global variables */
static struct netif server_netif;

/*TEST***************************/
extern data_list* first_element;
extern data_list* last_element;
extern uint16_t pedestal[512][16][32];
/********************************/
int main()
{
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;
	int group;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	printf("\n\r\n\r------START------\r\n");

	/* Initialize the global variables */
	if(init_global_var() == XST_SUCCESS) xil_printf("Global variables initialization pass!\r\n");
	else{
		xil_printf("-------END-------\r\n");
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

	/* TEST**************************/
	//dma_first_adress();
	int i,k,m;
	data_list* tmp_ptr;
	for(m=0; m<512; m++){
		for(k=0; k<16; k++){
			for(i=0; i<32; i++) pedestal[m][k][i] = 1024;
		}
	}

	first_element->data.data_struct.wdo_time = 0x1111111122222222;
	first_element->data.data_struct.dig_time = 0;
	first_element->data.data_struct.info = 0x001;
	first_element->data.data_struct.wdo_id = 511;
	for(k=0; k<16; k++){
		for(i=0; i<32; i++) first_element->data.data_struct.data[k][i] = 1024-i;
	}

	tmp_ptr = last_element;
	last_element = (data_list *)malloc(sizeof(data_list));
	if(!last_element){
		xil_printf("malloc for last_element failed in function, %s!\r\n", __func__);
	}
	last_element->next = NULL;
	last_element->previous = tmp_ptr;
	tmp_ptr->next = last_element;
	last_element->data.data_struct.wdo_time = 0x5555555555555555;
	last_element->data.data_struct.dig_time = 0;
	last_element->data.data_struct.info = 0x001;
	last_element->data.data_struct.wdo_id = 0;
	for(k=0; k<16; k++){
		for(i=0; i<32; i++) last_element->data.data_struct.data[k][i] = 1024-(i+32);
	}

	tmp_ptr = last_element;
	last_element = (data_list *)malloc(sizeof(data_list));
	if(!last_element){
		xil_printf("malloc for last_element failed in function, %s!\r\n", __func__);
	}
	last_element->next = NULL;
	last_element->previous = tmp_ptr;
	tmp_ptr->next = last_element;
	last_element->data.data_struct.wdo_time = 0x0123456789876543;
	last_element->data.data_struct.dig_time = 0;
	last_element->data.data_struct.info = 0x001;
	last_element->data.data_struct.wdo_id = 1;
	for(k=0; k<16; k++){
		for(i=0; i<32; i++) last_element->data.data_struct.data[k][i] = 1024-(i+64);
	}

	tmp_ptr = last_element;
	last_element = (data_list *)malloc(sizeof(data_list));
	if(!last_element){
		xil_printf("malloc for last_element failed in function, %s!\r\n", __func__);
	}
	last_element->next = NULL;
	last_element->previous = tmp_ptr;
	tmp_ptr->next = last_element;
	last_element->data.data_array[0] = 0x89876543;
	last_element->data.data_array[1] = 0x01234567;
	last_element->data.data_struct.dig_time = 0;
	last_element->data.data_struct.info = 0x015;
	last_element->data.data_struct.wdo_id = 2;
	for(k=0; k<16; k++){
		for(i=0; i<32; i++) last_element->data.data_struct.data[k][i] = 1024-(i+32);
	}

	tmp_ptr = last_element;
	last_element = (data_list *)malloc(sizeof(data_list));
	if(!last_element){
		xil_printf("malloc for last_element failed in function, %s!\r\n", __func__);
	}
	last_element->next = NULL;
	last_element->previous = tmp_ptr;
	tmp_ptr->next = last_element;
	last_element->data.data_struct.wdo_time = 0x0000000600000007;
	last_element->data.data_struct.dig_time = 0;
	last_element->data.data_struct.info = 0x004;
	last_element->data.data_struct.wdo_id = 56;
	for(k=0; k<16; k++){
		for(i=0; i<32; i++) last_element->data.data_struct.data[k][i] = 1024-i;
	}
	flag_axidma_rx[0]++;
	/******************************/
	flag_while_loop = true;
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


