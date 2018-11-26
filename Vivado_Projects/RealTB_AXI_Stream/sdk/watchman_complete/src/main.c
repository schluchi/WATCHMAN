/*
 * main.c
 *
 *  Created on: 16 nov. 2018
 *      Author: Anthony
 */

#include "lwip/init.h"
#include "netif/xadapter.h"
#include "platform_config.h"
#include "xparameters.h"
#include "udp_peripheral.h"
#include "axis_peripheral.h"
#include "global.h"

void init_global_var(void);
void cleanup_global_var(void);

int main()
{
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;
	struct netif server_netif;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	printf("\n\r\n\r------START------\r\n");

	/* Initialize the global variables */
	init_global_var();

	/* Initialize "echo_netif" to avoid warnings with function "xemac_add"*/
	echo_netif = &server_netif;

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
	if (!xemac_add(echo_netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
		printf("Error adding N/W interface\n\r");
		printf("-------END-------\r\n");
		return -1;
	}

	/* now enable interrupts */
	enable_interrupts();

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
	IP4_ADDR(&pc_ipaddr, 192, 16, 1, 11);
	print_ip("\r\nPC IP: ", &pc_ipaddr);

	/* Set the UDP connections and callback for data and commands */
	setup_udp_settings(pc_ipaddr);

	while (run_flag);

	cleanup_udp();
	cleanup_interrupts();
	cleanup_global_var();
	printf("-------END-------\r\n");

	return 0;
}

void init_global_var(void){
	count_ttcps_timer = 0;
	count_scu_timer = 0;
	run_flag = true;
	stream_flag = false;
	PtrData = (int *)malloc(NBR_DATA);
	nbre_of_bytes = 0;
}

void cleanup_global_var(void){
	free(PtrData);
}


