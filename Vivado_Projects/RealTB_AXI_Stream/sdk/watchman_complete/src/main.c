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
#include "file_hm.h"
#include "global.h"

/* Extern global variables */
extern struct netif *echo_netif;
extern volatile bool run_flag;
extern volatile bool flag_ttcps_timer;
extern volatile bool flag_scu_timer;
extern XScuWdt WdtScuInstance;
extern volatile bool flag_assertion;
extern volatile bool flag_while_loop;

int main()
{
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;
	struct netif server_netif;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	printf("\n\r\n\r------START------\r\n");

	/* Initialize the global variables */
	init_global_var();

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
	IP4_ADDR(&pc_ipaddr, 192, 16, 1, 11);
	print_ip("\r\nPC IP: ", &pc_ipaddr);

	/* Set the UDP connections and callback for data and commands */
	setup_udp_settings(pc_ipaddr);

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
	}

	/* Close and clear everything */
	cleanup_interrupts();
	cleanup_udp();
	cleanup_global_var();
	printf("-------END-------\r\n");

	return 0;
}


