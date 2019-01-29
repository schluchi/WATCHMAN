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
#include "pedestal.h"
#include "xtime_l.h"

#include "get_20_windows.h"
#include "transfer_function.h"
#include "recover_data.h"

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
extern int* regptr;
extern volatile bool recover_data_flag;
extern volatile bool get_20_windows_flag;
extern volatile bool empty_flag;
extern data_list* first_element;

/* Global variables */
static struct netif server_netif;

/* Typedef */
typedef enum clean_state_enum {GLOBAL_VAR=0x1,INTERRUPT=0x2,UDP=0x4} clean_state_en;
typedef enum dma_stm_enum{IDLE, STREAM, RECOVER_DATA, GET_20_WINDOWS} dma_stm_en;

/* Function declaration */
void end_main(clean_state_en state);

int main()
{
	//static XTime tStart, tEnd;
	ip_addr_t ipaddr, netmask, gw, pc_ipaddr;
	int pmt;
	dma_stm_en state_main = IDLE;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	printf("\n\r\n\r------START------\r\n");

	/* Initialize the global variables */
	if(init_global_var() == XST_SUCCESS) xil_printf("Global variables initialization pass!\r\n");
	else{
		end_main(GLOBAL_VAR);
		return -1;
	}

	/* Initialize "echo_netif" to avoid warnings with function "xemac_add" */
	echo_netif = &server_netif;

	// Mount SD Card and create log file
	FRESULT result = mount_sd_card();
	if (result != 0){
		end_main(GLOBAL_VAR);
		return -1;
	}

	/* Initial the interrupt timer, axidma, ... */
	if(init_interrupts() == XST_SUCCESS) xil_printf("Interrupts initialization pass!\r\n");
	else{
		end_main(GLOBAL_VAR | INTERRUPT);
		return -1;
	}

	/* Initilize the LWip */
	lwip_init();

	/* now enable interrupts */
	enable_interrupts();

	/* Initialize the DAC (Vped, Comparator value) */
	if(DAC_LTC2657_initialize() == XST_SUCCESS) xil_printf("DAC initialization pass!\r\n");
	else{
		end_main(GLOBAL_VAR);
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_VPED,VPED_ANALOG) != XST_SUCCESS){
		xil_printf("DAC: setting vped voltage failed!\r\n");
		end_main(GLOBAL_VAR);
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_0,THRESHOLD_CMP) != XST_SUCCESS){
		xil_printf("DAC: setting group 0 voltage failed!\r\n");
		end_main(GLOBAL_VAR);
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_1,THRESHOLD_CMP) != XST_SUCCESS){
		xil_printf("DAC: setting group 1 voltage failed!\r\n");
		end_main(GLOBAL_VAR);
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_2,THRESHOLD_CMP) != XST_SUCCESS){
		xil_printf("DAC: setting group 2 voltage failed!\r\n");
		end_main(GLOBAL_VAR);
		return -1;
	}
	if(DAC_LTC2657_SetChannelVoltage(DAC_GRP_3,THRESHOLD_CMP) != XST_SUCCESS){
		xil_printf("DAC: setting group 3 voltage failed!\r\n");
		end_main(GLOBAL_VAR);
		return -1;
	}


	/* Add network interface to the netif_list, and set it as default */
	ipaddr.addr = 0;
	gw.addr = 0;
	netmask.addr = 0;
	if (!xemac_add(echo_netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
		printf("Error adding N/W interface\n\r");
		end_main(GLOBAL_VAR | INTERRUPT);
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
		end_main(GLOBAL_VAR | INTERRUPT | UDP);
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
	// Initialize TargetC's registers
	SetTargetCRegisters();

	/* Test pattern */
	if(test_TPG() == XST_SUCCESS) printf("TestPattern Generator pass!\r\n");
	else{
		printf("TestPattern Generator failed!\n\r");
		end_main(GLOBAL_VAR | INTERRUPT | UDP);
		return -1;
	}

	/* Initialize pedestal */
	if(init_pedestals() == XST_SUCCESS) printf("Pedestal initialization pass!\r\n");
	else{
		printf("Pedestal initialization failed!\n\r");
		end_main(GLOBAL_VAR | INTERRUPT | UDP);
		return -1;
	}

	/* Initialize transfer function coefficients */
	if(init_transfer_function() == XST_SUCCESS) printf("Pedestal initialization pass!\r\n");
	else{
		printf("Transfer function initialization failed!\n\r");
		end_main(GLOBAL_VAR | INTERRUPT | UDP);
		return -1;
	}

	flag_while_loop = true;
	//**************************************************************************
	//**************************************************************************

	/*
	 * 				TO DO !!!!!!!!
	 * 	Reactivate wdt, but problem with recover data because of changing dac value (sleep(1) in fct)
	 * 	and be carefull with oher way to send data
	 * 	in infinity loop, when fct return error, after calling fct end_main, log problem and inifity loop to reboot with wdt (like assertion)
	 * 	before infinity loop, do the same, but raise a flag to counterreact the flag_while_loop test in the timer flag
	 */
	//**************************************************************************
	//**************************************************************************
	printf("Start while loop\r\n");
	while (run_flag){
		if(flag_assertion){
			break;
		}

		if(flag_ttcps_timer){
			update_timefile();
			flag_ttcps_timer = false;
		}

		if(flag_scu_timer){
			XScuWdt_RestartWdt(&WdtScuInstance);	// Reload the counter for the wdt
			flag_scu_timer = false;
		}

		switch(state_main){
			case IDLE:
				if(stream_flag && (!recover_data_flag) && (!get_20_windows_flag)){
					XAxiDma_SimpleTransfer_hm((UINTPTR)first_element->data.data_array, SIZE_DATA_ARRAY_BYT);
					ControlRegisterWrite(CPUMODE_MASK,ENABLE); // mode trigger
					state_main = STREAM;
				}
				if(recover_data_flag && (!stream_flag) && empty_flag){
					ControlRegisterWrite(CPUMODE_MASK,DISABLE); // mode with NBRWINDOS and FSTWINDOW
					state_main = RECOVER_DATA;
				}
				if(get_20_windows_flag && (!stream_flag) && empty_flag){
					ControlRegisterWrite(CPUMODE_MASK,DISABLE); // mode with NBRWINDOS and FSTWINDOW
					state_main = GET_20_WINDOWS;
				}
				break;
			case STREAM:
				if((!stream_flag) && empty_flag){
					ControlRegisterWrite(SWRESET_MASK,DISABLE);
					ControlRegisterWrite(CPUMODE_MASK,DISABLE); // mode with NBRWINDOS and FSTWINDOW
					ControlRegisterWrite(SWRESET_MASK,ENABLE);
					state_main = IDLE;
				}
				for(pmt=0; pmt<4; pmt++){
					if(flag_axidma_rx[pmt] > 0){
						dma_received_data(pmt);
						flag_axidma_rx[pmt]--;
					}
				}
				break;
			case RECOVER_DATA:
				if(send_data_transfer_fct() == XST_SUCCESS) printf("Recover data pass!\r\n");
				else{
					printf("Recover data failed!\n\r");
					end_main(GLOBAL_VAR | INTERRUPT | UDP);
					return -1;
				}
				recover_data_flag = false;
				state_main = IDLE;
				break;
			case GET_20_WINDOWS:
				if(get_20_windows_fct() == XST_SUCCESS) printf("Get a 20 windows pass!\r\n");
				else{
					printf("Get a 20 windows failed!\n\r");
					end_main(GLOBAL_VAR | INTERRUPT | UDP);
					return -1;
				}
				get_20_windows_flag = false;
				state_main = IDLE;
				break;
			default:
				state_main = IDLE;
				break;

		}
	}

	/* Close and clear everything */
	end_main(GLOBAL_VAR | INTERRUPT | UDP);

	return 0;
}


void end_main(clean_state_en state){

	if(state & INTERRUPT) cleanup_interrupts();
	if(state & UDP) cleanup_udp();
	if(state & GLOBAL_VAR) cleanup_global_var();
	printf("-------END-------\r\n");
}

